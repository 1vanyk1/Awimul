#include "impl/sys/shm.h"
#include "main_wm.h"
#include "main_wm_c.h"
#include <jni.h>
#include <android/native_window_jni.h>
#include <cstdlib>
#include <thread>
#include <string>
#include "window_manager.h"
#include "x11/headers/dix.h"
//#include "x11/headers/xutil.h"
#include "GL/glx.h"
#include "GL/gl.h"
#include "GL/glxext.h"
#include "x11/headers/extensions/XShm.h"
#include "x11/headers/xlibint.h"
#include "x11/headers/extensions/XTest.h"
#include "x11/headers/extensions/XInput2.h"
#include "x11/headers/inpututils.h"
//#include "x11/headers/xlibint.h"

#include <time.h>
#define FPS(start) (CLOCKS_PER_SEC / (clock()-start))
#define MAX_FPS 30

typedef CARD32 FbBits;
typedef int FbStride;

extern "C" {
int main_stubmain(int, char**, char**);

int main_xclock(int, char**);

int main_xeyes(int, char**);

int main_xcalc(int, char**);
};

char *tmp_loc = "/data/data/com.vantacom.aarm/files/tmp";

ANativeWindow *nativeWindow = nullptr;

FbBits *NativeFbBits = NULL;

int gotDrawable = 0;

int isOnXserver = 0;

int isPausedXserver = 0;

jclass classWM;

jobject java_WMObj;

jstring java_tmp_loc;

JavaVM* java_VM;

pthread_t thread_WM;

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

void loadEnd(JNIEnv* env)
{
    java_VM->AttachCurrentThread(&env, NULL);
    jmethodID loadEnd = env->GetMethodID(classWM, "loadEnd", "()V");
    env->CallVoidMethod(java_WMObj, loadEnd);
    java_VM->DetachCurrentThread();
}


void move(Display *display, int x, int y)
{
    for(int i = 0; i < 100; i++) {
        move_mouse(x + i, y + i);
    }
}


jint start_xapp(jint app) {
    switch (app) {
        default:
            break;
        case 0:
        {
            sleep(1);
            int argc = 3;
            char *argv[] {"main_xclock", "-update", "1"};

            main_xclock(argc, argv);
            break;
        }
        case 1:
        {
            sleep(2);
            int argc = 1;
            char *argv[] {"main_xeye"};

            main_xeyes(argc, argv);
            break;
        }
        case 2:
        {
            sleep(3);
            int argc = 1;
            char *argv[] {"main_xcalc"};

            main_xcalc(argc, argv);
            break;
        }
    }
    return EXIT_SUCCESS;
}


JNIEXPORT void set_surface(JNIEnv* env, jobject thiz, jobject surface) {
    ANativeWindow *window = ANativeWindow_fromSurface(env, surface);

    Display *dpy = XOpenDisplay(NULL);
    Window root = DefaultRootWindow(dpy);

    XWindowAttributes gwa;

    XGetWindowAttributes(dpy, root, &gwa);
    long width = gwa.width;
    long height = gwa.height;
    int depth = gwa.depth;

    ALOGE("size (%ld, %ld)", width, height);

    if (depth > 16)
        ANativeWindow_setBuffersGeometry(window, width, height, WINDOW_FORMAT_RGBA_8888);
    else
        ANativeWindow_setBuffersGeometry(window, width, height, WINDOW_FORMAT_RGB_565);

    nativeWindow = window;
}

void shut_down(JNIEnv* env, jobject thiz) {
//    Display *dpy = XOpenDisplay(NULL);
//    Window root = DefaultRootWindow(dpy);
//    Window parent;
//    Window w = None;
//    Window *children;
//    unsigned int nchildren;
//    Status s = XQueryTree(dpy, root, &w, &parent, &children, &nchildren);
//    if (s)
//        XFree(children);
//    else {
//        while (nchildren--) {
//            XKillClient(dpy, children[nchildren]);
//        }
//    }
//    XKillClient(dpy, root);
    isOnXserver = False;
    nativeWindow = nullptr;
    std::terminate();  //TODO: better XServer closing (it closes, but this way looks strange);
}

void pause_server(JNIEnv* env, jobject thiz, jboolean b) {
    if (b) {
        isPausedXserver = True;
    } else {
        isPausedXserver = False;
    }
}


void mouse_event(JNIEnv* env, jobject thiz, jint action, jint x, jint y, jint button) {
    move_mouse(x, y);
    switch (action) {
        default:
        case MOUSE_ACTION_MOVE:
            break;
        case MOUSE_ACTION_DOWN:
        case MOUSE_ACTION_UP:
            mouse_button_action(x, y, button, action);
            break;
    }
}


