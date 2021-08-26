#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#endif

#include <stdio.h>
#include "../x11/headers/x.h"
#include "../x11/headers/os.h"
#include "osdep.h"
#include "../x11/headers/opaque.h"
#include "../x11/headers/xos.h"
#include <signal.h>
#include <errno.h>
#ifdef HAVE_DLFCN_H
#include <dlfcn.h>
#endif
#ifdef HAVE_BACKTRACE
#include <execinfo.h>
#endif

#include "../x11/headers/misc.h"

#include "../x11/headers/dixstruct.h"

#if !defined(SYSV) && !defined(WIN32)
#include <sys/resource.h>
#endif

#ifndef ADMPATH
#define ADMPATH "/usr/adm/X%smsgs"
#endif

#ifdef RLIMIT_DATA
int limitDataSpace = -1;
#endif
#ifdef RLIMIT_STACK
int limitStackSpace = -1;
#endif
#ifdef RLIMIT_NOFILE
int limitNoFile = -1;
#endif

/* The actual user defined max number of clients */
int LimitClients = LIMITCLIENTS;

static OsSigWrapperPtr OsSigWrapper = NULL;

OsSigWrapperPtr
OsRegisterSigWrapper(OsSigWrapperPtr newSigWrapper)
{
    OsSigWrapperPtr oldSigWrapper = OsSigWrapper;

    OsSigWrapper = newSigWrapper;

    return oldSigWrapper;
}

/*
 * OsSigHandler --
 *    Catch unexpected signals and exit or continue cleanly.
 */
#if !defined(WIN32) || defined(__CYGWIN__)
static void
#ifdef SA_SIGINFO
OsSigHandler(int signo, siginfo_t * sip, void *unused)
#else
OsSigHandler(int signo)
#endif
{
#ifdef RTLD_DI_SETSIGNAL
    # define SIGNAL_FOR_RTLD_ERROR SIGQUIT
    if (signo == SIGNAL_FOR_RTLD_ERROR) {
        const char *dlerr = dlerror();

        if (dlerr) {
            LogMessageVerbSigSafe(X_ERROR, 1,
                                  "Dynamic loader error: %s\n", dlerr);
        }
    }
#endif                          /* RTLD_DI_SETSIGNAL */

    if (OsSigWrapper != NULL) {
        if (OsSigWrapper(signo) == 0) {
            /* ddx handled signal and wants us to continue */
            return;
        }
    }

    /* log, cleanup, and abort */
    xorg_backtrace();

#ifdef SA_SIGINFO
    if (sip->si_code == SI_USER) {
        ErrorFSigSafe("Received signal %u sent by process %u, uid %u\n", signo,
                     sip->si_pid, sip->si_uid);
    }
    else {
        switch (signo) {
        case SIGSEGV:
        case SIGBUS:
        case SIGILL:
        case SIGFPE:
            ErrorFSigSafe("%s at address %p\n", strsignal(signo), sip->si_addr);
        }
    }
#endif

    if (signo != SIGQUIT)
        CoreDump = TRUE;

    FatalError("Caught signal %d (%s). Server aborting\n",
               signo, strsignal(signo));
}
#endif /* !WIN32 || __CYGWIN__ */

#include "../x11/headers/busfault.h"
#include "../main_wm.h"

