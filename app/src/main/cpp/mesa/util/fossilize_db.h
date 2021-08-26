#ifndef FOSSILIZE_DB_H
#define FOSSILIZE_DB_H

#ifdef HAVE_FLOCK
#define FOZ_DB_UTIL 1
#endif

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "simple_mtx.h"

/* Max number of DBs our implementation can read from at once */
#define FOZ_MAX_DBS 9 /* Default DB + 8 Read only DBs */

#define FOSSILIZE_BLOB_HASH_LENGTH 40

enum {
    FOSSILIZE_COMPRESSION_NONE = 1,
    FOSSILIZE_COMPRESSION_DEFLATE = 2
};

enum {
    FOSSILIZE_FORMAT_VERSION = 6,
    FOSSILIZE_FORMAT_MIN_COMPAT_VERSION = 5
};

struct foz_payload_header {
    uint32_t payload_size;
    uint32_t format;
    uint32_t crc;
    uint32_t uncompressed_size;
};

struct foz_db_entry {
    uint8_t file_idx;
    uint8_t key[20];
    uint64_t offset;
    struct foz_payload_header header;
};

struct foz_db {
    FILE *file[FOZ_MAX_DBS];          /* An array of all foz dbs */
    FILE *db_idx;                     /* The default writable foz db idx */
    simple_mtx_t mtx;                 /* Mutex for file/hash table read/writes */
    void *mem_ctx;
    struct hash_table_u64 *index_db;  /* Hash table of all foz db entries */
    bool alive;
};

bool
foz_prepare(struct foz_db *foz_db, char *cache_path);

void
foz_destroy(struct foz_db *foz_db);

void *
foz_read_entry(struct foz_db *foz_db, const uint8_t *cache_key_160bit,
               size_t *size);

bool
foz_write_entry(struct foz_db *foz_db, const uint8_t *cache_key_160bit,
                const void *blob, size_t size);

#endif /* FOSSILIZE_DB_H */
