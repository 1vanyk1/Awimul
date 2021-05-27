package com.vantacom.aarm.managers;

import android.util.Log;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.OutputStreamWriter;

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
        StringBuilder res = new StringBuilder();
        BufferedReader process_stdout;
        int read;
        char[] buffer = new char[4096];
        try {
            Process process = Runtime.getRuntime().exec(line);
            process_stdout = new BufferedReader(new InputStreamReader(process.getInputStream()));

            StringBuffer output = new StringBuffer();
            while ((read = process_stdout.read(buffer)) > 0) {
                output.append(buffer, 0, read);
            }
            process_stdout.close();
            res.append(output.toString()).append("\n");

            process.waitFor();
        } catch (IOException e) {
            Log.e("$", e.toString());
        } catch (InterruptedException e) {
            Log.e("$", e.toString());
        }
        return res.toString();
    }

    public static String runCommandWithErrorLog(String line) {
        StringBuilder res = new StringBuilder();
        BufferedReader process_stdout, process_stderr;
        int read;
        char[] buffer = new char[4096];
        try {
            Process process = Runtime.getRuntime().exec(line);
            process_stdout = new BufferedReader(new InputStreamReader(process.getInputStream()));
            process_stderr = new BufferedReader(new InputStreamReader(process.getErrorStream()));

            StringBuffer output = new StringBuffer();
            while ((read = process_stdout.read(buffer)) > 0) {
                output.append(buffer, 0, read);
            }
            process_stdout.close();
            res.append(output.toString()).append("\n");

            output = new StringBuffer();
            while ((read = process_stderr.read(buffer)) > 0) {
                output.append(buffer, 0, read);
            }
            process_stderr.close();
            res.append(output.toString()).append("\n");

            process.waitFor();
        } catch (IOException e) {
            Log.e("$", e.toString());
        } catch (InterruptedException e) {
            Log.e("$", e.toString());
        }
        return res.toString();
    }

    public static String runCommandWithLog(String ... lines) {
        StringBuilder res = new StringBuilder();
        Runtime runtime = Runtime.getRuntime();
        BufferedReader process_stdout, process_stderr;
        BufferedWriter process_stdin;
        StringBuffer output;
        int read;
        char[] buffer = new char[4096];
        try {
            Process process = runtime.exec(lines[0]);
            process_stdout = new BufferedReader(new InputStreamReader(process.getInputStream()));
            process_stderr = new BufferedReader(new InputStreamReader(process.getErrorStream()));
            process_stdin = new BufferedWriter(new OutputStreamWriter(process.getOutputStream()));
            for (int i = 1; i < lines.length; i++) {
                process_stdin.write(lines[i]);
                process_stdin.newLine();
                process_stdin.flush();
            }

            output = new StringBuffer();
            while ((read = process_stdout.read(buffer)) > 0) {
                output.append(buffer, 0, read);
            }
            process_stdout.close();
            res.append(output.toString()).append("\n");

            output = new StringBuffer();
            while ((read = process_stderr.read(buffer)) > 0) {
                output.append(buffer, 0, read);
            }
            process_stdout.close();
            res.append(output.toString()).append("\n");

            process.waitFor();
            process_stdout.close();
            process_stdin.close();
        } catch (IOException e) {
            Log.e("$", e.toString());
        } catch (InterruptedException e) {
            Log.e("$", e.toString());
        }
        return res.toString();
    }
}
