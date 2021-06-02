#include "ipc.h"
#include <jni.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <dlfcn.h>
#include <string>
#include <array>
#include <android/log.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/msg.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sstream>
#include <ctime>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netdb.h>

#define ALOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, LOG_TAG, __VA_ARGS__)
#define ALOGD(...) __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#define ALOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define ALOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define ALOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
#define READ   0
#define WRITE  1

JavaVM *java_vm = NULL;
jobject java_object = NULL;

bool isPathExist(const std::string &s)
{
    struct stat buffer;
    return (stat (s.c_str(), &buffer) == 0);
}

FILE * popen2(std::string command, std::string type, int & pid)
{
    pid_t child_pid;
    int fd[2];
    pipe(fd);

    if((child_pid = fork()) == -1)
    {
        perror("fork");
        exit(1);
    }

    if (child_pid == 0)
    {
        if (type == "r")
        {
            close(fd[READ]);
            dup2(fd[WRITE], 1);
        }
        else
        {
            close(fd[WRITE]);
            dup2(fd[READ], 0);
        }

        setpgid(child_pid, child_pid);
        execl("/bin/sh", "/bin/sh", "-c", command.c_str(), NULL);
        exit(0);
    }
    else
    {
        if (type == "r")
        {
            close(fd[WRITE]);
        }
        else
        {
            close(fd[READ]);
        }
    }

    pid = child_pid;

    if (type == "r")
    {
        return fdopen(fd[READ], "r");
    }

    return fdopen(fd[WRITE], "w");
}

int pclose2(FILE * fp, pid_t pid)
{
    int stat;
    fclose(fp);
    while (waitpid(pid, &stat, 0) == -1)
    {
        if (errno != EINTR)
        {
            stat = -1;
            break;
        }
    }
    return stat;
}

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

void report(const char* msg, int terminate) {
    perror(msg);
    if (terminate) exit(-1);
}

jstring wine_init_jni(JNIEnv *env, jobject obj, jobjectArray cmdline, jobjectArray environment, jstring jpath)
{


//    void *handle;
//    char *error;
//    const char *path = ConvertJString(env, jpath);
//    handle = dlopen (path, RTLD_LAZY);
//    if (!handle) {
//        ALOGE("%s\n", dlerror());
//        return (jstring) "";
//    }
//    void (*start_main_thread)();
//    ALOGE("%s\n", path);
//    ALOGE("%s\n", "123");
//    *(void **)(&start_main_thread) = dlsym(handle, "start_main_thread");
//    if ((error = dlerror()) != NULL)  {
//        ALOGE("%s\n", error);
//        return (jstring) "";
//    }
//    ALOGE("%s\n", "123");
//    start_main_thread();


//    wine_init_jni(*env, &obj, &cmdline, environment);

//    char **argv;
//    char *str;
//    char error[1024];
//    int i, argc, length;
//
//    argc = (*env).GetArrayLength(cmdline);
//    for (i = length = 0; i < argc; i++)
//    {
//        jstring str_obj = static_cast<jstring>((*env).GetObjectArrayElement(cmdline, i));
//        length += (*env).GetStringUTFLength(str_obj) + 1;
//    }
//
//    argv = static_cast<char **>(malloc((argc + 1) * sizeof(*argv) + length));
//    str = (char *)(argv + argc + 1);
//    for (i = 0; i < argc; i++)
//    {
//        jstring str_obj = static_cast<jstring>((*env).GetObjectArrayElement(cmdline, i));
//        length = (*env).GetStringUTFLength(str_obj);
//        (*env).GetStringUTFRegion(str_obj, 0, (*env).GetStringLength(str_obj), str);
//        argv[i] = str;
//        str[length] = 0;
//        str += length + 1;
//    }
//    argv[argc] = NULL;
//
//    if (environment)
//    {
//        int count = (*env).GetArrayLength(environment);
//        for (i = 0; i < count - 1; i += 2)
//        {
//            jstring var_obj = static_cast<jstring>((*env).GetObjectArrayElement(environment, i));
//            jstring val_obj = static_cast<jstring>((*env).GetObjectArrayElement(environment,i + 1));
//            const char *var = (*env).GetStringUTFChars(var_obj, NULL);
//
//            if (val_obj)
//            {
//                const char *val = (*env).GetStringUTFChars(val_obj, NULL);
//                setenv( var, val, 1 );
//                if (!strcmp( var, "LD_LIBRARY_PATH" ))
//                {
//                    void (*update_func)(const char *);
//                    *(void **)(&update_func) = dlsym( RTLD_DEFAULT, "android_update_LD_LIBRARY_PATH" );
//                    if (update_func) update_func(val);
//                }
//                else if (!strcmp( var, "WINEDEBUGLOG" ))
//                {
//                    int fd = open( val, O_WRONLY | O_CREAT | O_APPEND, 0666 );
//                    if (fd != -1)
//                    {
//                        dup2( fd, 2 );
//                        close( fd );
//                    }
//                }
//                (*env).ReleaseStringUTFChars(val_obj, val);
//            }
//            else unsetenv(var);
//
//            (*env).ReleaseStringUTFChars(var_obj, var);
//        }
//    }
//
//    java_object = (*env).NewGlobalRef(obj);
//
//    void *handle;
//    const char *path = ConvertJString(env, jpath);
//    handle = dlopen (path, RTLD_LAZY);
//
//    void (*init_paths)(char **);
//    *(void **)(&init_paths) = dlsym(handle, "init_paths");
//    init_paths(argv);



//
//    void (*virtual_init)();
//    *(void **)(&virtual_init) = dlsym(handle, "virtual_init");
//    virtual_init();
//
//    void (*init_environment)(int, char **, jobjectArray);
//    *(void **)(&init_environment) = dlsym(handle, "init_environment");
//    init_environment(argc, argv, environment);
//
//    void (*start_main_thread)();
//    *(void **)(&start_main_thread) = dlsym(handle, "start_main_thread");
//    start_main_thread();

    return (jstring) "(*env).NewStringUTF(error)";
}

