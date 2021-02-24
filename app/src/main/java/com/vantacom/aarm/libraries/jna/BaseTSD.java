package com.vantacom.aarm.libraries.jna;

import com.sun.jna.IntegerType;
import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.ByReference;

public interface BaseTSD {
    public static class LONG_PTR extends IntegerType {
        public LONG_PTR() {
            this(0);
        }

        public LONG_PTR(long value) {
            super(Native.POINTER_SIZE, value);
        }

        public Pointer toPointer() {
            return Pointer.createConstant(longValue());
        }
    }

    public static class SSIZE_T extends LONG_PTR {
        public SSIZE_T() {
            this(0);
        }

        public SSIZE_T(long value) {
            super(value);
        }
    }

    public static class ULONG_PTR extends IntegerType {
        public ULONG_PTR() {
            this(0);
        }

        public ULONG_PTR(long value) {
            super(Native.POINTER_SIZE, value, true);
        }

        public Pointer toPointer() {
            return Pointer.createConstant(longValue());
        }
    }

    public static class ULONG_PTRByReference extends ByReference {
        public ULONG_PTRByReference() {
            this(new ULONG_PTR(0));
        }
        public ULONG_PTRByReference(ULONG_PTR value) {
            super(Native.POINTER_SIZE);
            setValue(value);
        }
        public void setValue(ULONG_PTR value) {
            if (Native.POINTER_SIZE == 4) {
                getPointer().setInt(0, value.intValue());
            }
            else {
                getPointer().setLong(0, value.longValue());
            }
        }
        public ULONG_PTR getValue() {
            return new ULONG_PTR(Native.POINTER_SIZE == 4
                    ? getPointer().getInt(0)
                    : getPointer().getLong(0));
        }
    }

    public static class DWORD_PTR extends IntegerType {
        public DWORD_PTR() {
            this(0);
        }

        public DWORD_PTR(long value) {
            super(Native.POINTER_SIZE, value);
        }
    }

    public static class SIZE_T extends ULONG_PTR {
        public SIZE_T() {
            this(0);
        }

        public SIZE_T(long value) {
            super(value);
        }
    }
}