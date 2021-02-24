package com.vantacom.aarm.libraries.jna;

import com.sun.jna.Library;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import com.sun.jna.ptr.ByteByReference;
import com.sun.jna.ptr.IntByReference;
import com.sun.jna.win32.W32APIOptions;
import com.vantacom.aarm.libraries.jna.WinGDI.ICONINFO;

public interface User32 extends Library, WinUser, WinNT {
    User32 INSTANCE = Native.load("/data/user/0/com.vantacom.aarm/files/armeabi-v7a/lib/wine/user32.dll.so", User32.class, W32APIOptions.DEFAULT_OPTIONS);
    HWND HWND_MESSAGE = new HWND(Pointer.createConstant(-3));
    int CS_GLOBALCLASS = 0x4000;
    int WS_EX_TOPMOST = 0x00000008;
    int DEVICE_NOTIFY_WINDOW_HANDLE = 0x00000000;
    int DEVICE_NOTIFY_SERVICE_HANDLE = 0x00000001;
    int DEVICE_NOTIFY_ALL_INTERFACE_CLASSES = 0x00000004;
    int SW_SHOWDEFAULT = 10;

    HDC GetDC(HWND hWnd);

    int ReleaseDC(HWND hWnd, HDC hDC);

    HWND FindWindow(String lpClassName, String lpWindowName);

    int GetClassName(HWND hWnd, char[] lpClassName, int nMaxCount);

    boolean GetGUIThreadInfo(int idThread, GUITHREADINFO lpgui);

    boolean GetWindowInfo(HWND hWnd, WINDOWINFO pwi);

    boolean GetWindowRect(HWND hWnd, RECT rect);

    boolean GetClientRect(HWND hWnd, RECT rect);

    int GetWindowText(HWND hWnd, char[] lpString, int nMaxCount);

    int GetWindowTextLength(HWND hWnd);

    int GetWindowModuleFileName(HWND hWnd, char[] lpszFileName, int cchFileNameMax);

    int GetWindowThreadProcessId(HWND hWnd, IntByReference lpdwProcessId);

    boolean EnumWindows(WNDENUMPROC lpEnumFunc, Pointer data);

    boolean EnumChildWindows(HWND hWnd, WNDENUMPROC lpEnumFunc, Pointer data);

    boolean EnumThreadWindows(int dwThreadId, WNDENUMPROC lpEnumFunc, Pointer data);

    boolean FlashWindowEx(FLASHWINFO pfwi);

    HICON LoadIcon(HINSTANCE hInstance, String iconName);

    HANDLE LoadImage(HINSTANCE hinst, String name, int type, int xDesired,
                     int yDesired, int load);

    boolean DestroyIcon(HICON hicon);

    int GetWindowLong(HWND hWnd, int nIndex);

    int SetWindowLong(HWND hWnd, int nIndex, int dwNewLong);

    LONG_PTR GetWindowLongPtr(HWND hWnd, int nIndex);

    Pointer SetWindowLongPtr(HWND hWnd, int nIndex, Pointer dwNewLongPtr);

    boolean SetLayeredWindowAttributes(HWND hwnd, int crKey, byte bAlpha, int dwFlags);

    boolean GetLayeredWindowAttributes(HWND hwnd, IntByReference pcrKey,
                                       ByteByReference pbAlpha, IntByReference pdwFlags);

    boolean UpdateLayeredWindow(HWND hwnd, HDC hdcDst, POINT pptDst,
                                SIZE psize, HDC hdcSrc, POINT pptSrc, int crKey,
                                BLENDFUNCTION pblend, int dwFlags);

    int SetWindowRgn(HWND hWnd, HRGN hRgn, boolean bRedraw);

    boolean GetKeyboardState(byte[] lpKeyState);

    short GetAsyncKeyState(int vKey);

    HHOOK SetWindowsHookEx(int idHook, HOOKPROC lpfn, HINSTANCE hMod, int dwThreadId);

    LRESULT CallNextHookEx(HHOOK hhk, int nCode, WPARAM wParam, LPARAM lParam);

    boolean UnhookWindowsHookEx(HHOOK hhk);

    int GetMessage(MSG lpMsg, HWND hWnd, int wMsgFilterMin, int wMsgFilterMax);

    boolean PeekMessage(MSG lpMsg, HWND hWnd, int wMsgFilterMin,
                        int wMsgFilterMax, int wRemoveMsg);

    boolean TranslateMessage(MSG lpMsg);

    LRESULT DispatchMessage(MSG lpMsg);

    void PostMessage(HWND hWnd, int msg, WPARAM wParam, LPARAM lParam);

    int PostThreadMessage(int  idThread, int  Msg, WPARAM wParam,  LPARAM lParam);

    void PostQuitMessage(int nExitCode);

    int GetSystemMetrics(int nIndex);

    HWND SetParent(HWND hWndChild, HWND hWndNewParent);

    boolean IsWindowVisible(HWND hWnd);

    boolean MoveWindow(HWND hWnd, int X, int Y, int nWidth, int nHeight, boolean bRepaint);

