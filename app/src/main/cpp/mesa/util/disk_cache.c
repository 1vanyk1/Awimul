#include <ctype.h>
#include <ftw.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/file.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <inttypes.h>

#include "crc32.h"
#include "debug.h"
#include "rand_xor.h"
#include "u_atomic.h"
#include "mesa-sha1.h"
#include "ralloc.h"
#include "compiler.h"

#include "disk_cache.h"
#include "disk_cache_os.h"

/* The cache version should be bumped whenever a change is made to the
 * structure of cache entries or the index. This will give any 3rd party
 * applications reading the cache entries a chance to adjust to the changes.
 *
 * - The cache version is checked internally when reading a cache entry. If we
 *   ever have a mismatch we are in big trouble as this means we had a cache
 *   collision. In case of such an event please check the skys for giant
 *   asteroids and that the entire Mesa team hasn't been eaten by wolves.
 *
 * - There is no strict requirement that cache versions be backwards
 *   compatible but effort should be taken to limit disruption where possible.
 */
#define CACHE_VERSION 1

#define DRV_KEY_CPY(_dst, _src, _src_size) \
do {                                       \
   memcpy(_dst, _src, _src_size);          \
   _dst += _src_size;                      \
} while (0);

struct disk_cache *
disk_cache_create(const char *gpu_name, const char *driver_id,
                  uint64_t driver_flags)
{
   void *local;
   struct disk_cache *cache = NULL;
   char *max_size_str;
   uint64_t max_size;

   uint8_t cache_version = CACHE_VERSION;
   size_t cv_size = sizeof(cache_version);

   if (!disk_cache_enabled())
      return NULL;

   /* A ralloc context for transient data during this invocation. */
   local = ralloc_context(NULL);
   if (local == NULL)
      goto fail;

   cache = rzalloc(NULL, struct disk_cache);
   if (cache == NULL)
      goto fail;

   /* Assume failure. */
   cache->path_init_failed = true;

#ifdef ANDROID
   /* Android needs the "disk cache" to be enabled for
    * EGL_ANDROID_blob_cache's callbacks to be called, but it doesn't actually
    * want any storing to disk to happen inside of the driver.
    */
   goto path_fail;
#endif

   char *path = disk_cache_generate_cache_dir(local, gpu_name, driver_id);
   if (!path)
      goto path_fail;

   cache->path = ralloc_strdup(cache, path);
   if (cache->path == NULL)
      goto path_fail;

   if (env_var_as_boolean("MESA_DISK_CACHE_SINGLE_FILE", false)) {
      if (!disk_cache_load_cache_index(local, cache))
         goto path_fail;
   }

   if (!disk_cache_mmap_cache_index(local, cache, path))
      goto path_fail;

   max_size = 0;

   max_size_str = getenv("MESA_GLSL_CACHE_MAX_SIZE");

   #ifdef MESA_GLSL_CACHE_MAX_SIZE
   if( !max_size_str ) {
      max_size_str = MESA_GLSL_CACHE_MAX_SIZE;
   }
   #endif

   if (max_size_str) {
      char *end;
      max_size = strtoul(max_size_str, &end, 10);
      if (end == max_size_str) {
         max_size = 0;
      } else {
         switch (*end) {
         case 'K':
         case 'k':
            max_size *= 1024;
            break;
         case 'M':
         case 'm':
            max_size *= 1024*1024;
            break;
         case '\0':
         case 'G':
         case 'g':
         default:
            max_size *= 1024*1024*1024;
            break;
         }
      }
   }

   /* Default to 1GB for maximum cache size. */
   if (max_size == 0) {
      max_size = 1024*1024*1024;
   }

   cache->max_size = max_size;

   /* 4 threads were chosen below because just about all modern CPUs currently
    * available that run Mesa have *at least* 4 cores. For these CPUs allowing
    * more threads can result in the queue being processed faster, thus
    * avoiding excessive memory use due to a backlog of cache entrys building
    * up in the queue. Since we set the UTIL_QUEUE_INIT_USE_MINIMUM_PRIORITY
    * flag this should have little negative impact on low core systems.
    *
    * The queue will resize automatically when it's full, so adding new jobs
    * doesn't stall.
    */
   if (!util_queue_init(&cache->cache_queue, "disk$", 32, 4,
                        UTIL_QUEUE_INIT_RESIZE_IF_FULL |
                        UTIL_QUEUE_INIT_USE_MINIMUM_PRIORITY |
                        UTIL_QUEUE_INIT_SET_FULL_THREAD_AFFINITY))
      goto fail;

   cache->path_init_failed = false;

 path_fail:

   cache->driver_keys_blob_size = cv_size;

   /* Create driver id keys */
   size_t id_size = strlen(driver_id) + 1;
   size_t gpu_name_size = strlen(gpu_name) + 1;
   cache->driver_keys_blob_size += id_size;
   cache->driver_keys_blob_size += gpu_name_size;

   /* We sometimes store entire structs that contains a pointers in the cache,
    * use pointer size as a key to avoid hard to debug issues.
    */
   uint8_t ptr_size = sizeof(void *);
   size_t ptr_size_size = sizeof(ptr_size);
   cache->driver_keys_blob_size += ptr_size_size;

   size_t driver_flags_size = sizeof(driver_flags);
   cache->driver_keys_blob_size += driver_flags_size;

   cache->driver_keys_blob =
      ralloc_size(cache, cache->driver_keys_blob_size);
   if (!cache->driver_keys_blob)
      goto fail;

   uint8_t *drv_key_blob = cache->driver_keys_blob;
   DRV_KEY_CPY(drv_key_blob, &cache_version, cv_size)
   DRV_KEY_CPY(drv_key_blob, driver_id, id_size)
   DRV_KEY_CPY(drv_key_blob, gpu_name, gpu_name_size)
   DRV_KEY_CPY(drv_key_blob, &ptr_size, ptr_size_size)
   DRV_KEY_CPY(drv_key_blob, &driver_flags, driver_flags_size)

   /* Seed our rand function */
   s_rand_xorshift128plus(cache->seed_xorshift128plus, true);

   ralloc_free(local);

   return cache;

 fail:
   if (cache)
      ralloc_free(cache);
   ralloc_free(local);

   return NULL;
}

