package com.vantacom.aarm;

import android.util.Log;

import java.lang.reflect.Method;

public class CustomClassManager {
    private CustomClassLoader ccl;
    private Class pClass;
    private Object object;

    public CustomClassManager(String className) throws ClassNotFoundException, InstantiationException, IllegalAccessException {
        ccl = new CustomClassLoader(CustomClassManager.class.getClassLoader());
        pClass = ccl.loadClass(className);
        object = pClass.newInstance();
    }

    private Class getClass(Class lClass) {
        switch (lClass.getName()) {
            default: return lClass;
            case "java.lang.Integer": return int.class;
            case "java.lang.Boolean": return boolean.class;
        }
    }

    public Object invoke(String methodName, Object ... args) {
        try {
            Class[] cArgs = new Class[args.length];
            for (int i = 0; i < args.length; i++) {
                cArgs[i] = getClass(args[i].getClass());
            }
            Method method = pClass.getMethod(methodName, cArgs);
            Object res;
            res = method.invoke(object, args);
            method = null;
            return res;
        } catch (Exception e) {
            Log.e("CCL/invoke", e.toString());
            return new Object[0];
        }
    }

    public void destroy() {
        try {
            invoke("destroy");
        } catch (Exception e) {
            Log.e("destroy()", e.toString());
        }
        pClass = null;
        object = null;
        try {
            ccl.destroy();
        } catch (Exception e) {
            Log.e("destroy()", e.toString());
        }
        ccl = null;
        System.gc();
        System.runFinalization();
    }
}
