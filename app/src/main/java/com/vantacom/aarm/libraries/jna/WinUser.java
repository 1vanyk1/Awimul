package com.vantacom.aarm.libraries.jna;

import com.sun.jna.Callback;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.Structure;
import com.sun.jna.Union;
import com.sun.jna.win32.StdCallLibrary.StdCallCallback;
import com.vantacom.aarm.libraries.jna.BaseTSD.ULONG_PTR;
import com.vantacom.aarm.libraries.jna.WinNT.HANDLE;

import java.util.Arrays;
import java.util.List;

import static com.sun.jna.win32.W32APITypeMapper.DEFAULT;

public interface WinUser extends WinDef {
    HWND HWND_BROADCAST = new HWND(Pointer.createConstant(0xFFFF));
    HWND HWND_MESSAGE = new HWND(Pointer.createConstant(-3));

    public static class HDEVNOTIFY extends PVOID {
        public HDEVNOTIFY() { }

        public HDEVNOTIFY(Pointer p) {
            super(p);
        }
    }

    int FLASHW_STOP = 0;
    int FLASHW_CAPTION = 1;
    int FLASHW_TRAY = 2;
    int FLASHW_ALL = (FLASHW_CAPTION | FLASHW_TRAY);
    int FLASHW_TIMER = 4;
    int FLASHW_TIMERNOFG = 12;

    int IMAGE_BITMAP = 0;
    int IMAGE_ICON = 1;
    int IMAGE_CURSOR = 2;
    int IMAGE_ENHMETAFILE = 3;

    int LR_DEFAULTCOLOR = 0x0000;
    int LR_MONOCHROME = 0x0001;
    int LR_COLOR = 0x0002;
    int LR_COPYRETURNORG = 0x0004;
    int LR_COPYDELETEORG = 0x0008;
    int LR_LOADFROMFILE = 0x0010;
    int LR_LOADTRANSPARENT = 0x0020;
    int LR_DEFAULTSIZE = 0x0040;
    int LR_VGACOLOR = 0x0080;
    int LR_LOADMAP3DCOLORS = 0x1000;
    int LR_CREATEDIBSECTION = 0x2000;
    int LR_COPYFROMRESOURCE = 0x4000;
    int LR_SHARED = 0x8000;