void
disk_cache_destroy(struct disk_cache *cache)
{
   if (cache && !cache->path_init_failed) {
      util_queue_finish(&cache->cache_queue);
      util_queue_destroy(&cache->cache_queue);

      if (env_var_as_boolean("MESA_DISK_CACHE_SINGLE_FILE", false))
         foz_destroy(&cache->foz_db);

      disk_cache_destroy_mmap(cache);
   }

   ralloc_free(cache);
}

void
disk_cache_wait_for_idle(struct disk_cache *cache)
{
   util_queue_finish(&cache->cache_queue);
}

void
disk_cache_remove(struct disk_cache *cache, const cache_key key)
{
   char *filename = disk_cache_get_cache_filename(cache, key);
   if (filename == NULL) {
      return;
   }

   disk_cache_evict_item(cache, filename);
}

static struct disk_cache_put_job *
create_put_job(struct disk_cache *cache, const cache_key key,
               const void *data, size_t size,
               struct cache_item_metadata *cache_item_metadata)
{
   struct disk_cache_put_job *dc_job = (struct disk_cache_put_job *)
      malloc(sizeof(struct disk_cache_put_job) + size);

   if (dc_job) {
      dc_job->cache = cache;
      memcpy(dc_job->key, key, sizeof(cache_key));
      dc_job->data = dc_job + 1;
      memcpy(dc_job->data, data, size);
      dc_job->size = size;

      /* Copy the cache item metadata */
      if (cache_item_metadata) {
         dc_job->cache_item_metadata.type = cache_item_metadata->type;
         if (cache_item_metadata->type == CACHE_ITEM_TYPE_GLSL) {
            dc_job->cache_item_metadata.num_keys =
               cache_item_metadata->num_keys;
            dc_job->cache_item_metadata.keys = (cache_key *)
               malloc(cache_item_metadata->num_keys * sizeof(cache_key));

            if (!dc_job->cache_item_metadata.keys)
               goto fail;

            memcpy(dc_job->cache_item_metadata.keys,
                   cache_item_metadata->keys,
                   sizeof(cache_key) * cache_item_metadata->num_keys);
         }
      } else {
         dc_job->cache_item_metadata.type = CACHE_ITEM_TYPE_UNKNOWN;
         dc_job->cache_item_metadata.keys = NULL;
      }
   }

   return dc_job;

fail:
   free(dc_job);

   return NULL;
}

static void
destroy_put_job(void *job, int thread_index)
{
   if (job) {
      struct disk_cache_put_job *dc_job = (struct disk_cache_put_job *) job;
      free(dc_job->cache_item_metadata.keys);

      free(job);
   }
}