void init(JNIEnv* env, jobject thiz, jstring jpath)
{
    int pid;
    const char *path = ConvertJString(env, jpath);
    std::array<char, 128> buffer;
    std::string res;
    std::string command("wineserver -f -p");
    FILE *proc_wineserver = popen2(command, "r", pid);
    if (!proc_wineserver)
    {
        ALOGE("%s\n", "Couldn't start command.");
        return;
    }
    ALOGE("[wine session] ... started with PID %d ...", pid);

    struct stat fileInfo;
    if (stat(path, &fileInfo) != 0) {
        ALOGE("%s\n", "Error");
        return;
    }
    std::ostringstream stringStream;
    stringStream << path;
    stringStream << "/server-";
    stringStream << std::hex << fileInfo.st_dev;
    stringStream << "-";
    stringStream << std::hex << fileInfo.st_ino;
    stringStream << "/socket";
    std::string socket_path = stringStream.str();
    ALOGE("[wine session] ... expecting socket at %s ...", socket_path.c_str());

    struct addrinfo hints;
    struct addrinfo *result, *rp;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNIX;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    hints.ai_protocol = 0;
    hints.ai_canonname = NULL;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;
    getaddrinfo(NULL, NULL, &hints, &result);

    int wineserver_client;
//    wineserver_client.settimeout(1.0);
    bool got_connection = false;
//    float wait_for_seconds = 0.01;
//    float timeout_after_seconds = 30.0;
//    std::time_t started_waiting_at = std::time(0);
//    int tried_this_many_times = 0;
    while (!got_connection) {
        ALOGE("%s\n", "1");
        for (rp = result; rp != NULL; rp = rp->ai_next) {
            wineserver_client = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
            ALOGE("%s\n", "LOOK");
            if (wineserver_client == -1) {
                continue;
            }
            if (connect(wineserver_client, rp->ai_addr, rp->ai_addrlen) == 0) {
                ALOGE("%s\n", "1");
                got_connection = true;
                break;
            }
            close(wineserver_client);
        }
        sleep(1);
    }

//    while (true) {
//        if (isPathExist(socket_path)) {
//            tried_this_many_times++;
//            try {
//
//                got_connection = true;
//                close(wineserver_client);
//                break;
//            } catch (const std::exception&) {}
//        }
//    }
    ALOGE("%d\n", got_connection);

    while (fgets(buffer.data(), 128, proc_wineserver) != NULL) {
        res += buffer.data();
    }
    int returnCode = pclose2(proc_wineserver, pid);

    ALOGE("%s\n", res.c_str());
    ALOGE("%d\n", returnCode);


//    void *handle;
//    char *error;
//    const char *path = ConvertJString(env, jpath);
//    handle = dlopen (path, RTLD_LAZY);
//    if (!handle) {
//        ALOGE("%s\n", dlerror());
//        return;
//    }
//    dlerror();
//    unsigned int (*fptr)();
//    *(void **)(&fptr) = dlsym(handle, "GetTickCount");
//    if ((error = dlerror()) != NULL)  {
//        ALOGE("%s\n", error);
//        return;
//    }
//    ALOGE("%d\n", (*fptr)());
//    dlclose(handle);
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
        {"wine_init", "([Ljava/lang/String;[Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;", (void*)wine_init_jni},
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
    java_vm = vm;

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