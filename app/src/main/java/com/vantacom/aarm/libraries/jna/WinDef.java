package com.vantacom.aarm.libraries.jna;

import com.sun.jna.IntegerType;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.PointerType;
import com.sun.jna.Structure;
import com.sun.jna.Structure.FieldOrder;
import com.sun.jna.ptr.ByReference;
import com.vantacom.aarm.libraries.jna.BaseTSD.LONG_PTR;
import com.vantacom.aarm.libraries.jna.WinNT.HANDLE;
import com.vantacom.aarm.libraries.jna.WinNT.HANDLEByReference;

@SuppressWarnings("serial")
public interface WinDef {
    int MAX_PATH = 260;

    public static class WORD extends IntegerType implements Comparable<WORD> {
        public static final int SIZE = 2;

        public WORD() {
            this(0);
        }

        public WORD(long value) {
            super(SIZE, value, true);
        }

        @Override
        public int compareTo(WORD other) {
            return compare(this, other);
        }
    }

    public class WORDByReference extends ByReference {
        public WORDByReference() {
            this(new WORD(0));
        }

        public WORDByReference(WORD value) {
            super(WORD.SIZE);
            setValue(value);
        }

        public void setValue(WORD value) {
            getPointer().setShort(0, value.shortValue());
        }

        public WORD getValue() {
            return new WORD(getPointer().getShort(0));
        }
    }

    public static class DWORD extends IntegerType implements Comparable<DWORD> {
        public static final int SIZE = 4;

        public DWORD() {
            this(0);
        }

        public DWORD(long value) {
            super(SIZE, value, true);
        }

        public WORD getLow() {
            return new WORD(longValue() & 0xFFFF);
        }

        public WORD getHigh() {
            return new WORD((longValue() >> 16) & 0xFFFF);
        }

        @Override
        public int compareTo(DWORD other) {
            return compare(this, other);
        }
    }

    public class DWORDByReference extends ByReference {
        public DWORDByReference() {
            this(new DWORD(0));
        }

        public DWORDByReference(DWORD value) {
            super(DWORD.SIZE);
            setValue(value);
        }

        public void setValue(DWORD value) {
            getPointer().setInt(0, value.intValue());
        }

        public DWORD getValue() {
            return new DWORD(getPointer().getInt(0));
        }
    }

    public static class LONG extends IntegerType implements Comparable<LONG> {
        public static final int SIZE = Native.LONG_SIZE;

        public LONG() {
            this(0);
        }

        public LONG(long value) {
            super(SIZE, value);
        }

        @Override
        public int compareTo(LONG other) {
            return compare(this, other);
        }
    }

    public class LONGByReference extends ByReference {
        public LONGByReference() {
            this(new LONG(0L));
        }

        public LONGByReference(LONG value) {
            super(LONG.SIZE);
            setValue(value);
        }

        public void setValue(LONG value) {
            getPointer().setInt(0, value.intValue());
        }

        public LONG getValue() {
            return new LONG(getPointer().getInt(0));
        }
    }

    public static class LONGLONG extends IntegerType implements Comparable<LONGLONG> {
        public static final int SIZE = Native.LONG_SIZE *2;

        public LONGLONG() {
            this(0);
        }

        public LONGLONG(long value) {
            super(8, value, false);
        }

        @Override
        public int compareTo(LONGLONG other) {
            return compare(this, other);
        }
    }

    public class LONGLONGByReference extends ByReference {
        public LONGLONGByReference() {
            this(new LONGLONG(0));
        }

        public LONGLONGByReference(LONGLONG value) {
            super(LONGLONG.SIZE);
            setValue(value);
        }

        public void setValue(LONGLONG value) {
            getPointer().setLong(0, value.longValue());
        }

        public LONGLONG getValue() {
            return new LONGLONG(getPointer().getLong(0));
        }
    }

    public static class HDC extends HANDLE {
        public HDC() {}

        public HDC(Pointer p) {
            super(p);
        }
    }

    public static class HICON extends HANDLE {
        public HICON() {}

        public HICON(HANDLE handle) {
            this(handle.getPointer());
        }

        public HICON(Pointer p) {
            super(p);
        }
    }

