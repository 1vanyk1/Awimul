#include "../libXt/Intrinsic.h"
#include "../headers/xfuncproto.h"

#define XER_NBBY 8		/* number of bits in a byte */
#define BYTE_MASK 255

#define HEADER_SIZE 6

#define EDITRES_IS_OBJECT 2
#define EDITRES_IS_UNREALIZED 0

/*
 * Format for atoms
 */
#define EDITRES_FORMAT             8
#define EDITRES_SEND_EVENT_FORMAT 32

/*
 * Atoms
 */
#define EDITRES_NAME         "Editres"
#define EDITRES_COMMAND_ATOM "EditresCommand"
#define EDITRES_COMM_ATOM    "EditresComm"
#define EDITRES_CLIENT_VALUE "EditresClientVal"
#define EDITRES_PROTOCOL_ATOM "EditresProtocol"

typedef enum {
    SendWidgetTree = 0,
    SetValues      = 1,
    GetResources   = 2,
    GetGeometry    = 3,
    FindChild      = 4,
    GetValues      = 5
} EditresCommand;

typedef enum {
    NormalResource     = 0,
    ConstraintResource = 1
} ResourceType;

/*
 * The type of a resource identifier
 */
typedef unsigned char ResIdent;

typedef enum {
    PartialSuccess   = 0,
    Failure	   = 1,
    ProtocolMismatch = 2
} EditResError;

typedef struct _WidgetInfo {
    unsigned short num_widgets;
    unsigned long *ids;
    Widget real_widget;
} WidgetInfo;

typedef struct _ProtocolStream {
    unsigned long size, alloc;
    unsigned char *real_top, *top, *current;
} ProtocolStream;

/************************************************************
 * Function definitions for reading and writing protocol requests
 ************************************************************/
_XFUNCPROTOBEGIN

void _XEditResPutString8
        (
                ProtocolStream		*stream,
                _Xconst char		*str
        );

void _XEditResPut8
        (
                ProtocolStream		*stream,
                unsigned int		value
        );

void _XEditResPut16
        (
                ProtocolStream		*stream,
                unsigned int		value
        );

void _XEditResPut32
        (
                ProtocolStream		*stream,
                unsigned long		value
        );

void _XEditResPutWidgetInfo
        (
                ProtocolStream		*stream,
                WidgetInfo		*info
        );

void _XEditResResetStream
        (
                ProtocolStream		*stream
        );

Bool _XEditResGet8
        (
                ProtocolStream		*stream,
                unsigned char		*value
        );

Bool _XEditResGet16
        (
                ProtocolStream		*stream,
                unsigned short		*value
        );

Bool _XEditResGetSigned16
        (
                ProtocolStream		*stream,
                short			*value
        );

Bool _XEditResGet32
        (
                ProtocolStream		*stream,
                unsigned long		*value
        );

Bool _XEditResGetString8
        (
                ProtocolStream		*stream,
                char			**str
        );

Bool _XEditResGetWidgetInfo
        (
                ProtocolStream		*stream,
                WidgetInfo		*info
        );

_XFUNCPROTOEND