    boolean SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx,
                         int cy, int uFlags);

    boolean AttachThreadInput(DWORD idAttach, DWORD idAttachTo, boolean fAttach);

    boolean SetForegroundWindow(HWND hWnd);

    HWND GetForegroundWindow();

    HWND SetFocus(HWND hWnd);

    DWORD SendInput(DWORD nInputs, WinUser.INPUT[] pInputs, int cbSize);

    DWORD WaitForInputIdle(HANDLE hProcess, DWORD dwMilliseconds);

    boolean InvalidateRect(HWND hWnd, RECT lpRect, boolean bErase);

    boolean RedrawWindow(HWND hWnd, RECT lprcUpdate, HRGN hrgnUpdate, DWORD flags);

    HWND GetWindow(HWND hWnd, DWORD uCmd);

    boolean UpdateWindow(HWND hWnd);

    boolean ShowWindow(HWND hWnd, int nCmdShow);

    boolean CloseWindow(HWND hWnd);

    boolean RegisterHotKey(HWND hWnd, int id, int fsModifiers, int vk);

    boolean UnregisterHotKey(Pointer hWnd, int id);

    boolean GetLastInputInfo(LASTINPUTINFO plii);

    ATOM RegisterClassEx(WNDCLASSEX lpwcx);

    boolean UnregisterClass(String lpClassName, HINSTANCE hInstance);

    HWND CreateWindowEx(int dwExStyle, String lpClassName,
                        String lpWindowName, int dwStyle, int x, int y, int nWidth,
                        int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance,
                        LPVOID lpParam);

    boolean DestroyWindow(HWND hWnd);

    boolean GetClassInfoEx(HINSTANCE hinst, String lpszClass, WNDCLASSEX lpwcx);

    LRESULT CallWindowProc(Pointer lpPrevWndFunc, HWND hWnd, int Msg, WPARAM wParam, LPARAM lParam);

    LRESULT DefWindowProc(HWND hWnd, int Msg, WPARAM wParam, LPARAM lParam);

    HDEVNOTIFY RegisterDeviceNotification(HANDLE hRecipient, Structure notificationFilter, int Flags);

    boolean UnregisterDeviceNotification(HDEVNOTIFY Handle);

    int RegisterWindowMessage(String string);

    HMONITOR MonitorFromPoint(POINT.ByValue pt, int dwFlags);

    HMONITOR MonitorFromRect(RECT lprc, int dwFlags);

    HMONITOR MonitorFromWindow(HWND hwnd, int dwFlags);

    BOOL GetMonitorInfo(HMONITOR hMonitor, MONITORINFO lpmi);

    BOOL GetMonitorInfo(HMONITOR hMonitor, MONITORINFOEX lpmi);

    BOOL EnumDisplayMonitors(HDC hdc, RECT lprcClip, MONITORENUMPROC lpfnEnum, LPARAM dwData);

    BOOL GetWindowPlacement(HWND hwnd, WINDOWPLACEMENT lpwndpl);

    BOOL SetWindowPlacement(HWND hwnd, WINDOWPLACEMENT lpwndpl);

    BOOL AdjustWindowRect(RECT lpRect, DWORD dwStyle, BOOL bMenu);

    BOOL AdjustWindowRectEx(RECT lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle);

    BOOL ExitWindowsEx(UINT uFlags, DWORD dReason);

    BOOL LockWorkStation();

    boolean GetIconInfo(HICON hIcon, ICONINFO piconinfo);

    LRESULT SendMessageTimeout(HWND hWnd, int msg, WPARAM wParam, LPARAM lParam,
                               int fuFlags, int uTimeout, DWORDByReference lpdwResult);

    ULONG_PTR GetClassLongPtr(HWND hWnd, int nIndex);

    int GetRawInputDeviceList(RAWINPUTDEVICELIST[] pRawInputDeviceList, IntByReference puiNumDevices, int cbSize);

    HWND GetDesktopWindow();

    boolean PrintWindow(HWND hWnd, HDC dest, int flags);

    boolean IsWindowEnabled(HWND hWnd);

    boolean IsWindow(HWND hWnd);

    HWND FindWindowEx(HWND parent, HWND child, String className, String window);

    HWND GetAncestor(HWND hwnd, int gaFlags);

    boolean GetCursorPos(POINT p);

    boolean SetCursorPos(long x, long y);

    HANDLE SetWinEventHook(int eventMin, int eventMax, HMODULE hmodWinEventProc, WinEventProc winEventProc,
                           int processID, int threadID, int flags);

    boolean UnhookWinEvent(HANDLE hook);

    HICON CopyIcon(HICON hIcon);

    int GetClassLong(HWND hWnd, int nIndex);

    public int RegisterClipboardFormat(String formatName);

    public HWND GetActiveWindow();

    LRESULT SendMessage(HWND hWnd, int msg, WPARAM wParam, LPARAM lParam);

    int GetKeyboardLayoutList(int nBuff, HKL[] lpList);

    boolean GetKeyboardLayoutName(char[] pwszKLID);

    short VkKeyScanExA(byte ch, HKL dwhkl);

    short VkKeyScanExW(char ch, HKL dwhkl);

    int MapVirtualKeyEx(int uCode, int uMapType, HKL dwhkl);

    int ToUnicodeEx(int wVirtKey, int wScanCode, byte[] lpKeyState, char[] pwszBuff, int cchBuff, int wFlags, HKL dwhkl);

    int LoadString(HINSTANCE hInstance, int uID, Pointer lpBuffer, int cchBufferMax);
}
