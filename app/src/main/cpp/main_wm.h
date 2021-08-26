#ifndef AWIMUL_MAIN_WM_H
#define AWIMUL_MAIN_WM_H
#define LOG_TAG "WM"

#define DEBUG

#define XSERVER_PORT "0"
#define AWIMUL_FILES_PATH "/data/data/com.vantacom.aarm/files"
#include <android/log.h>

#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

#endif //AWIMUL_MAIN_WM_H