    public static class HCURSOR extends HICON {
        public HCURSOR() {}

        public HCURSOR(Pointer p) {
            super(p);
        }
    }

    public static class HMENU extends HANDLE {
        public HMENU() {

        }

        public HMENU(Pointer p) {
            super(p);
        }
    }

    public static class HPEN extends HANDLE {
        public HPEN() {}

        public HPEN(Pointer p) {
            super(p);
        }
    }

    public static class HRSRC extends HANDLE {
        public HRSRC() {}

        public HRSRC(Pointer p) {
            super(p);
        }
    }

    public static class HPALETTE extends HANDLE {
        public HPALETTE() {}

        public HPALETTE(Pointer p) {
            super(p);
        }
    }

    public static class HBITMAP extends HANDLE {
        public HBITMAP() {}

        public HBITMAP(Pointer p) {
            super(p);
        }
    }

    public static class HRGN extends HANDLE {
        public HRGN() {}

        public HRGN(Pointer p) {
            super(p);
        }
    }

    public static class HWND extends HANDLE {
        public HWND() {}

        public HWND(Pointer p) {
            super(p);
        }
    }

    public static class HINSTANCE extends HANDLE {}

    public static class HMODULE extends HINSTANCE {}

    public static class HFONT extends HANDLE {
        public HFONT() {}

        public HFONT(Pointer p) {
            super(p);
        }
    }

    public static class HKL extends HANDLE {
        public HKL() {}

        public HKL(Pointer p) {
            super(p);
        }

        public HKL(int i) {
            super(Pointer.createConstant(i));
        }

        public int getLanguageIdentifier() {
            return (int) (Pointer.nativeValue(getPointer()) & 0xFFFF);
        }

        public int getDeviceHandle() {
            return (int) (Pointer.nativeValue(getPointer()) >> 16 & 0xFFFF);
        }

        @Override
        public String toString() {
            return String.format("%08x", Pointer.nativeValue(getPointer()));
        }
    }

    public static class LPARAM extends LONG_PTR {
        public LPARAM() {
            this(0);
        }

        public LPARAM(long value) {
            super(value);
        }
    }

    public static class LRESULT extends LONG_PTR {
        public LRESULT() {
            this(0);
        }

        public LRESULT(long value) {
            super(value);
        }
    }

    public static class INT_PTR extends IntegerType {
        public INT_PTR() {
            super(Native.POINTER_SIZE);
        }

        public INT_PTR(long value) {
            super(Native.POINTER_SIZE, value);
        }

        public Pointer toPointer() {
            return Pointer.createConstant(longValue());
        }
    }

    public static class UINT_PTR extends IntegerType {
        public UINT_PTR() {
            super(Native.POINTER_SIZE);
        }

        public UINT_PTR(long value) {
            super(Native.POINTER_SIZE, value, true);
        }

        public Pointer toPointer() {
            return Pointer.createConstant(longValue());
        }
    }

    public static class WPARAM extends UINT_PTR {
        public WPARAM() {
            this(0);
        }

        public WPARAM(long value) {
            super(value);
        }
    }

    @FieldOrder({"left", "top", "right", "bottom"})
    public class RECT extends Structure {
        public int left;
        public int top;
        public int right;
        public int bottom;

        @Override
        public String toString() {
            return "[(" + left + "," + top + ")(" + right + "," + bottom + ")]";
        }
    }

    public static class ULONG extends IntegerType implements Comparable<ULONG> {
        public static final int SIZE = Native.LONG_SIZE;

        public ULONG() {
            this(0);
        }

        public ULONG(long value) {
            super(SIZE, value, true);
        }

        @Override
        public int compareTo(ULONG other) {
            return compare(this, other);
        }
    }

    public class ULONGByReference extends ByReference {

        public ULONGByReference() {
            this(new ULONG(0));
        }

        public ULONGByReference(ULONG value) {
            super(ULONG.SIZE);
            setValue(value);
        }

        public void setValue(ULONG value) {
            getPointer().setInt(0, value.intValue());
        }

        public ULONG getValue() {
            return new ULONG(getPointer().getInt(0));
        }
    }

    public static class ULONGLONG extends IntegerType implements Comparable<ULONGLONG> {
        public static final int SIZE = 8;

