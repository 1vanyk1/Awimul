package com.vantacom.aarm.managers;

import android.util.Log;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class ConsoleManager {
    public static void runCommand(String line) {
        try {
            Process process = Runtime.getRuntime().exec(line);
            process.waitFor();
        } catch (IOException e) {
            Log.e("$", e.toString());
        } catch (InterruptedException e) {
            Log.e("$", e.toString());
        }
    }

    public static String runCommandWithLog(String line) {
        try {
            Process process = Runtime.getRuntime().exec(line);
            BufferedReader reader = new BufferedReader(new InputStreamReader(process.getInputStream()));
            int read;
            char[] buffer = new char[4096];
            StringBuffer output = new StringBuffer();
            while ((read = reader.read(buffer)) > 0) {
                output.append(buffer, 0, read);
            }
            reader.close();

            process.waitFor();

            return output.toString();
        } catch (IOException e) {
            Log.e("$", e.toString());
        } catch (InterruptedException e) {
            Log.e("$", e.toString());
        }
        return "";
    }
}
