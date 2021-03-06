package com.vantacom.aarm.libraries.jna;

import com.sun.jna.Native;
import com.sun.jna.Pointer;
import com.sun.jna.ptr.ByReference;
import com.vantacom.aarm.libraries.jna.WinNT.HANDLE;

public interface WinReg {

    public static class HKEY extends HANDLE {
        public HKEY() { }
        public HKEY(Pointer p) { super(p); }
        public HKEY(int value) { super(new Pointer(value)); }
    }

    public static class HKEYByReference extends ByReference {
        public HKEYByReference() {
            this(null);
        }

        public HKEYByReference(HKEY h) {
            super(Native.POINTER_SIZE);
            setValue(h);
        }

        public void setValue(HKEY h) {
            getPointer().setPointer(0, h != null ? h.getPointer() : null);
        }

        public HKEY getValue() {
            Pointer p = getPointer().getPointer(0);
            if (p == null)
                return null;
            if (WinBase.INVALID_HANDLE_VALUE.getPointer().equals(p))
                return (HKEY) WinBase.INVALID_HANDLE_VALUE;
            HKEY h = new HKEY();
            h.setPointer(p);
            return h;
        }
    }

    HKEY HKEY_CLASSES_ROOT = new HKEY(0x80000000);
    HKEY HKEY_CURRENT_USER = new HKEY(0x80000001);
    HKEY HKEY_LOCAL_MACHINE = new HKEY(0x80000002);
    HKEY HKEY_USERS = new HKEY(0x80000003);
    HKEY HKEY_PERFORMANCE_DATA= new HKEY(0x80000004);
    HKEY HKEY_PERFORMANCE_TEXT= new HKEY(0x80000050);
    HKEY HKEY_PERFORMANCE_NLSTEXT = new HKEY(0x80000060);
    HKEY HKEY_CURRENT_CONFIG  = new HKEY(0x80000005);
    HKEY HKEY_DYN_DATA = new HKEY(0x80000006);
}