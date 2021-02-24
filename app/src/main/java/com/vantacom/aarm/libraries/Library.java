package com.vantacom.aarm.libraries;

import com.vantacom.aarm.wine.WineActivity;

import java.io.File;

public interface Library {
    public void init(WineActivity activity, File libraryDir);

    public void destroy() throws Throwable;
}