    public class GUITHREADINFO extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("cbSize", "flags", "hwndActive", "hwndFocus", "hwndCapture",
                    "hwndMenuOwner", "hwndMoveSize", "hwndCaret", "rcCaret");
        }

        public int cbSize = size();
        public int flags;
        public HWND hwndActive;
        public HWND hwndFocus;
        public HWND hwndCapture;
        public HWND hwndMenuOwner;
        public HWND hwndMoveSize;
        public HWND hwndCaret;
        public RECT rcCaret;
    }

    public class WINDOWINFO extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("cbSize", "rcWindow", "rcClient", "dwStyle", "dwExStyle",
                    "dwWindowStatus", "cxWindowBorders", "cyWindowBorders", "atomWindowType",
                    "wCreatorVersion");
        }

        public int cbSize = size();
        public RECT rcWindow;
        public RECT rcClient;
        public int dwStyle;
        public int dwExStyle;
        public int dwWindowStatus;
        public int cxWindowBorders;
        public int cyWindowBorders;
        public short atomWindowType;
        public short wCreatorVersion;
    }

    public class WINDOWPLACEMENT extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("length","flags","showCmd","ptMinPosition","ptMaxPosition", "rcNormalPosition");
        }

        public static final int WPF_SETMINPOSITION = 0x1;
        public static final int WPF_RESTORETOMAXIMIZED = 0x2;
        public static final int WPF_ASYNCWINDOWPLACEMENT = 0x4;

        public int length = size();
        public int flags;
        public int showCmd;
        public POINT ptMinPosition;
        public POINT ptMaxPosition;
        public RECT rcNormalPosition;
    }

    int GWL_EXSTYLE = -20;
    int GWL_STYLE = -16;
    int GWL_WNDPROC = -4;
    int GWL_HINSTANCE = -6;
    int GWL_ID = -12;
    int GWL_USERDATA = -21;
    int GWL_HWNDPARENT = -8;

    int DWL_DLGPROC = Native.POINTER_SIZE;
    int DWL_MSGRESULT = 0;
    int DWL_USER = 2*Native.POINTER_SIZE;

    int WS_BORDER    = 0x800000;
    int WS_CAPTION    = 0xc00000;
    int WS_CHILD    = 0x40000000;
    int WS_CHILDWINDOW    = 0x40000000;
    int WS_CLIPCHILDREN = 0x2000000;
    int WS_CLIPSIBLINGS = 0x4000000;
    int WS_DISABLED    = 0x8000000;
    int WS_DLGFRAME    = 0x400000;
    int WS_GROUP    = 0x20000;
    int WS_HSCROLL    = 0x100000;
    int WS_ICONIC    = 0x20000000;
    int WS_MAXIMIZE    = 0x1000000;
    int WS_MAXIMIZEBOX    = 0x10000;
    int WS_MINIMIZE    = 0x20000000;
    int WS_MINIMIZEBOX    = 0x20000;
    int WS_OVERLAPPED = 0x00000000;
    int WS_POPUP    = 0x80000000;
    int WS_SYSMENU    = 0x80000;
    int WS_THICKFRAME    = 0x40000;
    int WS_POPUPWINDOW    = (WS_POPUP | WS_BORDER | WS_SYSMENU);
    int WS_OVERLAPPEDWINDOW    = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
            WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
    int WS_SIZEBOX    = 0x40000;
    int WS_TABSTOP    = 0x10000;
    int WS_TILED    = 0;
    int WS_TILEDWINDOW    = (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU |
            WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX);
    int WS_VISIBLE    = 0x10000000;
    int WS_VSCROLL    = 0x200000;
    int WS_EX_COMPOSITED = 0x20000000;
    int WS_EX_LAYERED = 0x80000;
    int WS_EX_TRANSPARENT = 32;
    int LWA_COLORKEY = 1;
    int LWA_ALPHA = 2;

    int ULW_COLORKEY = 1;
    int ULW_ALPHA = 2;
    int ULW_OPAQUE = 4;

    public class MSG extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("hWnd", "message", "wParam", "lParam", "time", "pt");
        }

        public HWND hWnd;
        public int message;
        public WPARAM wParam;
        public LPARAM lParam;
        public int time;
        public POINT pt;
    }

    public class COPYDATASTRUCT extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("dwData", "cbData", "lpData");
        }

        public COPYDATASTRUCT() {
            super();
        }

        public COPYDATASTRUCT(Pointer p) {
            super(p);
            read();
        }

        public ULONG_PTR dwData;
        public int cbData;
        public Pointer lpData;
    }

    public class FLASHWINFO extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("cbSize", "hWnd", "dwFlags", "uCount", "dwTimeout");
        }

        public int cbSize = size();
        public HANDLE hWnd;
        public int dwFlags;
        public int uCount;
        public int dwTimeout;
    }

    public interface WNDENUMPROC extends StdCallCallback {
        boolean callback(HWND hWnd, Pointer data);
    }

    public interface LowLevelMouseProc extends HOOKPROC {
        LRESULT callback(int nCode, WPARAM wParam, MSLLHOOKSTRUCT lParam);
    }

    public interface LowLevelKeyboardProc extends HOOKPROC {
        LRESULT callback(int nCode, WPARAM wParam, KBDLLHOOKSTRUCT lParam);
    }

    public static interface WinEventProc extends Callback {
        void callback(HANDLE hWinEventHook, DWORD event, HWND hwnd, LONG idObject, LONG idChild, DWORD dwEventThread,
                      DWORD dwmsEventTime);
    }

    public class SIZE extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("cx", "cy");
        }

        public int cx, cy;

        public SIZE() {}

        public SIZE(int w, int h) {
            this.cx = w;
            this.cy = h;
        }
    }

    int AC_SRC_OVER = 0x00;
    int AC_SRC_ALPHA = 0x01;
    int AC_SRC_NO_PREMULT_ALPHA = 0x01;
    int AC_SRC_NO_ALPHA = 0x02;

    public class BLENDFUNCTION extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("BlendOp", "BlendFlags", "SourceConstantAlpha", "AlphaFormat");
        }

        public byte BlendOp = AC_SRC_OVER;
        public byte BlendFlags = 0;
        public byte SourceConstantAlpha;
        public byte AlphaFormat;
    }

    int VK_SHIFT = 16;
    int VK_LSHIFT = 0xA0;
    int VK_RSHIFT = 0xA1;
    int VK_CONTROL = 17;
    int VK_LCONTROL = 0xA2;
    int VK_RCONTROL = 0xA3;
    int VK_MENU = 18;
    int VK_LMENU = 0xA4;
    int VK_RMENU = 0xA5;

    int MOD_ALT = 0x0001;
    int MOD_CONTROL = 0x0002;
    int MOD_NOREPEAT = 0x4000;
    int MOD_SHIFT = 0x0004;
    int MOD_WIN = 0x0008;

    int WH_KEYBOARD = 2;
    int WH_CALLWNDPROC = 4;
    int WH_MOUSE = 7;
    int WH_KEYBOARD_LL = 13;
    int WH_MOUSE_LL = 14;

    public class HHOOK extends HANDLE {}

    public interface HOOKPROC extends StdCallCallback {}

    public class CWPSTRUCT extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("lParam", "wParam", "message", "hwnd");
        }

        public CWPSTRUCT() {
            super();
        }

        public CWPSTRUCT(Pointer p) {
            super(p);
            read();
        }

        public LPARAM lParam;
        public WPARAM wParam;
        public int message;
        public HWND hwnd;
    }

    int WM_PAINT = 0x000F;
    int WM_CLOSE = 0x0010;
    int WM_QUIT = 0x0012;
    int WM_SHOWWINDOW = 0x0018;
    int WM_DRAWITEM = 0x002B;
    int WM_KEYDOWN = 0x0100;
    int WM_CHAR = 0x0102;
    int WM_SYSCOMMAND = 0x0112;
    int WM_MDIMAXIMIZE = 0x0225;
    int WM_HOTKEY = 0x0312;
    int WM_USER = 0x0400;
    int WM_COPYDATA = 0x004A;

    int WM_KEYUP = 257;
    int WM_SYSKEYDOWN = 260;
    int WM_SYSKEYUP = 261;

    int WM_SESSION_CHANGE = 0x2b1;
    int WM_CREATE = 0x0001;
    int WM_SIZE = 0x0005;
    int WM_DESTROY = 0x0002;

    public static final int WM_DEVICECHANGE = 0x0219;

    int WM_GETICON = 0x007F;
    int ICON_BIG = 1;
    int ICON_SMALL = 0;
    int ICON_SMALL2 = 2;

    public class MSLLHOOKSTRUCT extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("pt", "mouseData", "flags", "time", "dwExtraInfo");
        }

        public POINT pt;
        public int mouseData;
        public int flags;
        public int time;
        public ULONG_PTR dwExtraInfo;
    }

    public class KBDLLHOOKSTRUCT extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("vkCode", "scanCode", "flags", "time", "dwExtraInfo");
        }

        public int vkCode;
        public int scanCode;
        public int flags;
        public int time;
        public ULONG_PTR dwExtraInfo;
    }

    int SM_CXSCREEN = 0;
    int SM_CYSCREEN = 1;
    int SM_CXVSCROLL = 2;
    int SM_CYHSCROLL = 3;
    int SM_CYCAPTION = 4;
    int SM_CXBORDER = 5;
    int SM_CYBORDER = 6;
    int SM_CXDLGFRAME = 7;
    int SM_CYDLGFRAME = 8;
    int SM_CYVTHUMB = 9;
    int SM_CXHTHUMB = 10;
    int SM_CXICON = 11;
    int SM_CYICON = 12;
    int SM_CXCURSOR = 13;
    int SM_CYCURSOR = 14;
    int SM_CYMENU = 15;
    int SM_CXFULLSCREEN = 16;
    int SM_CYFULLSCREEN = 17;
    int SM_CYKANJIWINDOW = 18;
    int SM_MOUSEPRESENT = 19;
    int SM_CYVSCROLL = 20;
    int SM_CXHSCROLL = 21;
    int SM_DEBUG = 22;
    int SM_SWAPBUTTON = 23;
    int SM_RESERVED1 = 24;
    int SM_RESERVED2 = 25;
    int SM_RESERVED3 = 26;
    int SM_RESERVED4 = 27;
    int SM_CXMIN = 28;
    int SM_CYMIN = 29;
    int SM_CXSIZE = 30;
    int SM_CYSIZE = 31;
    int SM_CXFRAME = 32;
    int SM_CYFRAME = 33;
    int SM_CXMINTRACK = 34;
    int SM_CYMINTRACK = 35;
    int SM_CXDOUBLECLK = 36;
    int SM_CYDOUBLECLK = 37;
    int SM_CXICONSPACING = 38;
    int SM_CYICONSPACING = 39;
    int SM_MENUDROPALIGNMENT = 40;
    int SM_PENWINDOWS = 41;
    int SM_DBCSENABLED = 42;
    int SM_CMOUSEBUTTONS = 43;

    int SM_CXFIXEDFRAME = SM_CXDLGFRAME;
    int SM_CYFIXEDFRAME = SM_CYDLGFRAME;
    int SM_CXSIZEFRAME = SM_CXFRAME;
    int SM_CYSIZEFRAME = SM_CYFRAME;

    int SM_SECURE = 44;
    int SM_CXEDGE = 45;
    int SM_CYEDGE = 46;
    int SM_CXMINSPACING = 47;
    int SM_CYMINSPACING = 48;
    int SM_CXSMICON = 49;
    int SM_CYSMICON = 50;
    int SM_CYSMCAPTION = 51;
    int SM_CXSMSIZE = 52;
    int SM_CYSMSIZE = 53;
    int SM_CXMENUSIZE = 54;
    int SM_CYMENUSIZE = 55;
    int SM_ARRANGE = 56;
    int SM_CXMINIMIZED = 57;
    int SM_CYMINIMIZED = 58;
    int SM_CXMAXTRACK = 59;
    int SM_CYMAXTRACK = 60;
    int SM_CXMAXIMIZED = 61;
    int SM_CYMAXIMIZED = 62;
    int SM_NETWORK = 63;
    int SM_CLEANBOOT = 67;
    int SM_CXDRAG = 68;
    int SM_CYDRAG = 69;
    int SM_SHOWSOUNDS = 70;
    int SM_CXMENUCHECK = 71;
    int SM_CYMENUCHECK = 72;
    int SM_SLOWMACHINE = 73;
    int SM_MIDEASTENABLED = 74;
    int SM_MOUSEWHEELPRESENT = 75;
    int SM_XVIRTUALSCREEN = 76;
    int SM_YVIRTUALSCREEN = 77;
    int SM_CXVIRTUALSCREEN = 78;
    int SM_CYVIRTUALSCREEN = 79;
    int SM_CMONITORS = 80;
    int SM_SAMEDISPLAYFORMAT = 81;
    int SM_IMMENABLED = 82;
    int SM_CXFOCUSBORDER = 83;
    int SM_CYFOCUSBORDER = 84;
    int SM_TABLETPC = 86;
    int SM_MEDIACENTER = 87;
    int SM_STARTER = 88;
    int SM_SERVERR2 = 89;
    int SM_MOUSEHORIZONTALWHEELPRESENT = 91;
    int SM_CXPADDEDBORDER = 92;
    int SM_REMOTESESSION = 0x1000;
    int SM_SHUTTINGDOWN = 0x2000;
    int SM_REMOTECONTROL = 0x2001;
    int SM_CARETBLINKINGENABLED = 0x2002;

    int SW_HIDE = 0;
    int SW_SHOWNORMAL = 1;
    int SW_NORMAL = 1;
    int SW_SHOWMINIMIZED = 2;
    int SW_SHOWMAXIMIZED = 3;
    int SW_MAXIMIZE = 3;
    int SW_SHOWNOACTIVATE = 4;
    int SW_SHOW = 5;
    int SW_MINIMIZE = 6;
    int SW_SHOWMINNOACTIVE = 7;
    int SW_SHOWNA = 8;
    int SW_RESTORE = 9;
    int SW_SHOWDEFAULT = 10;
    int SW_FORCEMINIMIZE = 11;
    int SW_MAX = 11;

    int RDW_INVALIDATE = 0x0001;
    int RDW_INTERNALPAINT = 0x0002;
    int RDW_ERASE = 0x0004;
    int RDW_VALIDATE = 0x0008;
    int RDW_NOINTERNALPAINT = 0x0010;
    int RDW_NOERASE = 0x0020;
    int RDW_NOCHILDREN = 0x0040;
    int RDW_ALLCHILDREN = 0x0080;
    int RDW_UPDATENOW = 0x0100;
    int RDW_ERASENOW = 0x0200;
    int RDW_FRAME = 0x0400;
    int RDW_NOFRAME = 0x0800;

    int GW_HWNDFIRST = 0;
    int GW_HWNDLAST = 1;
    int GW_HWNDNEXT = 2;
    int GW_HWNDPREV = 3;
    int GW_OWNER = 4;
    int GW_CHILD = 5;
    int GW_ENABLEDPOPUP = 6;
    int SWP_ASYNCWINDOWPOS = 0x4000;
    int SWP_DEFERERASE = 0x2000;
    int SWP_DRAWFRAME = 0x0020;
    int SWP_FRAMECHANGED = 0x0020;
    int SWP_HIDEWINDOW = 0x0080;
    int SWP_NOACTIVATE = 0x0010;
    int SWP_NOCOPYBITS = 0x0100;
    int SWP_NOMOVE = 0x0002;
    int SWP_NOOWNERZORDER = 0x0200;
    int SWP_NOREDRAW = 0x0008;
    int SWP_NOREPOSITION = 0x0200;
    int SWP_NOSENDCHANGING = 0x0400;
    int SWP_NOSIZE = 0x0001;
    int SWP_NOZORDER = 0x0004;
    int SWP_SHOWWINDOW = 0x0040;
    int SC_MINIMIZE = 0xF020;
    int SC_MAXIMIZE = 0xF030;
    int BS_PUSHBUTTON                  = 0x00000000;
    int BS_DEFPUSHBUTTON               = 0x00000001;
    int BS_CHECKBOX                    = 0x00000002;
    int BS_AUTOCHECKBOX                = 0x00000003;
    int BS_RADIOBUTTON                 = 0x00000004;
    int BS_3STATE                      = 0x00000005;
    int BS_AUTO3STATE                  = 0x00000006;
    int BS_GROUPBOX                    = 0x00000007;
    int BS_USERBUTTON                  = 0x00000008;
    int BS_AUTORADIOBUTTON             = 0x00000009;
    int BS_PUSHBOX                     = 0x0000000A;
    int BS_OWNERDRAW                   = 0x0000000B;
    int BS_TYPEMASK                    = 0x0000000F;
    int BS_LEFTTEXT                    = 0x00000020;

    public static class HARDWAREINPUT extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("uMsg", "wParamL", "wParamH");
        }

        public static class ByReference extends HARDWAREINPUT implements Structure.ByReference {
            public ByReference() {}

            public ByReference(Pointer memory) {
                super(memory);
            }
        }

        public HARDWAREINPUT() {}

        public HARDWAREINPUT(Pointer memory) {
            super(memory);
            read();
        }

        public DWORD uMsg;
        public WORD wParamL;
        public WORD wParamH;
    }

    public static class INPUT extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("type", "input");
        }

        public static final int INPUT_MOUSE = 0;
        public static final int INPUT_KEYBOARD = 1;
        public static final int INPUT_HARDWARE = 2;

        public static class ByReference extends INPUT implements Structure.ByReference {
            public ByReference() {}

            public ByReference(Pointer memory) {
                super(memory);
            }
        }

        public INPUT() {}

        public INPUT(Pointer memory) {
            super(memory);
            read();
        }

        public DWORD type;
        public INPUT_UNION input = new INPUT_UNION();

        public static class INPUT_UNION extends Union {
            public INPUT_UNION() {}

            public INPUT_UNION(Pointer memory) {
                super(memory);
                read();
            }

            public MOUSEINPUT mi;
            public KEYBDINPUT ki;
            public HARDWAREINPUT hi;
        }
    }

    public static class KEYBDINPUT extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("wVk", "wScan", "dwFlags", "time", "dwExtraInfo");
        }

        public static final int KEYEVENTF_EXTENDEDKEY = 0x0001;
        public static final int KEYEVENTF_KEYUP = 0x0002;
        public static final int KEYEVENTF_UNICODE = 0x0004;
        public static final int KEYEVENTF_SCANCODE = 0x0008;

        public static class ByReference extends KEYBDINPUT implements Structure.ByReference {
            public ByReference() {}

            public ByReference(Pointer memory) {
                super(memory);
            }
        }

        public KEYBDINPUT() {
        }

        public KEYBDINPUT(Pointer memory) {
            super(memory);
            read();
        }

        public WORD wVk;
        public WORD wScan;
        public DWORD dwFlags;
        public DWORD time;
        public ULONG_PTR dwExtraInfo;
    }

    public static class MOUSEINPUT extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("dx", "dy", "mouseData", "dwFlags", "time", "dwExtraInfo");
        }

        public static class ByReference extends MOUSEINPUT implements Structure.ByReference {
            public ByReference() {}

            public ByReference(Pointer memory) {
                super(memory);
            }
        }

        public MOUSEINPUT() {}

        public MOUSEINPUT(Pointer memory) {
            super(memory);
            read();
        }

        public LONG dx;
        public LONG dy;
        public DWORD mouseData;
        public DWORD dwFlags;
        public DWORD time;
        public ULONG_PTR dwExtraInfo;
    }

    public static class LASTINPUTINFO extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("cbSize", "dwTime");
        }

        public int cbSize = size();
        public int dwTime;
    }

    public class WNDCLASSEX extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("cbSize", "style", "lpfnWndProc", "cbClsExtra", "cbWndExtra",
                    "hInstance", "hIcon", "hCursor", "hbrBackground", "lpszMenuName",
                    "lpszClassName", "hIconSm");
        }

        public static class ByReference extends WNDCLASSEX implements
                Structure.ByReference {
        }

        public WNDCLASSEX() {
            super(DEFAULT);
        }

        public WNDCLASSEX(Pointer memory) {
            super(memory, ALIGN_DEFAULT, DEFAULT);
            read();
        }

        public int cbSize = this.size();
        public int style;
        public Callback lpfnWndProc;
        public int cbClsExtra;
        public int cbWndExtra;
        public HINSTANCE hInstance;
        public HICON hIcon;
        public HCURSOR hCursor;
        public HBRUSH hbrBackground;
        public String lpszMenuName;
        public String lpszClassName;
        public HICON hIconSm;
    }

    public interface WindowProc extends StdCallCallback {
        LRESULT callback(HWND hwnd, int uMsg, WPARAM wParam, LPARAM lParam);
    }

    public class HMONITOR extends HANDLE {
        public HMONITOR() {}

        public HMONITOR(Pointer p)
        {
            super(p);
        }
    }

    final int MONITOR_DEFAULTTONULL =        0x00000000;
    final int MONITOR_DEFAULTTOPRIMARY =     0x00000001;
    final int MONITOR_DEFAULTTONEAREST =     0x00000002;
    final int MONITORINFOF_PRIMARY =         0x00000001;
    final int CCHDEVICENAME =  32;

    public class MONITORINFO extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("cbSize", "rcMonitor", "rcWork", "dwFlags");
        }

        public int     cbSize = size();
        public RECT    rcMonitor;
        public RECT    rcWork;
        public int     dwFlags;
    }

    public class MONITORINFOEX extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("cbSize", "rcMonitor", "rcWork", "dwFlags", "szDevice");
        }

        public int     cbSize;
        public RECT    rcMonitor;
        public RECT    rcWork;
        public int     dwFlags;
        public char[]  szDevice;

        public MONITORINFOEX() {
            szDevice = new char[CCHDEVICENAME];
            cbSize = size();
        }
    }

    public interface MONITORENUMPROC extends StdCallCallback
    {
        public int apply(HMONITOR hMonitor, HDC hdcMonitor, RECT lprcMonitor, LPARAM dwData);
    }

    int EWX_HYBRID_SHUTDOWN = 0x00400000;
    int EWX_LOGOFF = 0;
    int EWX_POWEROFF = 0x00000008;
    int EWX_REBOOT = 0x00000002;
    int EWX_RESTARTAPPS = 0x00000040;
    int EWX_SHUTDOWN = 0x00000001;
    int EWX_FORCE = 0x00000004;
    int EWX_FORCEIFHUNG = 0x00000010;
    int GA_PARENT = 1;
    int GA_ROOT = 2;
    int GA_ROOTOWNER = 3;
    int GCW_ATOM = -32;
    int GCL_HICON = -14;
    int GCL_HICONSM = -34;
    int GCL_CBCLSEXTRA = -20;
    int GCL_CBWNDEXTRA = -18;
    int GCLP_HBRBACKGROUND = -10;
    int GCLP_HCURSOR = -12;
    int GCLP_HICON = -14;
    int GCLP_HICONSM = -34;
    int GCLP_HMODULE = -16;
    int GCLP_MENUNAME = -8;
    int GCL_STYLE = -26;
    int GCLP_WNDPROC = -24;
    int SMTO_ABORTIFHUNG = 0x0002;
    int SMTO_BLOCK = 0x0001;
    int SMTO_NORMAL = 0x0000;
    int SMTO_NOTIMEOUTIFNOTHUNG = 0x0008;
    int SMTO_ERRORONEXIT=0x0020;
    int IDC_APPSTARTING = 32650;
    int IDC_ARROW = 32512;
    int IDC_CROSS = 32515;
    int IDC_HAND = 32649;
    int IDC_HELP = 32651;
    int IDC_IBEAM = 32513;
    int IDC_NO = 32648;
    int IDC_SIZEALL = 32646;
    int IDC_SIZENESW = 32643;
    int IDC_SIZENS = 32645;
    int IDC_SIZENWSE = 32642;
    int IDC_SIZEWE = 32644;
    int IDC_UPARROW = 32516;
    int IDC_WAIT = 32514;
    int IDI_APPLICATION = 32512;
    int IDI_ASTERISK = 32516;
    int IDI_EXCLAMATION = 32515;
    int IDI_HAND = 32513;
    int IDI_QUESTION = 32514;
    int IDI_WINLOGO = 32517;
    int RIM_TYPEMOUSE =  0;
    int RIM_TYPEKEYBOARD = 1;
    int RIM_TYPEHID = 2;

    public class RAWINPUTDEVICELIST extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("hDevice", "dwType");
        }

        public HANDLE hDevice;
        public int dwType;

        public RAWINPUTDEVICELIST() {
            super();
        }

        public RAWINPUTDEVICELIST(Pointer p) {
            super(p);
        }

        public int sizeof() {
            return calculateSize(false);
        }

        @Override
        public String toString() {
            return "hDevice=" + hDevice + ", dwType=" + dwType;
        }
    }

    public int CF_BITMAT = 2;
    public int CF_DIB = 8;
    public int CF_DIBV5 = 17;
    public int CF_DIF = 5;
    public int CF_DSPBITMAP = 0x0082;
    public int CF_DSPENHMETAFILE = 0x008E;
    public int CF_DSPMETAFILEPICT = 0x0083;
    public int CF_DSPTEXT = 0x0081;
    public int CF_ENHMETAFILE = 14;
    public int CF_GDIOBJFIRST = 0x0300;
    public int CF_GDIOBJLAST = 0x03FF;
    public int CF_HDROP = 15;
    public int CF_LOCALE = 16;
    public int CF_METAFILEPICT = 3;
    public int CF_OEMTEXT = 7;
    public int CF_OWNERDISPLAY = 0x0080;
    public int CF_PALETTE = 9;
    public int CF_PENDATA = 10;
    public int CF_PRIVATEFIRST = 0x0200;
    public int CF_PRIVATELAST = 0x02FF;
    public int CF_RIFF = 11;
    public int CF_SYLK = 4;
    public int CF_TEXT = 1;
    public int CF_TIFF = 6;
    public int CF_UNICODETEXT = 13;
    public int CF_WAVE = 12;
    int MAPVK_VK_TO_VSC = 0;
    int MAPVK_VSC_TO_VK = 1;
    int MAPVK_VK_TO_CHAR = 2;
    int MAPVK_VSC_TO_VK_EX = 3;
    int MAPVK_VK_TO_VSC_EX = 4;
    int KL_NAMELENGTH = 9;
    int MODIFIER_SHIFT_MASK = 1;
    int MODIFIER_CTRL_MASK = 2;
    int MODIFIER_ALT_MASK = 4;
    int MODIFIER_HANKAKU_MASK = 8;
    int MODIFIER_RESERVED1_MASK = 16;
    int MODIFIER_RESERVED2_MASK = 32;
}