package com.vantacom.aarm.managers;

import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;
import android.util.Log;

import java.util.ArrayList;

public class PackageDBManager {
    private String DBName = "packages.db";
    private SQLiteDatabase db;

    private static PackageDBManager dbManager;

    public static PackageDBManager getInstance(Context context) {
        if (dbManager == null) {
            dbManager = new PackageDBManager(context);
        }
        return dbManager;
    }

    private PackageDBManager(Context context) {
        db = context.openOrCreateDatabase(DBName, Context.MODE_PRIVATE, null);
        createTables();
    }

    private void createTables() {
        db.execSQL("CREATE TABLE IF NOT EXISTS PackageInfo (id INTEGER PRIMARY KEY AUTOINCREMENT, name VARCHAR NOT NULL UNIQUE, prefix VARCHAR NOT NULL, firstLoad VARCHAR, size VARCHAR, colorDepth INTEGER, workInBackground VARCHAR default '0', showCursor VARCHAR default '0');");
        Cursor cursor;
        cursor = db.rawQuery("SELECT * FROM PackageInfo LIMIT 0,1", null);
        if (cursor.getColumnIndex("workInBackground") == -1) {
            db.execSQL("ALTER TABLE PackageInfo ADD COLUMN workInBackground VARCHAR default '0';");
        }
        if (cursor.getColumnIndex("showCursor") == -1) {
            db.execSQL("ALTER TABLE PackageInfo ADD COLUMN showCursor VARCHAR default '0';");
        }
    }

    public void addPackage() {
        Cursor cursor = db.rawQuery("SELECT * FROM PackageInfo;", null);
        boolean line = cursor.moveToFirst();
        if (!line) {
            db.execSQL("INSERT INTO PackageInfo(name, prefix, firstLoad, size, colorDepth, workInBackground, showCursor) VALUES('prefix0', 'prefix0', '1', 'native', 32, '0', '0');");
        } else {
            cursor.moveToLast();
            int num = Integer.parseInt(cursor.getString(cursor.getColumnIndex("prefix")).substring(6));
            while (db.rawQuery("SELECT * FROM PackageInfo WHERE name = 'prefix" + (num + 1) + "';", null).moveToFirst()) {
                num++;
            }
            db.execSQL("INSERT INTO PackageInfo(name, prefix, firstLoad, size, colorDepth, workInBackground, showCursor) VALUES('prefix" + (num + 1) + "', 'prefix" + (num + 1) + "', '1', 'native', 32, '0', '0');");
        }
    }

    public void deletePackage(String name) {
        db.execSQL("DELETE FROM PackageInfo WHERE name = '" + name + "';");
    }

    public boolean isBool(String name, String string) {
        Cursor cursor = db.rawQuery("SELECT * FROM PackageInfo WHERE name = '" + name + "';", null);
        boolean line = cursor.moveToFirst();
        if (!line) { return false; }
        String value = cursor.getString(cursor.getColumnIndex(string));
        cursor.close();
        return value.equals("1");
    }

    public void setBool(String name, String string, boolean value) {
        String res;
        if (value) {    res = "1";
        } else {        res = "0";}
        db.execSQL("UPDATE PackageInfo SET " + string + "='" + res + "' WHERE name = '" + name + "';");
    }

    public String getString(String name, String string) {
        Cursor cursor = db.rawQuery("SELECT * FROM PackageInfo WHERE name = '" + name + "';", null);
        boolean line = cursor.moveToFirst();
        if (!line) { return ""; }
        String value = cursor.getString(cursor.getColumnIndex(string));
        cursor.close();
        return value;
    }

    public void setString(String name, String string, String value) {
        db.execSQL("UPDATE PackageInfo SET " + string + "='" + value + "' WHERE name = '" + name + "';");
    }

    public int getInt(String name, String string) {
        Cursor cursor = db.rawQuery("SELECT * FROM PackageInfo WHERE name = '" + name + "';", null);
        boolean line = cursor.moveToFirst();
        if (!line) { return 0; }
        int value = Integer.parseInt(cursor.getString(cursor.getColumnIndex(string)));
        cursor.close();
        return value;
    }

    public void setInt(String name, String string, int value) {
        db.execSQL("UPDATE PackageInfo SET " + string + "=" + value + " WHERE name = '" + name + "';");
    }

    public void changeSize(String name, int width, int height) {
        setString(name, "size", width + "x" + height);
    }

    public ArrayList<String> getPackageNames() {
        ArrayList<String> names = new ArrayList<String>();
        Cursor cursor = db.rawQuery("SELECT * FROM PackageInfo", null);
        boolean line = cursor.moveToFirst();
        while (line) {
            names.add(cursor.getString(cursor.getColumnIndex("name")));
            line = cursor.moveToNext();
        }
        return names;
    }
}
