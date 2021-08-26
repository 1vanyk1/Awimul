#ifndef _OS_TIME_H_
#define _OS_TIME_H_

#include <stdbool.h>
#include <stdint.h>
#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

/* must be equal to PIPE_TIMEOUT_INFINITE */
#define OS_TIMEOUT_INFINITE 0xffffffffffffffffull

/*
 * Get the current time in nanoseconds from an unknown base.
 */
int64_t
os_time_get_nano(void);


/*
 * Get the current time in microseconds from an unknown base.
 */
static inline int64_t
os_time_get(void)
{
    return os_time_get_nano() / 1000;
}


static inline struct tm *
os_localtime(const time_t *timer, struct tm *buf)
{
#ifdef _WIN32
    return localtime_s(buf, timer) ? NULL : buf;
#else
    return localtime_r(timer, buf);
#endif
}


/*
 * Sleep.
 */
void
os_time_sleep(int64_t usecs);


/*
 * Helper function for detecting time outs, taking in account overflow.
 *
 * Returns true if the current time has elapsed beyond the specified interval.
 */
static inline bool
os_time_timeout(int64_t start,
                int64_t end,
                int64_t curr)
{
    if (start <= end)
        return !(start <= curr && curr < end);
    else
        return !((start <= curr) || (curr < end));
}


/**
 * Convert a relative timeout in nanoseconds into an absolute timeout,
 * in other words, it returns current time + timeout.
 * os_time_get_nano() must be monotonic.
 * OS_TIMEOUT_INFINITE is passed through unchanged. If the calculation
 * overflows, OS_TIMEOUT_INFINITE is returned.
 */
int64_t
os_time_get_absolute_timeout(uint64_t timeout);


/**
 * Wait until the variable at the given memory location is zero.
 *
 * \param var           variable
 * \param timeout       timeout in ns, can be anything from 0 (no wait) to
 *                      OS_TIMEOUT_INFINITE (wait forever)
 * \return     true if the variable is zero
 */
bool
os_wait_until_zero(volatile int *var, uint64_t timeout);


/**
 * Wait until the variable at the given memory location is zero.
 * The timeout is the absolute time when the waiting should stop. If it is
 * less than or equal to the current time, it only returns the status and
 * doesn't wait. OS_TIMEOUT_INFINITE waits forever. This requires that
 * os_time_get_nano is monotonic.
 *
 * \param var       variable
 * \param timeout   the time in ns when the waiting should stop
 * \return     true if the variable is zero
 */
bool
os_wait_until_zero_abs_timeout(volatile int *var, int64_t timeout);

#ifdef __cplusplus
}
#endif

#endif /* _OS_TIME_H_ */