        public ULONGLONG() {
            this(0);
        }

        public ULONGLONG(long value) {
            super(SIZE, value, true);
        }

        @Override
        public int compareTo(ULONGLONG other) {
            return compare(this, other);
        }
    }

    public class ULONGLONGByReference extends ByReference {
        public ULONGLONGByReference() {
            this(new ULONGLONG(0));
        }

        public ULONGLONGByReference(ULONGLONG value) {
            super(ULONGLONG.SIZE);
            setValue(value);
        }

        public void setValue(ULONGLONG value) {
            getPointer().setLong(0, value.longValue());
        }

        public ULONGLONG getValue() {
            return new ULONGLONG(getPointer().getLong(0));
        }
    }

    public static class DWORDLONG extends IntegerType implements Comparable<DWORDLONG> {
        public static final int SIZE = 8;

        public DWORDLONG() {
            this(0);
        }

        public DWORDLONG(long value) {
            super(SIZE, value, true);
        }

        @Override
        public int compareTo(DWORDLONG other) {
            return compare(this, other);
        }
    }

    public static class HBRUSH extends HANDLE {
        public HBRUSH() {}

        public HBRUSH(Pointer p) {
            super(p);
        }
    }

    public static class ATOM extends WORD {
        public ATOM() {
            this(0);
        }

        public ATOM(long value) {
            super(value);
        }
    }

    public static class PVOID extends PointerType {
        public PVOID() {
            super();
        }

        public PVOID(Pointer pointer) {
            super(pointer);
        }
    }

    public static class LPVOID extends PointerType {
        public LPVOID() {
            super();
        }

        public LPVOID(Pointer p) {
            super(p);
        }
    }

    @FieldOrder({"x", "y"})
    public class POINT extends Structure {
        public static class ByReference extends POINT implements Structure.ByReference {
            public ByReference() {
            }

            public ByReference(Pointer memory) {
                super(memory);
            }

            public ByReference(int x, int y) {
                super(x, y);
            }

        }

        public static class ByValue extends POINT implements Structure.ByValue {
            public ByValue() {
            }

            public ByValue(Pointer memory) {
                super(memory);
            }

            public ByValue(int x, int y) {
                super(x, y);
            }

        }

        public int x;
        public int y;

        public POINT() {
            super();
        }

        public POINT(Pointer memory) {
            super(memory);
            read();
        }

        public POINT(int x, int y) {
            this.x = x;
            this.y = y;
        }
    }

    public static class USHORT extends IntegerType implements Comparable<USHORT> {
        public static final int SIZE = 2;

        public USHORT() {
            this(0);
        }

        public USHORT(long value) {
            super(SIZE, value, true);
        }

        @Override
        public int compareTo(USHORT other) {
            return compare(this, other);
        }
    }

    public class USHORTByReference extends ByReference {
        public USHORTByReference() {
            this(new USHORT(0));
        }

        public USHORTByReference(USHORT value) {
            super(USHORT.SIZE);
            setValue(value);
        }

        public USHORTByReference(short value) {
            super(USHORT.SIZE);
            setValue(new USHORT(value));
        }

        public void setValue(USHORT value) {
            getPointer().setShort(0, value.shortValue());
        }

        public USHORT getValue() {
            return new USHORT(getPointer().getShort(0));
        }
    }

    public static class SHORT extends IntegerType implements Comparable<SHORT> {
        public static final int SIZE = 2;

        public SHORT() {
            this(0);
        }

        public SHORT(long value) {
            super(SIZE, value, false);
        }

        @Override
        public int compareTo(SHORT other) {
            return compare(this, other);
        }
    }

    public static class UINT extends IntegerType implements Comparable<UINT> {
        public static final int SIZE = 4;

        public UINT() {
            this(0);
        }

        public UINT(long value) {
            super(SIZE, value, true);
        }

        @Override
        public int compareTo(UINT other) {
            return compare(this, other);
        }
    }

    public class UINTByReference extends ByReference {
        public UINTByReference() {
            this(new UINT(0));
        }

        public UINTByReference(UINT value) {
            super(UINT.SIZE);
            setValue(value);
        }

        public void setValue(UINT value) {
            getPointer().setInt(0, value.intValue());
        }

