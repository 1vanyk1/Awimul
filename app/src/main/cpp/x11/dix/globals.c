#include "../headers/x.h"
#include "../headers/xmd.h"
#include "../headers/misc.h"
#include "../headers/windowstr.h"
#include "../headers/scrnintstr.h"
#include "../headers/input.h"
#include "../headers/dixfont.h"
#include "../headers/dixstruct.h"
#include "../headers/os.h"
#include "../headers/xorg-server.h"

ScreenInfo screenInfo;

KeybdCtrl defaultKeyboardControl = {
        DEFAULT_KEYBOARD_CLICK,
        DEFAULT_BELL,
        DEFAULT_BELL_PITCH,
        DEFAULT_BELL_DURATION,
        DEFAULT_AUTOREPEAT,
        DEFAULT_AUTOREPEATS,
        DEFAULT_LEDS,
        0
};

PtrCtrl defaultPointerControl = {
        DEFAULT_PTR_NUMERATOR,
        DEFAULT_PTR_DENOMINATOR,
        DEFAULT_PTR_THRESHOLD,
        0
};

ClientPtr clients[MAXCLIENTS];
ClientPtr serverClient;
int currentMaxClients;          /* current size of clients array */
long maxBigRequestSize = MAX_BIG_REQUEST_SIZE;

unsigned long globalSerialNumber = 0;
unsigned long serverGeneration = 0;

/* these next four are initialized in main.c */
CARD32 ScreenSaverTime;
CARD32 ScreenSaverInterval;
int ScreenSaverBlanking;
int ScreenSaverAllowExposures;

/* default time of 10 minutes */
CARD32 defaultScreenSaverTime = (10 * (60 * 1000));
CARD32 defaultScreenSaverInterval = (10 * (60 * 1000));
int defaultScreenSaverBlanking = PreferBlanking;
int defaultScreenSaverAllowExposures = AllowExposures;

#ifdef SCREENSAVER
Bool screenSaverSuspended = FALSE;
#endif

const char *defaultFontPath = COMPILEDDEFAULTFONTPATH;
FontPtr defaultFont;            /* not declared in dix.h to avoid including font.h in
                                   every compilation of dix code */
CursorPtr rootCursor;
Bool party_like_its_1989 = FALSE;
Bool whiteRoot = FALSE;

TimeStamp currentTime;

int defaultColorVisualClass = -1;
int monitorResolution = 0;

const char *display;
int displayfd = -1;
Bool explicit_display = FALSE;
char *ConnectionInfo;
