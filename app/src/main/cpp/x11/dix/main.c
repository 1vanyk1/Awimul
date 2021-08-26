#ifdef HAVE_DIX_CONFIG_H
#include <dix-config.h>
#include <version-config.h>
#endif

#include <jni.h>
#include "../headers/x.h"
#include "../headers/xos.h"          /* for unistd.h  */
#include "../headers/xproto.h"
#include "../../pixman/pixman.h"
#include "../headers/scrnintstr.h"
#include "../headers/misc.h"
#include "../headers/os.h"
#include "../headers/windowstr.h"
#include "../headers/resource.h"
#include "../headers/dixstruct.h"
#include "../headers/gcstruct.h"
#include "../headers/extension.h"
#include "../headers/colormap.h"
#include "../headers/colormapst.h"
#include "../headers/cursorstr.h"
#include "../headers/selection.h"
#include "../headers/fonts/font.h"
#include "../headers/fonts/fontstruct.h"
#include "../headers/fonts/libxfont2.h"
#include "../headers/opaque.h"
#include "../headers/servermd.h"
#include "../headers/hotplug.h"
#include "../headers/dixfont.h"
#include "../headers/extnsionst.h"
#include "../headers/privates.h"
#include "../headers/registry.h"
#include "../headers/client.h"
#include "../headers/exevents.h"
#ifdef PANORAMIX
#include "../xext/panoramiXsrv.h"
#else
         /* InitEvents() */
#endif

#ifdef DPMSExtension
#include "../headers/extensions/dpmsconst.h"
#include "../xext/dpmsproc.h"
#include "../../main_wm.h"

#endif

extern void Dispatch(void);

CallbackListPtr RootWindowFinalizeCallback = NULL;

