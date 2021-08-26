#include "main_wm.h"
#include <jni.h>
#include <cstdlib>
#include <string>
#include "window_manager.h"
#include "x11/headers/dix.h"
#include "impl/sys/shm.h"

extern "C" {
int main_stubmain(int, char**, char**);
};

char *xserver_loc = "/tmp/.X11-unix/X0";

char *tmp_loc = "/data/data/com.vantacom.aarm/files/tmp";

static const char *classPathName = "com/vantacom/aarm/xserver/WM";

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

jboolean checkXServerIsLoaded(JNIEnv* env, jobject thiz, jstring tmp) {
    char* display_str;
    tmp_loc = const_cast<char *>(ConvertJString(env, tmp));
    display_str = static_cast<char *>(malloc(strlen(":") + strlen(XSERVER_PORT) + 1));
    strcpy(display_str, ":");
    strcat(display_str, XSERVER_PORT);
    const char* display_c_str = display_str;
    Display* display = XOpenDisplay(display_c_str);
    if (display == nullptr) {
        ALOGE("Failed to open X display %s", XDisplayName(display_c_str));
        return 0;
    }
    ALOGE("Opened X display %s", XDisplayName(display_c_str));
    XCloseDisplay(display);
    return 1;
}

jint start_xserver(JNIEnv* env, jobject thiz, jstring jstring1, jstring tmp) {
    tmp_loc = const_cast<char *>(ConvertJString(env, tmp));
    ALOGE("%s", tmp_loc);
    char* display_str;
    display_str = static_cast<char *>(malloc(strlen(":") + strlen(XSERVER_PORT) + 1));
    strcpy(display_str, ":");
    strcat(display_str, XSERVER_PORT);
    setenv("DISPLAY", display_str,1);
    char *argv[] {"main_stubmain", display_str, "-screen", "0", "800x600x16"};
    char *envp[] {};
    ALOGE("1");
    main_stubmain(5, argv, envp);
    ALOGE("1");
    xserver_loc = const_cast<char *>(ConvertJString(env, jstring1));

//    unique_ptr<WindowManager> window_manager = WindowManager::Create();
//    if (!window_manager) {
//        ALOGE("Failed to initialize window manager.");
//        return EXIT_FAILURE;
//    }
//    ALOGE("EXIT_SUCCESS");
//    window_manager->Run();

    return EXIT_SUCCESS;
}


jint start_wm(JNIEnv* env, jobject thiz, jstring jstring1) {
    xserver_loc = const_cast<char *>(ConvertJString(env, jstring1));

    unique_ptr<WindowManager> window_manager = WindowManager::Create();
    if (!window_manager) {
        ALOGE("Failed to initialize window manager.");
        return EXIT_FAILURE;
    }
    window_manager->Run();
    ALOGE("EXIT_SUCCESS");
    return EXIT_SUCCESS;
}


typedef union {
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;

static JNINativeMethod methods[] = {
        {"init", "(Ljava/lang/String;Ljava/lang/String;)I", (void*)start_xserver },
        {"checkXServerIsLoaded", "(Ljava/lang/String;)Z", (void*)checkXServerIsLoaded },
        {"startWM", "(Ljava/lang/String;)I", (void*)start_wm },
};

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