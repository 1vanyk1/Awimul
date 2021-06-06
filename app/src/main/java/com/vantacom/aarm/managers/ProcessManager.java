package com.vantacom.aarm.managers;

import android.util.Log;

import com.vantacom.aarm.ipc.IPClib;

import java.util.ArrayList;

public class ProcessManager {
    private ArrayList<String> pausedProcesses;
    private int pid_pos, name_pos;
    private boolean isPaused;

    public ProcessManager() {
        pausedProcesses = new ArrayList<String>();
        pid_pos = 1;
        name_pos = 8;
        isPaused = false;
        try {
            String[] head = ConsoleManager.runCommandWithLog("ps").split("\n")[0].split("\\s+");
            for (int i = 0; i < head.length; i++) {
                if (head[i].equals("PID")) {
                    pid_pos = i;
                } else if (head[i].equals("NAME")) {
                    name_pos = i;
                }
            }
        } catch (Exception e) {
            Log.e("ProcessManager", e.toString());
        }
    }

    public void pauseSystem() {
        if (!isPaused) {
            try {
                pausedProcesses.clear();
                String[] processes = ConsoleManager.runCommandWithLog("ps").split("\n");
                String[] process;
                isPaused = true;
                IPClib ipc = new IPClib();
                for (int i = 1; i < processes.length; i++) {
                    process = processes[i].split("\\s+");
                    if (!process[name_pos].equals("com.vantacom.aarm") && !process[name_pos].equals("ps")) {
                        pausedProcesses.add(process[pid_pos]);
                        ConsoleManager.runCommand(String.format("kill -STOP %s", process[pid_pos]));
                    }
                }
            } catch (Exception e) {
                Log.e("ProcessManager", e.toString());
            }
        }
    }

    public void resumeSystem() {
        try {
            for (int i = 0; i < pausedProcesses.size(); i++) {
                ConsoleManager.runCommand(String.format("kill -CONT %s", pausedProcesses.get(i)));
            }
        } catch (Exception e) {
            Log.e("ProcessManager", e.toString());
        } finally {
            isPaused = false;
        }
    }

    public boolean getIsPaused() { return isPaused; }
}