        public UINT getValue() {
            return new UINT(getPointer().getInt(0));
        }
    }

    public static class SCODE extends ULONG {
        public SCODE() {
            this(0);
        }

        public SCODE(long value) {
            super(value);
        }
    }

    public static class SCODEByReference extends ByReference {
        public SCODEByReference() {
            this(new SCODE(0));
        }

        public SCODEByReference(SCODE value) {
            super(SCODE.SIZE);
            setValue(value);
        }

        public void setValue(SCODE value) {
            getPointer().setInt(0, value.intValue());
        }

        public SCODE getValue() {
            return new SCODE(getPointer().getInt(0));
        }
    }

    public static class LCID extends DWORD {
        public LCID() {
            super(0);
        }

        public LCID(long value) {
            super(value);
        }
    }

    public static class BOOL extends IntegerType implements Comparable<BOOL> {
        public static final int SIZE = 4;

        public BOOL() {
            this(0);
        }

        public BOOL(boolean value) {
            this(value ? 1L : 0L);
        }

        public BOOL(long value) {
            super(SIZE, value, false);
            assert value == 0 || value == 1;
        }

        public boolean booleanValue() {
            if (this.intValue() > 0) {
                return true;
            } else {
                return false;
            }
        }

        @Override
        public String toString() {
            return Boolean.toString(booleanValue());
        }

        @Override
        public int compareTo(BOOL other) {
            return compare(this, other);
        }

        public static int compare(BOOL v1, BOOL v2) {
            if (v1 == v2) {
                return 0;
            } else if (v1 == null) {
                return 1;
            } else if (v2 == null) {
                return (-1);
            } else {
                return compare(v1.booleanValue(), v2.booleanValue());
            }
        }

        public static int compare(BOOL v1, boolean v2) {
            if (v1 == null) {
                return 1;
            } else {
                return compare(v1.booleanValue(), v2);
            }
        }

        public static int compare(boolean v1, boolean v2) {
            if (v1 == v2) {
                return 0;
            } else if (v1) {
                return 1;
            } else {
                return (-1);
            }
        }
    }

    public static class BOOLByReference extends ByReference {
        public BOOLByReference() {
            this(new BOOL(0));
        }

        public BOOLByReference(BOOL value) {
            super(BOOL.SIZE);
            setValue(value);
        }

        public void setValue(BOOL value) {
            getPointer().setInt(0, value.intValue());
        }

        public BOOL getValue() {
            return new BOOL(getPointer().getInt(0));
        }
    }

    public static class UCHAR extends IntegerType implements Comparable<UCHAR> {
        public static final int SIZE = 1;

        public UCHAR() {
            this(0);
        }

        public UCHAR(char ch) {
            this(ch & 0xFF);
        }

        public UCHAR(long value) {
            super(SIZE, value, true);
        }

        @Override
        public int compareTo(UCHAR other) {
            return compare(this, other);
        }
    }

    public static class BYTE extends UCHAR {
        public BYTE() {
            this(0);
        }

        public BYTE(long value) {
            super(value);
        }
    }

    public static class CHAR extends IntegerType implements Comparable<CHAR> {
        public static final int SIZE = 1;

        public CHAR() {
            this(0);
        }

        public CHAR(byte ch) {
            this(ch & 0xFF);
        }

        public CHAR(long value) {
            super(1, value, false);
        }

        @Override
        public int compareTo(CHAR other) {
            return compare(this, other);
        }
    }

    public static class CHARByReference extends ByReference {
        public CHARByReference() {
            this(new CHAR(0));
        }

        public CHARByReference(CHAR value) {
            super(CHAR.SIZE);
            setValue(value);
        }

        public void setValue(CHAR value) {
            getPointer().setByte(0, value.byteValue());
        }

        public CHAR getValue() {
            return new CHAR(getPointer().getByte(0));
        }
    }

    public static class HGLRC extends HANDLE {
        public HGLRC() {}

        public HGLRC(Pointer p) {
            super(p);
        }
    }

    public static class HGLRCByReference extends HANDLEByReference {
        public HGLRCByReference() {}

        public HGLRCByReference(HGLRC h) {
            super(h);
        }
    }
}