static void
cache_put(void *job, int thread_index)
{
   assert(job);

   unsigned i = 0;
   char *filename = NULL;
   struct disk_cache_put_job *dc_job = (struct disk_cache_put_job *) job;

   if (env_var_as_boolean("MESA_DISK_CACHE_SINGLE_FILE", false)) {
      disk_cache_write_item_to_disk_foz(dc_job);
   } else {
      filename = disk_cache_get_cache_filename(dc_job->cache, dc_job->key);
      if (filename == NULL)
         goto done;

      /* If the cache is too large, evict something else first. */
      while (*dc_job->cache->size + dc_job->size > dc_job->cache->max_size &&
             i < 8) {
         disk_cache_evict_lru_item(dc_job->cache);
         i++;
      }

      disk_cache_write_item_to_disk(dc_job, filename);

done:
      free(filename);
   }
}

void
disk_cache_put(struct disk_cache *cache, const cache_key key,
               const void *data, size_t size,
               struct cache_item_metadata *cache_item_metadata)
{
   if (cache->blob_put_cb) {
      cache->blob_put_cb(key, CACHE_KEY_SIZE, data, size);
      return;
   }

   if (cache->path_init_failed)
      return;

   struct disk_cache_put_job *dc_job =
      create_put_job(cache, key, data, size, cache_item_metadata);

   if (dc_job) {
      util_queue_fence_init(&dc_job->fence);
      util_queue_add_job(&cache->cache_queue, dc_job, &dc_job->fence,
                         cache_put, destroy_put_job, dc_job->size);
   }
}

uint8_t *
disk_cache_get(struct disk_cache *cache, const cache_key key, size_t *size)
{
   if (size)
      *size = 0;

   if (cache->blob_get_cb) {
      /* This is what Android EGL defines as the maxValueSize in egl_cache_t
       * class implementation.
       */
      const signed long max_blob_size = 64 * 1024;
      void *blob = malloc(max_blob_size);
      if (!blob)
         return NULL;

      signed long bytes =
         cache->blob_get_cb(key, CACHE_KEY_SIZE, blob, max_blob_size);

      if (!bytes) {
         free(blob);
         return NULL;
      }

      if (size)
         *size = bytes;
      return blob;
   }

   if (env_var_as_boolean("MESA_DISK_CACHE_SINGLE_FILE", false)) {
      return disk_cache_load_item_foz(cache, key, size);
   } else {
      char *filename = disk_cache_get_cache_filename(cache, key);
      if (filename == NULL)
         return NULL;

      return disk_cache_load_item(cache, filename, size);
   }
}

void
disk_cache_put_key(struct disk_cache *cache, const cache_key key)
{
   const uint32_t *key_chunk = (const uint32_t *) key;
   int i = CPU_TO_LE32(*key_chunk) & CACHE_INDEX_KEY_MASK;
   unsigned char *entry;

   if (cache->blob_put_cb) {
      cache->blob_put_cb(key, CACHE_KEY_SIZE, key_chunk, sizeof(uint32_t));
      return;
   }

   if (cache->path_init_failed)
      return;

   entry = &cache->stored_keys[i * CACHE_KEY_SIZE];

   memcpy(entry, key, CACHE_KEY_SIZE);
}

/* This function lets us test whether a given key was previously
 * stored in the cache with disk_cache_put_key(). The implement is
 * efficient by not using syscalls or hitting the disk. It's not
 * race-free, but the races are benign. If we race with someone else
 * calling disk_cache_put_key, then that's just an extra cache miss and an
 * extra recompile.
 */
bool
disk_cache_has_key(struct disk_cache *cache, const cache_key key)
{
   const uint32_t *key_chunk = (const uint32_t *) key;
   int i = CPU_TO_LE32(*key_chunk) & CACHE_INDEX_KEY_MASK;
   unsigned char *entry;

   if (cache->blob_get_cb) {
      uint32_t blob;
      return cache->blob_get_cb(key, CACHE_KEY_SIZE, &blob, sizeof(uint32_t));
   }

   if (cache->path_init_failed)
      return false;

   entry = &cache->stored_keys[i * CACHE_KEY_SIZE];

   return memcmp(entry, key, CACHE_KEY_SIZE) == 0;
}

void
disk_cache_compute_key(struct disk_cache *cache, const void *data, size_t size,
                       cache_key key)
{
   struct mesa_sha1 ctx;

   _mesa_sha1_init(&ctx);
   _mesa_sha1_update(&ctx, cache->driver_keys_blob,
                     cache->driver_keys_blob_size);
   _mesa_sha1_update(&ctx, data, size);
   _mesa_sha1_final(&ctx, key);
}

void
disk_cache_set_callbacks(struct disk_cache *cache, disk_cache_put_cb put,
                         disk_cache_get_cb get)
{
   cache->blob_put_cb = put;
   cache->blob_get_cb = get;
}

