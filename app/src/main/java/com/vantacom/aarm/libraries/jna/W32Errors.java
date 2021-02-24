package com.vantacom.aarm.libraries.jna;

import com.vantacom.aarm.libraries.jna.WinNT.HRESULT;

public abstract class W32Errors implements WinError {
    public static final boolean SUCCEEDED(int hr) {
        return hr >= 0;
    }

    public static final boolean FAILED(int hr) {
        return hr < 0;
    }

    public static final boolean SUCCEEDED(HRESULT hr) {
        return hr == null || SUCCEEDED(hr.intValue());
    }

    public static final boolean FAILED(HRESULT hr) {
        return hr != null && FAILED(hr.intValue());
    }

    public static final int HRESULT_CODE(int hr) {
        return hr & 0xFFFF;
    }

    public static final int SCODE_CODE(int sc) {
        return sc & 0xFFFF;
    }

    public static final int HRESULT_FACILITY(int hr) {
        return (hr >>= 16) & 0x1fff;
    }

    public static final int SCODE_FACILITY(short sc) {
        return (sc >>= 16) & 0x1fff;
    }

    public static short HRESULT_SEVERITY(int hr) {
        return (short) ((hr >>= 31) & 0x1);
    }

    public static short SCODE_SEVERITY(short sc) {
        return (short) ((sc >>= 31) & 0x1);
    }

    public static int MAKE_HRESULT(short sev, short fac, short code) {
        return ((sev << 31) | (fac << 16) | code);
    }

    public static final int MAKE_SCODE(short sev, short fac, short code) {
        return ((sev << 31) | (fac << 16) | code);
    }

    public static final HRESULT HRESULT_FROM_WIN32(int x) {
        int f = FACILITY_WIN32;
        return new HRESULT(x <= 0 ? x : ((x) & 0x0000FFFF) | (f <<= 16)
                | 0x80000000);
    }

    public static final int FILTER_HRESULT_FROM_FLT_NTSTATUS(int x) {
        int f = FACILITY_USERMODE_FILTER_MANAGER;
        return (((x) & 0x8000FFFF) | (f <<= 16));
    }

}