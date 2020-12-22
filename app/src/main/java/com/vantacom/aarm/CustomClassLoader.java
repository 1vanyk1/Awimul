package com.vantacom.aarm;

import java.io.DataInputStream;
import java.io.File;
import java.io.IOException;
import java.io.InputStream;
import java.util.HashMap;
import java.util.Map;

public class CustomClassLoader extends ClassLoader {
    private Map<String, Class<?>> classes = new HashMap<String, Class<?>>();

    public CustomClassLoader(ClassLoader parent) {
        super(parent);
    }

    private Class getClass(String name) throws ClassNotFoundException {
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
        System.out.println("Loading Class '" + name + "'");
        if (name.startsWith("com.journaldev")) {
            System.out.println("Loading Class using CCLoader");
            c = getClass(name);
        } else {
            c = super.loadClass(name);
        }
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