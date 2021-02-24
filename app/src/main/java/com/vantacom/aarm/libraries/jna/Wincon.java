package com.vantacom.aarm.libraries.jna;

import com.sun.jna.Structure;
import com.sun.jna.Union;
import com.sun.jna.ptr.IntByReference;

import java.util.Arrays;
import java.util.List;

public interface Wincon {
    boolean AllocConsole();

    boolean FreeConsole();

    int ATTACH_PARENT_PROCESS=(-1);

    boolean AttachConsole(int dwProcessId);

    boolean FlushConsoleInputBuffer(WinNT.HANDLE hConsoleInput);

    int CTRL_C_EVENT=0;
    int CTRL_BREAK_EVENT=1;

    boolean GenerateConsoleCtrlEvent(int dwCtrlEvent, int dwProcessGroupId);

    int GetConsoleCP();

    boolean SetConsoleCP(int wCodePageID);

    int GetConsoleOutputCP();

    boolean SetConsoleOutputCP(int wCodePageID);

    WinDef.HWND GetConsoleWindow();

    boolean GetNumberOfConsoleInputEvents(WinNT.HANDLE hConsoleInput, IntByReference lpcNumberOfEvents);

    boolean GetNumberOfConsoleMouseButtons(IntByReference lpNumberOfMouseButtons);

    int STD_INPUT_HANDLE=(-10);
    int STD_OUTPUT_HANDLE=(-11);
    int STD_ERROR_HANDLE=(-12);

    WinNT.HANDLE GetStdHandle(int nStdHandle);

    boolean SetStdHandle(int nStdHandle, WinNT.HANDLE hHandle);

    int CONSOLE_FULLSCREEN=1;
    int CONSOLE_FULLSCREEN_HARDWARE=2;

    boolean GetConsoleDisplayMode(IntByReference lpModeFlags);

    int ENABLE_PROCESSED_INPUT=0x0001;
    int ENABLE_LINE_INPUT=0x0002;
    int ENABLE_ECHO_INPUT=0x0004;
    int ENABLE_WINDOW_INPUT=0x0008;
    int ENABLE_MOUSE_INPUT=0x0010;
    int ENABLE_INSERT_MODE=0x0020;
    int ENABLE_QUICK_EDIT_MODE=0x0040;
    int ENABLE_EXTENDED_FLAGS=0x0080;
    int ENABLE_VIRTUAL_TERMINAL_PROCESSING = 0x0004;
    int DISABLE_NEWLINE_AUTO_RETURN = 0x0008;
    int ENABLE_VIRTUAL_TERMINAL_INPUT = 0x0200;

    int ENABLE_PROCESSED_OUTPUT=0x0001;
    int ENABLE_WRAP_AT_EOL_OUTPUT=0x0002;

    boolean GetConsoleMode(WinNT.HANDLE hConsoleHandle, IntByReference lpMode);

    boolean SetConsoleMode(WinNT.HANDLE hConsoleHandle, int dwMode);

    int MAX_CONSOLE_TITLE_LENGTH=64 * 1024;

    int GetConsoleTitle(char[] lpConsoleTitle, int nSize);

    int GetConsoleOriginalTitle(char[] lpConsoleTitle, int nSize);

    boolean SetConsoleTitle(String lpConsoleTitle);

    boolean GetConsoleScreenBufferInfo(WinNT.HANDLE hConsoleOutput, CONSOLE_SCREEN_BUFFER_INFO lpConsoleScreenBufferInfo);

    boolean ReadConsoleInput(WinNT.HANDLE hConsoleInput, INPUT_RECORD[] lpBuffer, int nLength, IntByReference lpNumberOfEventsRead);

    boolean WriteConsole(WinNT.HANDLE hConsoleOutput, String lpBuffer, int nNumberOfCharsToWrite, IntByReference lpNumberOfCharsWritten, WinDef.LPVOID lpReserved);

    public static class COORD extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("X", "Y");
        }

        public short X;
        public short Y;

        @Override
        public String toString() {
            return String.format("COORD(%s,%s)", X, Y);
        }
    }

    public static class SMALL_RECT extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("Left", "Top", "Right", "Bottom");
        }

        public short Left;
        public short Top;
        public short Right;
        public short Bottom;

        @Override
        public String toString() {
            return String.format("SMALL_RECT(%s,%s)(%s,%s)", Left, Top, Right, Bottom);
        }
    }

    public static class CONSOLE_SCREEN_BUFFER_INFO extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("dwSize", "dwCursorPosition", "wAttributes", "srWindow", "dwMaximumWindowSize");
        }

        public COORD dwSize;
        public COORD dwCursorPosition;
        public short wAttributes;
        public SMALL_RECT srWindow;
        public COORD dwMaximumWindowSize;

        @Override
        public String toString() {
            return String.format("CONSOLE_SCREEN_BUFFER_INFO(%s,%s,%s,%s,%s)", dwSize, dwCursorPosition, wAttributes, srWindow, dwMaximumWindowSize);
        }
    }

    public static class INPUT_RECORD extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("EventType", "Event");
        }

        public static final short KEY_EVENT = 0x01;
        public static final short MOUSE_EVENT = 0x02;
        public static final short WINDOW_BUFFER_SIZE_EVENT = 0x04;

        public short EventType;
        public Event Event;

        public static class Event extends Union {
            public KEY_EVENT_RECORD KeyEvent;
            public MOUSE_EVENT_RECORD MouseEvent;
            public WINDOW_BUFFER_SIZE_RECORD WindowBufferSizeEvent;
        }

        @Override
        public void read() {
            super.read();
            switch (EventType) {
                case KEY_EVENT:
                    Event.setType("KeyEvent");
                    break;
                case MOUSE_EVENT:
                    Event.setType("MouseEvent");
                    break;
                case WINDOW_BUFFER_SIZE_EVENT:
                    Event.setType("WindowBufferSizeEvent");
                    break;
            }
            Event.read();
        }

        @Override
        public String toString() {
            return String.format("INPUT_RECORD(%s)", EventType);
        }
    }

    public static class KEY_EVENT_RECORD extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("bKeyDown", "wRepeatCount", "wVirtualKeyCode", "wVirtualScanCode", "uChar", "dwControlKeyState");
        }

        public boolean bKeyDown;
        public short wRepeatCount;
        public short wVirtualKeyCode;
        public short wVirtualScanCode;
        public char uChar;
        public int dwControlKeyState;

        @Override
        public String toString() {
            return String.format("KEY_EVENT_RECORD(%s,%s,%s,%s,%s,%s)", bKeyDown, wRepeatCount, wVirtualKeyCode, wVirtualKeyCode, wVirtualScanCode, uChar, dwControlKeyState);
        }
    }

    public static class MOUSE_EVENT_RECORD extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("dwMousePosition", "dwButtonState", "dwControlKeyState", "dwEventFlags");
        }

        public COORD dwMousePosition;
        public int dwButtonState;
        public int dwControlKeyState;
        public int dwEventFlags;

        @Override
        public String toString() {
            return String.format("MOUSE_EVENT_RECORD(%s,%s,%s,%s)", dwMousePosition, dwButtonState, dwControlKeyState, dwEventFlags);
        }
    }

    public static class WINDOW_BUFFER_SIZE_RECORD extends Structure {
        @Override
        protected List<String> getFieldOrder() {
            return Arrays.asList("dwSize");
        }

        public COORD dwSize;

        @Override
        public String toString() {
            return String.format("WINDOW_BUFFER_SIZE_RECORD(%s)", dwSize);
        }
    }
}