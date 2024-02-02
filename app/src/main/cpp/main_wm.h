#ifndef AWIMUL_MAIN_WM_H
#define AWIMUL_MAIN_WM_H
#define LOG_TAG "WM"

#define DEBUG

#define XSERVER_PORT "0"
#define AWIMUL_FILES_PATH "/data/data/com.vantacom.aarm/files"
#include <android/log.h>
#include <jni.h>

#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)


#define memcpy_bitmap_4(dest_buf, src, len) \
    long *d = (long *)dest_buf.bits; \
    long const *s = (long const *)src; \
    size_t l = dest_buf.height; \
    size_t l2; \
    if (dest_buf.width == dest_buf.stride) { \
        while (l--) { \
            l2 = dest_buf.width; \
            while (l2--) \
                *d++ = *s++; \
        } \
    } else { \
        size_t delta = dest_buf.stride - dest_buf.width; \
        while (l--) { \
            l2 = dest_buf.width; \
            while (l2--) \
                *d++ = *s++; \
            d += delta; \
        } \
    }


#define memcpy_bitmap_2(dest_buf, src, len) \
    char *d = (char *)dest_buf.bits; \
    char const *s = (char const *)src; \
    size_t l = dest_buf.height; \
    size_t l2; \
    if (dest_buf.width == dest_buf.stride) { \
        while (l--) { \
            l2 = dest_buf.width; \
            while (l2--) \
                *d++ = *s++; \
        } \
    } else { \
        size_t delta = dest_buf.stride - dest_buf.width; \
        while (l--) { \
            l2 = dest_buf.width; \
            while (l2--) \
                *d++ = *s++; \
            d += delta; \
        } \
    }


#endif //AWIMUL_MAIN_WM_H
