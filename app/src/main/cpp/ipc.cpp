#include "ipc.h"
#include <jni.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <dlfcn.h>
#include <string>
#include <android/log.h>

#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

const char* ConvertJString(JNIEnv* env, jstring str)
{
    if ( !str ) std::string();

    const jsize len = env->GetStringUTFLength(str);
    const char* strChars = env->GetStringUTFChars(str, (jboolean *)0);

    std::string Result(strChars, len);

    env->ReleaseStringUTFChars(str, strChars);

    return strChars;
}

jstring string(JNIEnv* env, jobject thiz)
{
    return env->NewStringUTF("Hello from JNI LIBS!");
}


void init(JNIEnv* env, jobject thiz, jstring jpath)
{
    void *handle;
    void *cosine;
    char *error;
    const char *path = ConvertJString(env, jpath);
    handle = dlopen (path, RTLD_LAZY);
    if (!handle) {
        ALOGE("%s\n", dlerror());
        return;
    }
    dlerror();
    unsigned int (*fptr)();
    *(void **)(&fptr) = dlsym(handle, "GetTickCount");
    if ((error = dlerror()) != NULL)  {
        ALOGE("%s\n", error);
        return;
    }
    ALOGE("%d\n", (*fptr)());
    dlclose(handle);
}


void sendSignal(JNIEnv* env, jobject thiz, jint pid)
{
    int ret;
//    ret = kill(pid, SIGKILL);
    ALOGE("ret : %d", pid);
    ALOGE("ret : %d", ret);
}


static JNINativeMethod methods[] = {
        {"string", "()Ljava/lang/String;", (void*)string },
        {"sendSignal", "(I)V", (void*)sendSignal },
        {"init", "(Ljava/lang/String;)V", (void*)init },
};

static const char *classPathName = "com/vantacom/aarm/ipc/IPClib";

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

typedef union {
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;

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