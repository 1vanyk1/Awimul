package com.vantacom.aarm;

import android.util.Log;
import android.view.Surface;

import java.io.DataInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.lang.reflect.Method;
import java.util.HashMap;
import java.util.Map;

public class LibraryManager {
    private CustomClassLoader ccl;
    private Class pClass;
    private Object object;

    public LibraryManager(String className) throws ClassNotFoundException, InstantiationException, IllegalAccessException {
        ccl = new CustomClassLoader(LibraryManager.class.getClassLoader());
        pClass = ccl.loadClass(className);
        object = pClass.newInstance();
    }

    private Class getClass(Class lClass) {
        switch (lClass.getName()) {
            default: return lClass;
            case "java.lang.Integer": return int.class;
            case "java.lang.Boolean": return boolean.class;
            case "java.lang.Byte": return byte.class;
        }
    }

    public Class[] getMethodArgs(String method) {
        switch (method) {
            default: return null;
            case "wine_surface_changed": return new Class[] {int.class, Surface.class, boolean.class};
        }
    }

    public Object invoke(String methodName, Object ... args) {
        try {
            Class[] cArgs = getMethodArgs(methodName);
            if (cArgs == null) {
                cArgs = new Class[args.length];
                for (int i = 0; i < args.length; i++) {
                    if (args[i] == null) {
                        cArgs[i] = null;
                    } else {
                        cArgs[i] = getClass(args[i].getClass());
                    }
                }
            }
            Method method = pClass.getMethod(methodName, cArgs);
            Object res;
            res = method.invoke(object, args);
            method = null;
            return res;
        } catch (Exception e) {
            Log.e("LM/invoke", e.toString());
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

    class CustomClassLoader extends ClassLoader {
        private Map<String, Class<?>> classes = new HashMap<String, Class<?>>();

        public CustomClassLoader(ClassLoader parent) {
            super(parent);
        }

        private Class getClass(String name) {
            String file = name.replace('.', File.separatorChar) + ".class";
            byte[] b = null;
            try {
                b = loadClassFileData(file);
                Class c = defineClass(name, b, 0, b.length);
                resolveClass(c);
                return c;
            } catch (IOException e) {
                e.printStackTrace();
                return null;
            }
        }

        @Override
        public Class loadClass(String name) throws ClassNotFoundException {
            Class c;
            c = super.loadClass(name);
            classes.put(name, c);
            return c;
        }

        private byte[] loadClassFileData(String name) throws IOException {
            InputStream stream = getClass().getClassLoader().getResourceAsStream(
                    name);
            int size = stream.available();
            byte buff[] = new byte[size];
            DataInputStream in = new DataInputStream(stream);
            in.readFully(buff);
            in.close();
            return buff;
        }

        public void destroy() {
            classes.clear();
        }
    }
}
