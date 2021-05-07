package com.vantacom.aarm.managers;

import android.app.Activity;
import android.content.Context;
import android.os.Environment;
import android.text.TextUtils;
import android.util.Log;

import com.vantacom.aarm.MainActivity;

import java.io.BufferedInputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.Vector;
import java.util.zip.ZipEntry;
import java.util.zip.ZipInputStream;

import static android.os.Environment.DIRECTORY_DOWNLOADS;

public class FileManager {
    private final static String EXP_PATH = "/Android/obb/";

    public static String[] copyAPKExpansionFiles(Context context, int mainVersion, int patchVersion, String dest) {
        String packageName = context.getPackageName();
        Vector<String> ret = new Vector<String>();
        if (Environment.getExternalStorageState().equals(Environment.MEDIA_MOUNTED)) {
            File root = Environment.getExternalStorageDirectory();
            File expPath = new File(root.toString() + EXP_PATH + packageName);
            if (expPath.exists()) {
                if (mainVersion > 0) {
                    String strMainPath = expPath + File.separator + "main." + mainVersion + "." + packageName + ".obb";
                    File main = new File(strMainPath);
                    if (main.isFile()) {
                        ret.add(strMainPath);
                        unpackZip(main, dest);
                    }
                }
                if (patchVersion > 0) {
                    String strPatchPath = expPath + File.separator + "patch." + mainVersion + "." + packageName + ".obb";
                    File main = new File(strPatchPath);
                    if (main.isFile()) {
                        ret.add(strPatchPath);
                        unpackZip(main, dest);
                    }
                }
            }
        }
        String[] retArray = new String[ret.size()];
        ret.toArray(retArray);
        return retArray;
    }

    public static boolean checkAPKExpansionFiles(Context context) {
        String packageName = context.getPackageName();
        File root = Environment.getExternalStorageDirectory();
        File expPath = new File(root.toString() + EXP_PATH + packageName);
        return expPath.exists();
    }

    public static String readFileString(File file)
    {
        try
        {
            return new BufferedReader(new InputStreamReader(new FileInputStream(file), "UTF-8")).readLine();
        }
        catch (IOException e) { return null; }
    }

    public static File createFile(String filename, Activity activity) throws IOException {
        String fileSeparator = System.getProperty("file.separator");
        String absoluteFilePath = Environment.getExternalStoragePublicDirectory(DIRECTORY_DOWNLOADS) + "/" + filename;
        File file = new File(absoluteFilePath);
        return file;
    }

    public static void copyFromAssetFolder(Context context, String source, String dest) {
        try {
            String files[] = context.getAssets().list(source);
            if (files == null) return;
            if (files.length == 0) {
                copyFromAssetFile(context, source, dest);
            } else {
                File file = new File(dest);
                file.mkdirs();
                for (String filename : files) {
                    copyFromAssetFolder(context, source + File.separator + filename, dest + File.separator + filename);
                }
            }
        } catch (IOException e) {
            Log.e("copyFromAssetFolder", e.toString());
        }
    }

    public static int getCountOfFilesZip(File source) throws IOException {
        InputStream is = new FileInputStream(source);
        ZipInputStream zis = new ZipInputStream(new BufferedInputStream(is));
        ZipEntry ze;
        int count = 0;
        while ((ze = zis.getNextEntry()) != null) {
            if (!ze.isDirectory()) {
                count += 1;
            }
        }
        return count;
    }

    public static void unpackZip(File source, String target) {
        File outFile = new File(target);
        outFile.mkdirs();
        try {
            String filename;
            InputStream is = new FileInputStream(source);
            ZipInputStream zis = new ZipInputStream(new BufferedInputStream(is));
            ZipEntry ze;
            int count;
            while ((ze = zis.getNextEntry()) != null) {
                filename = ze.getName();
                if (ze.isDirectory()) {
                    File file = new File(target + filename);
                    file.mkdirs();
                    continue;
                }
                FileOutputStream fout = new FileOutputStream(target + filename);
                byte[] buffer = new byte[65536];
                while ((count = zis.read(buffer)) != -1) {
                    fout.write(buffer, 0, count);
                }
                fout.close();
                File file = new File(target + filename);
                file.setExecutable(true);
                zis.closeEntry();
            }
            zis.close();
        } catch (IOException e) {
            Log.e("unpackZip", e.toString());
        }
    }