void
OsInit(void)
{
    static Bool been_here = FALSE;
#ifndef XQUARTZ
    static const char *devnull = "/dev/null";
    char fname[PATH_MAX];
#endif

    if (!been_here) {
#if !defined(WIN32) || defined(__CYGWIN__)
        struct sigaction act, oact;
        int i;

        int siglist[] = { SIGSEGV, SIGQUIT, SIGILL, SIGFPE, SIGBUS,
                          SIGABRT,
                          SIGSYS,
                          SIGXCPU,
                          SIGXFSZ,
#ifdef SIGEMT
                SIGEMT,
#endif
                          0 /* must be last */
        };
        sigemptyset(&act.sa_mask);
#ifdef SA_SIGINFO
        act.sa_sigaction = OsSigHandler;
        act.sa_flags = SA_SIGINFO;
#else
        act.sa_handler = OsSigHandler;
        act.sa_flags = 0;
#endif
        for (i = 0; siglist[i] != 0; i++) {
            if (sigaction(siglist[i], &act, &oact)) {
                ErrorF("failed to install signal handler for signal %d: %s\n",
                       siglist[i], strerror(errno));
            }
        }
#endif /* !WIN32 || __CYGWIN__ */
#ifdef BUSFAULT
        busfault_init();
#endif
        server_poll = ospoll_create();
        if (!server_poll)
            FatalError("failed to allocate poll structure");
#ifdef HAVE_BACKTRACE
        /*
         * initialize the backtracer, since the ctor calls dlopen(), which
         * calls malloc(), which isn't signal-safe.
         */
        do {
            void *array;

            backtrace(&array, 1);
        } while (0);
#endif

#ifdef RTLD_DI_SETSIGNAL
        /* Tell runtime linker to send a signal we can catch instead of SIGKILL
         * for failures to load libraries/modules at runtime so we can clean up
         * after ourselves.
         */
        {
            int failure_signal = SIGNAL_FOR_RTLD_ERROR;

            dlinfo(RTLD_SELF, RTLD_DI_SETSIGNAL, &failure_signal);
        }
#endif

#if !defined(XQUARTZ)    /* STDIN is already /dev/null and STDOUT/STDERR is managed by console_redirect.c */
        /*
         * If a write of zero bytes to stderr returns non-zero, i.e. -1,
         * then writing to stderr failed, and we'll write somewhere else
         * instead. (Apparently this never happens in the Real World.)
         */
        if (write(2, fname, 0) == -1) {
            FILE *err;

            if (strlen(display) + strlen(ADMPATH) + 1 < sizeof fname)
                snprintf(fname, sizeof(fname), ADMPATH, display);
            else
                strcpy(fname, devnull);
            /*
             * uses stdio to avoid os dependencies here,
             * a real os would use
             *  open (fname, O_WRONLY|O_APPEND|O_CREAT, 0666)
             */
            if (!(err = fopen(fname, "a+")))
                err = fopen(devnull, "w");
            if (err && (fileno(err) != 2)) {
                dup2(fileno(err), 2);
                fclose(err);
            }
#if defined(SYSV) || defined(SVR4) || defined(WIN32) || defined(__CYGWIN__)
            {
                static char buf[BUFSIZ];

                setvbuf(stderr, buf, _IOLBF, BUFSIZ);
            }
#else
            setlinebuf(stderr);
#endif
        }
#endif /* !XQUARTZ */
#if !defined(WIN32) || defined(__CYGWIN__)
        if (getpgrp() == 0)
            setpgid(0, 0);
#endif

#ifdef RLIMIT_DATA
        if (limitDataSpace >= 0) {
            struct rlimit rlim;

            if (!getrlimit(RLIMIT_DATA, &rlim)) {
                if ((limitDataSpace > 0) && (limitDataSpace < rlim.rlim_max))
                    rlim.rlim_cur = limitDataSpace;
                else
                    rlim.rlim_cur = rlim.rlim_max;
                (void) setrlimit(RLIMIT_DATA, &rlim);
            }
        }
#endif
#ifdef RLIMIT_STACK
        if (limitStackSpace >= 0) {
            struct rlimit rlim;
            if (!getrlimit(RLIMIT_STACK, &rlim)) {
                if ((limitStackSpace > 0) && (limitStackSpace < rlim.rlim_max))
                    rlim.rlim_cur = limitStackSpace;
                else
                    rlim.rlim_cur = rlim.rlim_max;
                (void) setrlimit(RLIMIT_STACK, &rlim);
            }
        }
#endif
#ifdef RLIMIT_NOFILE
        if (limitNoFile >= 0) {
            struct rlimit rlim;

            if (!getrlimit(RLIMIT_NOFILE, &rlim)) {
                if ((limitNoFile > 0) && (limitNoFile < rlim.rlim_max))
                    rlim.rlim_cur = limitNoFile;
                else
                    rlim.rlim_cur = rlim.rlim_max;
                (void) setrlimit(RLIMIT_NOFILE, &rlim);
            }
        }
#endif
        LockServer();
        been_here = TRUE;
    }
    TimerInit();
    OsVendorInit();
    OsResetSignals();
    /*
     * No log file by default.  OsVendorInit() should call LogInit() with the
     * log file name if logging to a file is desired.
     */
    LogInit(NULL, NULL);
    SmartScheduleInit();
}

void
OsCleanup(Bool terminating)
{
    if (terminating) {
        UnlockServer();
    }
}
