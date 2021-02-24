package com.vantacom.aarm.libraries.jna;

import com.sun.jna.LastErrorException;
import com.vantacom.aarm.libraries.jna.WinNT.HRESULT;

import java.lang.reflect.InvocationTargetException;
import java.lang.reflect.Method;
import java.util.logging.Level;
import java.util.logging.Logger;

public class Win32Exception extends LastErrorException {

    private static final long serialVersionUID = 1L;

    private HRESULT _hr;

    public HRESULT getHR() {
        return _hr;
    }

    public Win32Exception(int code) {
        this(code, W32Errors.HRESULT_FROM_WIN32(code));
    }

    public Win32Exception(HRESULT hr) {
        this(W32Errors.HRESULT_CODE(hr.intValue()), hr);
    }

    protected Win32Exception(int code, HRESULT hr) {
        this(code, hr, Kernel32Util.formatMessage(hr));
    }

    protected Win32Exception(int code, HRESULT hr, String msg) {
        super(code, msg);
        _hr = hr;
    }

    private static Method addSuppressedMethod = null;
    static {
        try {
            addSuppressedMethod = Throwable.class.getMethod("addSuppressed", Throwable.class);
        } catch (NoSuchMethodException ex) {
        } catch (SecurityException ex) {
            Logger.getLogger(Win32Exception.class.getName()).log(Level.SEVERE, "Failed to initialize 'addSuppressed' method", ex);
        }
    }

    void addSuppressedReflected(Throwable exception) {
        if(addSuppressedMethod == null) {
            return;
        }
        try {
            addSuppressedMethod.invoke(this, exception);
        } catch (IllegalAccessException ex) {
            throw new RuntimeException("Failed to call addSuppressedMethod", ex);
        } catch (IllegalArgumentException ex) {
            throw new RuntimeException("Failed to call addSuppressedMethod", ex);
        } catch (InvocationTargetException ex) {
            throw new RuntimeException("Failed to call addSuppressedMethod", ex);
        }
    }
}