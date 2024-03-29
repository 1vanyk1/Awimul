#ifndef OS_THREAD_H_
#define OS_THREAD_H_


#include "../include/pipe/p_compiler.h"
#include "../util/u_debug.h" /* for assert */
#include "../util/u_thread.h"


#define pipe_mutex_assert_locked(mutex) \
   __pipe_mutex_assert_locked(&(mutex))

static inline void
__pipe_mutex_assert_locked(mtx_t *mutex)
{
#ifdef DEBUG
    /* NOTE: this would not work for recursive mutexes, but
    * mtx_t doesn't support those
    */
   int ret = mtx_trylock(mutex);
   assert(ret == thrd_busy);
   if (ret == thrd_success)
      mtx_unlock(mutex);
#else
    (void)mutex;
#endif
}


/*
 * Semaphores
 */

typedef struct
{
    mtx_t mutex;
    cnd_t cond;
    int counter;
} pipe_semaphore;


static inline void
pipe_semaphore_init(pipe_semaphore *sema, int init_val)
{
    (void) mtx_init(&sema->mutex, mtx_plain);
    cnd_init(&sema->cond);
    sema->counter = init_val;
}

static inline void
pipe_semaphore_destroy(pipe_semaphore *sema)
{
    mtx_destroy(&sema->mutex);
    cnd_destroy(&sema->cond);
}

/** Signal/increment semaphore counter */
static inline void
pipe_semaphore_signal(pipe_semaphore *sema)
{
    mtx_lock(&sema->mutex);
    sema->counter++;
    cnd_signal(&sema->cond);
    mtx_unlock(&sema->mutex);
}

/** Wait for semaphore counter to be greater than zero */
static inline void
pipe_semaphore_wait(pipe_semaphore *sema)
{
    mtx_lock(&sema->mutex);
    while (sema->counter <= 0) {
        cnd_wait(&sema->cond, &sema->mutex);
    }
    sema->counter--;
    mtx_unlock(&sema->mutex);
}



/*
 * Thread-specific data.
 */

typedef struct {
    tss_t key;
    int initMagic;
} pipe_tsd;


#define PIPE_TSD_INIT_MAGIC 0xff8adc98


static inline void
pipe_tsd_init(pipe_tsd *tsd)
{
    if (tss_create(&tsd->key, NULL/*free*/) != 0) {
        exit(-1);
    }
    tsd->initMagic = PIPE_TSD_INIT_MAGIC;
}

static inline void *
pipe_tsd_get(pipe_tsd *tsd)
{
    if (tsd->initMagic != (int) PIPE_TSD_INIT_MAGIC) {
        pipe_tsd_init(tsd);
    }
    return tss_get(tsd->key);
}

static inline void
pipe_tsd_set(pipe_tsd *tsd, void *value)
{
    if (tsd->initMagic != (int) PIPE_TSD_INIT_MAGIC) {
        pipe_tsd_init(tsd);
    }
    if (tss_set(tsd->key, value) != 0) {
        exit(-1);
    }
}

#endif /* OS_THREAD_H_ */