int
dix_main(int argc, char *argv[], char *envp[])
{
    int i;
    HWEventQueueType alwaysCheckForInput[2];

    display = "0";

    InitRegions();

    CheckUserParameters(argc, argv, envp);

    CheckUserAuthorization();

    ProcessCommandLine(argc, argv);

    alwaysCheckForInput[0] = 0;
    alwaysCheckForInput[1] = 1;
    while (1) {
        serverGeneration++;
        ScreenSaverTime = defaultScreenSaverTime;
        ScreenSaverInterval = defaultScreenSaverInterval;
        ScreenSaverBlanking = defaultScreenSaverBlanking;
        ScreenSaverAllowExposures = defaultScreenSaverAllowExposures;

        InitBlockAndWakeupHandlers();
        /* Perform any operating system dependent initializations you'd like */
        OsInit();
        ALOGE("null OSINIT");
        if (serverGeneration == 1) {
            ALOGE("serverGeneration");
            CreateWellKnownSockets();
            for (i = 1; i < LimitClients; i++)
                clients[i] = NullClient;
            serverClient = calloc(sizeof(ClientRec), 1);
            if (!serverClient)
                FatalError("couldn't create server client");
            InitClient(serverClient, 0, (void *) NULL);
        }
        else {
            ALOGE("null OSINIT 2");
            ResetWellKnownSockets();
        }
        clients[0] = serverClient;
        currentMaxClients = 1;
        /* clear any existing selections */
        InitSelections();
        /* Initialize privates before first allocation */
        dixResetPrivates();
        /* Initialize server client devPrivates, to be reallocated as
         * more client privates are registered
         */
        if (!dixAllocatePrivates(&serverClient->devPrivates, PRIVATE_CLIENT))
            FatalError("failed to create server client privates");

        if (!InitClientResources(serverClient)) /* for root resources */
            FatalError("couldn't init server resources");

        SetInputCheck(&alwaysCheckForInput[0], &alwaysCheckForInput[1]);
        screenInfo.numScreens = 0;
        InitAtoms();
        InitEvents();
        xfont2_init_glyph_caching();
        dixResetRegistry();
        InitFonts();
        InitCallbackManager();
        InitOutput(&screenInfo, argc, argv);
        if (screenInfo.numScreens < 1)
            FatalError("no screens found");
        InitExtensions(argc, argv);
        for (i = 0; i < screenInfo.numGPUScreens; i++) {
            ScreenPtr pScreen = screenInfo.gpuscreens[i];
            if (!CreateScratchPixmapsForScreen(pScreen))
                FatalError("failed to create scratch pixmaps");
            if (pScreen->CreateScreenResources &&
                !(*pScreen->CreateScreenResources) (pScreen))
                FatalError("failed to create screen resources");
        }
        for (i = 0; i < screenInfo.numScreens; i++) {
            ScreenPtr pScreen = screenInfo.screens[i];

            if (!CreateScratchPixmapsForScreen(pScreen))
                FatalError("failed to create scratch pixmaps");
            if (pScreen->CreateScreenResources &&
                !(*pScreen->CreateScreenResources) (pScreen))
                FatalError("failed to create screen resources");
            if (!CreateGCperDepth(i))
                FatalError("failed to create scratch GCs");
            if (!CreateDefaultStipple(i))
                FatalError("failed to create default stipple");
            if (!CreateRootWindow(pScreen))
                FatalError("failed to create root window");
            CallCallbacks(&RootWindowFinalizeCallback, pScreen);
        }

        if (SetDefaultFontPath(defaultFontPath) != Success) {
            ErrorF("[dix] failed to set default font path '%s'",
                   defaultFontPath);
        }
        if (!SetDefaultFont("fixed")) {
            FatalError("could not open default font");
        }

        if (!(rootCursor = CreateRootCursor(NULL, 0))) {
            FatalError("could not open default cursor font");
        }
#ifdef PANORAMIX
        /*
         * Consolidate window and colourmap information for each screen
         */
        if (!noPanoramiXExtension)
            PanoramiXConsolidate();
#endif
        for (i = 0; i < screenInfo.numScreens; i++)
            InitRootWindow(screenInfo.screens[i]->root);
        InitCoreDevices();
        InitInput(argc, argv);
        ALOGE("InitInput");
        InitAndStartDevices();
        ReserveClientIds(serverClient);

        dixSaveScreens(serverClient, SCREEN_SAVER_FORCER, ScreenSaverReset);

        dixCloseRegistry();

#ifdef PANORAMIX
        if (!noPanoramiXExtension) {
            if (!PanoramiXCreateConnectionBlock()) {
                FatalError("could not create connection block info");
            }
        }
        else
#endif
        {
            if (!CreateConnectionBlock()) {
                FatalError("could not create connection block info");
            }
        }

        NotifyParentProcess();

        InputThreadInit();

        Dispatch();

        UndisplayDevices();
        DisableAllDevices();

        /* Now free up whatever must be freed */
        if (screenIsSaved == SCREEN_SAVER_ON)
            dixSaveScreens(serverClient, SCREEN_SAVER_OFF, ScreenSaverReset);
        FreeScreenSaverTimer();
        CloseDownExtensions();

#ifdef PANORAMIX
        {
            Bool remember_it = noPanoramiXExtension;

            noPanoramiXExtension = TRUE;
            FreeAllResources();
            noPanoramiXExtension = remember_it;
        }
#else
        FreeAllResources();
#endif
        ALOGE("CloseInput");
        CloseInput();

        InputThreadFini();

        for (i = 0; i < screenInfo.numScreens; i++)
            screenInfo.screens[i]->root = NullWindow;

        CloseDownDevices();

        CloseDownEvents();

        for (i = screenInfo.numGPUScreens - 1; i >= 0; i--) {
            ScreenPtr pScreen = screenInfo.gpuscreens[i];
            FreeScratchPixmapsForScreen(pScreen);
            dixFreeScreenSpecificPrivates(pScreen);
            (*pScreen->CloseScreen) (pScreen);
            dixFreePrivates(pScreen->devPrivates, PRIVATE_SCREEN);
            free(pScreen);
            screenInfo.numGPUScreens = i;
        }

        for (i = screenInfo.numScreens - 1; i >= 0; i--) {
            FreeScratchPixmapsForScreen(screenInfo.screens[i]);
            FreeGCperDepth(i);
            FreeDefaultStipple(i);
            dixFreeScreenSpecificPrivates(screenInfo.screens[i]);
            (*screenInfo.screens[i]->CloseScreen) (screenInfo.screens[i]);
            dixFreePrivates(screenInfo.screens[i]->devPrivates, PRIVATE_SCREEN);
            free(screenInfo.screens[i]);
            screenInfo.numScreens = i;
        }

        ReleaseClientIds(serverClient);
        dixFreePrivates(serverClient->devPrivates, PRIVATE_CLIENT);
        serverClient->devPrivates = NULL;

        dixFreeRegistry();

        FreeFonts();

        FreeAllAtoms();

        FreeAuditTimer();

        DeleteCallbackManager();

        ClearWorkQueue();

        if (dispatchException & DE_TERMINATE) {
            CloseWellKnownConnections();
        }

        OsCleanup((dispatchException & DE_TERMINATE) != 0);

        if (dispatchException & DE_TERMINATE) {
            ddxGiveUp(EXIT_NO_ERROR);
            break;
        }

        free(ConnectionInfo);
        ConnectionInfo = NULL;
    }
    return 0;
}