    public static void copyFolder(File source, File target) {
        String[] files = source.list();
        if (files == null) {
            copyFile(source, target);
        } else {
            target.mkdirs();
            for (String filename : files) {
                File srcFile = new File(source, filename);
                File destFile = new File(target, filename);
                copyFolder(srcFile, destFile);
            }
        }
    }

    public static void copyFolder(String source, String dest) {
        File srcFile = new File(source);
        File destFile = new File(dest);
        copyFolder(srcFile, destFile);
    }

    public static void copyFile(InputStream is, OutputStream os){
        try {
            byte[] buffer = new byte[65536];
            int length;
            while ((length = is.read(buffer)) > 0) {
                os.write(buffer, 0, length);
            }
            is.close();
            os.close();
        } catch (IOException e) {
            Log.e("copyFile", e.toString());
        }
    }

    public static void copyFile(File source, File dest){
        InputStream is;
        OutputStream os;
        try {
            is = new FileInputStream(source);
            os = new FileOutputStream(dest);
            copyFile(is, os);
        } catch (IOException e) {
            Log.e("copyFile", e.toString());
        }
    }

    public static void copyFile(InputStream is, File dest) {
        OutputStream os;
        try {
            os = new FileOutputStream(dest);
            copyFile(is, os);
        } catch (IOException e) {
            Log.e("copyFile", e.toString());
        }
    }

    public static void copyFile(String source, String dest) {
        try {
            File inFile = new File(source);
            FileInputStream is = new FileInputStream(inFile);
            File outFile = new File(dest);
            OutputStream os = new FileOutputStream(outFile);
            copyFile(is, os);
        } catch (IOException e) {
            Log.e("copyFile", e.toString());
        }
    }

    public static void copyFromAssetFile(Context context, String source, String dest) {
        try {
            InputStream is = context.getAssets().open(source);
            File outFile = new File(dest);
            OutputStream os = new FileOutputStream(outFile);
            copyFile(is, os);
        } catch (IOException e) {
            Log.e("copyFile", e.toString());
        }
    }

    public static String getDriveCPath(Context context, String prefix) {
        return context.getFilesDir() + String.format("/%s/drive_c", prefix);
    }

    public static String getPrefixPath(Context context, String prefix) {
        return context.getFilesDir() + String.format("/%s", prefix);
    }

    public static String getCanonicalPrefixPath(Context context, String prefix) throws IOException {
        return context.getFilesDir().getCanonicalPath() + String.format("/%s", prefix);
    }

    public static void createFile(String path, String text) {
        File saveFile = new File(path);
        try {
            FileOutputStream os = new FileOutputStream(saveFile);
            os.write(text.getBytes());
            os.close();
        } catch (Exception e) {
            Log.e("createFile", e.toString());
        }
    }

    public static String fixPWD(Context context, String pwd) {
        String packageName = MainActivity.PACKAGE_NAME;
        ArrayList<String> pwdList = new ArrayList<String>(Arrays.asList(pwd.split("/")));
        if (!pwdList.contains(packageName)) {
            return pwd.trim();
        }
        ArrayList<String> path = new ArrayList<String>(Arrays.asList(context.getFilesDir().getParent().split("/")));
        for (int i = pwdList.indexOf(packageName) + 1; i < pwdList.size(); i++) {
            path.add(pwdList.get(i));
        }
        return TextUtils.join("/", path).trim();
    }

    public static void deleteFile(String path) {
        File saveFile = new File(path);
        saveFile.delete();
    }

    public static void deleteFolder(Context context, File folder) {
        String[] files = folder.list();
        try {
            if (!folder.getCanonicalPath().contains(FileManager.getCanonicalPrefixPath(context, "prefixes"))) {
                ConsoleManager.runCommand(String.format("rm %s", folder.getAbsolutePath()));
                return;
            }
        } catch (Exception e) {}
        if (files != null) {
            for (String filename : files) {
                File srcFile = new File(folder, filename);
                deleteFolder(context, srcFile);
            }
        }
        folder.delete();
    }

    public static void deleteFolder(Context context, String folder) {
        File folderFile = new File(folder);
        deleteFolder(context, folderFile);
    }

    public static void deleteUser(Context activity, String packageName) {
        PackageDBManager packageManager = PackageDBManager.getInstance(activity);
        FileManager.deleteFolder(activity, FileManager.getPrefixPath(activity, "prefixes/" + packageManager.getString(packageName, "prefix")));
        packageManager.deletePackage(packageName);
    }
}