void key_press(JNIEnv* env, jobject thiz, jint key, jint is_pressed) {
    key_press_c(key, is_pressed);
}


void render_image() {
    Display *dpy = XOpenDisplay(NULL);
    Window root = DefaultRootWindow(dpy);

    XWindowAttributes gwa;

    XGetWindowAttributes(dpy, root, &gwa);
    long width = gwa.width;
    long height = gwa.height;
    int depth = gwa.depth;

    double start = clock();

    while (isOnXserver) {
        if (gotDrawable && !isPausedXserver && FPS(start) < MAX_FPS && nativeWindow != nullptr && NativeFbBits != NULL) {
            start = clock();
            LockDisplay(dpy);
            ANativeWindow_Buffer surface_buffer;
            if (ANativeWindow_lock(nativeWindow, &surface_buffer, NULL) == 0) {
                if (depth > 16) {
                    memcpy_bitmap_4(surface_buffer, NativeFbBits, height * width);
                }
                else {
                    memcpy_bitmap_2(surface_buffer, NativeFbBits, height * width);
                }
//                memcpy(surface_buffer.bits, NativeFbBits, height * width * 4);
                ANativeWindow_unlockAndPost(nativeWindow);
            }
//            ALOGE("fps %4.f  spf %.4f\n", FPS(start), 1 / FPS(start));
            if (isOnXserver) {
                UnlockDisplay(dpy);
                SyncHandle();
            }
        }
    }
}


jint start_wm(JNIEnv* env) {
    java_VM->AttachCurrentThread(&env, NULL);
    tmp_loc = const_cast<char *>(ConvertJString(env, java_tmp_loc));

    unique_ptr<WindowManager> window_manager = WindowManager::Create();
    if (!window_manager) {
        ALOGE("Failed to initialize window manager.");
        java_VM->DetachCurrentThread();
        shut_down(NULL, NULL);
        return EXIT_FAILURE;
    }

    get_mouse_device();
    get_keyboard_device();

    std::thread end_thread(loadEnd, env);
    end_thread.join();

    std::thread t1(start_xapp, 0);

    std::thread t2(start_xapp, 1);

    std::thread t3(start_xapp, 2);

    std::thread render_thread(render_image);

    window_manager->Run();
    ALOGE("EXIT_SUCCESS");
    java_VM->DetachCurrentThread();
    return EXIT_SUCCESS;
}



jint start_xserver(JNIEnv* env, jobject thiz, jstring tmp, jint width, jint height) {
    java_WMObj = env->NewGlobalRef(thiz);
    java_tmp_loc = (jstring)(env->NewGlobalRef(tmp));
    tmp_loc = const_cast<char *>(ConvertJString(env, tmp));
    ALOGE("%s", tmp_loc);
    char* display_str;
    display_str = static_cast<char *>(malloc(strlen(":") + strlen(XSERVER_PORT) + 1));
    strcpy(display_str, ":");
    strcat(display_str, XSERVER_PORT);
    char screen_info[16 + sizeof(char)];
    std::sprintf(screen_info, "%dx%dx24", width, height);
    setenv("DISPLAY", display_str,1);
    int argc = 5;
    char *argv[] {"main_stubmain", display_str, "-screen", "0", screen_info};
    char *envp[] {};
    isOnXserver = True;
//    pthread_create(thread_WM, NULL, start_wm, env);
    std::thread t1(start_wm, env);
    main_stubmain(argc, argv, envp);
    t1.detach();
    return EXIT_SUCCESS;
}


typedef union {
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;

static JNINativeMethod methods[] = {
        {"init", "(Ljava/lang/String;II)I", (void*)start_xserver },
        {"setSurface", "(Landroid/view/Surface;)V", (void*)set_surface },
        {"shutDown", "()V", (void*)shut_down },
        {"mouseEvent", "(IIII)V", (void*)mouse_event },
        {"pauseServer", "(Z)V", (void*)pause_server },
        {"keyPress", "(II)V", (void*)key_press },
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
    java_VM = vm;
    UnionJNIEnvToVoid uenv;
    uenv.venv = NULL;
    jint result = -1;
    JNIEnv* env = NULL;

    if (vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) == JNI_OK) {
        env = uenv.env;
        jclass clazz = env->FindClass(classPathName);
        classWM = (jclass)env->NewGlobalRef(clazz);
        if (registerNatives(env) != JNI_TRUE) {
            goto bail;
        }
        result = JNI_VERSION_1_4;
    }
    bail:
    return result;
}