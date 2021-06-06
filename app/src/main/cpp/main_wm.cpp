#include "main_wm.h"
#include <jni.h>
#include <cstdlib>
#include <string>
#include "window_manager.h"

#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

char *xserver_loc = "/tmp/.X11-unix/X";

using ::std::unique_ptr;

const char* ConvertJString(JNIEnv* env, jstring str)
{
    if ( !str ) std::string();

    const jsize len = env->GetStringUTFLength(str);
    const char* strChars = env->GetStringUTFChars(str, (jboolean *)0);

    std::string Result(strChars, len);

    env->ReleaseStringUTFChars(str, strChars);

    return strChars;
}

jint start_wm(JNIEnv* env, jobject thiz, jstring jstring) {

    xserver_loc = const_cast<char *>(ConvertJString(env, jstring));
    char* display_str;
    display_str = static_cast<char *>(malloc(strlen(":") + strlen(XSERVER_PORT) + 1));
    strcpy(display_str, ":");
    strcat(display_str, XSERVER_PORT);

    setenv("DISPLAY", display_str,1);
    unique_ptr<WindowManager> window_manager = WindowManager::Create();
    if (!window_manager) {
        ALOGE("Failed to initialize window manager.");
        return EXIT_FAILURE;
    }
    ALOGE("EXIT_SUCCESS");
    window_manager->Run();

    return EXIT_SUCCESS;
}

typedef union {
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;

static JNINativeMethod methods[] = {
        {"init", "(Ljava/lang/String;)I", (void*)start_wm },
};

static const char *classPathName = "com/vantacom/aarm/xserver/WM";

static int registerNativeMethods(JNIEnv* env, const char* className, JNINativeMethod* gMethods, int numMethods)
{
    jclass clazz;
    clazz = env->FindClass(className);
    if (clazz == NULL) {
        return JNI_FALSE;
    }
    if (env->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static int registerNatives(JNIEnv* env)
{
    if (!registerNativeMethods(env, classPathName, methods, sizeof(methods) / sizeof(methods[0]))) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

jint JNI_OnLoad(JavaVM* vm, void* reserved)
{
    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    jint result = -1;
    JNIEnv* env = NULL;

    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK) {
        goto bail;
    }
    env = uenv.env;
    if (registerNatives(env) != JNI_TRUE) {
        goto bail;
    }

    result = JNI_VERSION_1_4;

    bail:
    return result;
}