#ifndef INDICATORS_H
#define INDICATORS_H 1

#define	_LED_Index	(1<<0)
#define	_LED_Mods	(1<<1)
#define	_LED_Groups	(1<<2)
#define	_LED_Ctrls	(1<<3)
#define	_LED_Explicit	(1<<4)
#define	_LED_Automatic	(1<<5)
#define	_LED_DrivesKbd	(1<<6)

#define	_LED_NotBound	255

typedef struct _LEDInfo
{
    CommonInfo defs;
    Atom name;
    unsigned char indicator;
    unsigned char flags;
    unsigned char which_mods;
    unsigned char real_mods;
    unsigned short vmods;
    unsigned char which_groups;
    unsigned char groups;
    unsigned int ctrls;
} LEDInfo;

extern void ClearIndicatorMapInfo(Display * /* dpy */ ,
                                  LEDInfo *     /* info */
);


extern LEDInfo *AddIndicatorMap(LEDInfo * /* oldLEDs */ ,
                                LEDInfo *       /* newLED */
);

extern int SetIndicatorMapField(LEDInfo * /* led */ ,
                                XkbDescPtr /* xkb */ ,
                                const char * /* field */ ,
                                ExprDef * /* arrayNdx */ ,
                                ExprDef *       /* value */
);

extern LEDInfo *HandleIndicatorMapDef(IndicatorMapDef * /* stmt */ ,
                                      XkbDescPtr /* xkb */ ,
                                      LEDInfo * /* dflt */ ,
                                      LEDInfo * /* oldLEDs */ ,
                                      unsigned  /* mergeMode */
);

extern Bool CopyIndicatorMapDefs(XkbFileInfo * /* result */ ,
                                 LEDInfo * /* leds */ ,
                                 LEDInfo **     /* unboundRtrn */
);

extern Bool BindIndicators(XkbFileInfo * /* result */ ,
                           Bool /* force */ ,
                           LEDInfo * /* unbound */ ,
                           LEDInfo **   /* unboundRtrn */
);

#endif /* INDICATORS_H */
