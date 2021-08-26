extern Bool miSpriteInitialize(ScreenPtr /*pScreen */ ,
                               miPointerScreenFuncPtr   /*screenFuncs */
);

extern Bool miDCRealizeCursor(ScreenPtr pScreen, CursorPtr pCursor);
extern Bool miDCUnrealizeCursor(ScreenPtr pScreen, CursorPtr pCursor);
extern Bool miDCPutUpCursor(DeviceIntPtr pDev, ScreenPtr pScreen,
                            CursorPtr pCursor, int x, int y,
                            unsigned long source, unsigned long mask);
extern Bool miDCSaveUnderCursor(DeviceIntPtr pDev, ScreenPtr pScreen,
                                int x, int y, int w, int h);
extern Bool miDCRestoreUnderCursor(DeviceIntPtr pDev, ScreenPtr pScreen,
                                   int x, int y, int w, int h);
extern Bool miDCDeviceInitialize(DeviceIntPtr pDev, ScreenPtr pScreen);
extern void miDCDeviceCleanup(DeviceIntPtr pDev, ScreenPtr pScreen);
