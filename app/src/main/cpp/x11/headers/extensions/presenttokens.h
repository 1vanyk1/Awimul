#ifndef _PRESENT_TOKENS_H_
#define _PRESENT_TOKENS_H_

#define PRESENT_NAME			"Present"
#define PRESENT_MAJOR			1
#define PRESENT_MINOR			0

#define PresentNumberErrors		0
#define PresentNumberEvents		0

/* Requests */
#define X_PresentQueryVersion		0
#define X_PresentPixmap			1
#define X_PresentNotifyMSC		2
#define X_PresentSelectInput		3
#define X_PresentQueryCapabilities      4

#define PresentNumberRequests		5

/* Present operation options */
#define PresentOptionNone               0
#define PresentOptionAsync              (1 << 0)
#define PresentOptionCopy               (1 << 1)
#define PresentOptionUST                (1 << 2)

#define PresentAllOptions       (PresentOptionAsync | \
                                 PresentOptionCopy | \
                                 PresentOptionUST)

/* Present capabilities */

#define PresentCapabilityNone           0
#define PresentCapabilityAsync          1
#define PresentCapabilityFence          2
#define PresentCapabilityUST            4

#define PresentAllCapabilities  (PresentCapabilityAsync | \
                                 PresentCapabilityFence | \
                                 PresentCapabilityUST)

/* Events */
#define PresentConfigureNotify	0
#define PresentCompleteNotify	1
#define PresentIdleNotify       2
#if PRESENT_FUTURE_VERSION
#define PresentRedirectNotify	3
#endif

/* Event Masks */
#define PresentConfigureNotifyMask      1
#define PresentCompleteNotifyMask       2
#define PresentIdleNotifyMask           4
#if PRESENT_FUTURE_VERSION
#define PresentRedirectNotifyMask       8
#endif

#if PRESENT_FUTURE_VERSION
#define PRESENT_REDIRECT_NOTIFY_MASK    PresentRedirectNotifyMask
#else
#define PRESENT_REDIRECT_NOTIFY_MASK    0
#endif

#define PresentAllEvents   (PresentConfigureNotifyMask |        \
                            PresentCompleteNotifyMask |         \
                            PresentIdleNotifyMask |             \
                            PRESENT_REDIRECT_NOTIFY_MASK)

/* Complete Kinds */

#define PresentCompleteKindPixmap       0
#define PresentCompleteKindNotifyMSC    1

/* Complete Modes */

#define PresentCompleteModeCopy         0
#define PresentCompleteModeFlip         1
#define PresentCompleteModeSkip         2

#endif
