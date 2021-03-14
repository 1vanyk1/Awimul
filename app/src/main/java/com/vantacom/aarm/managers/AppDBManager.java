package com.vantacom.aarm.managers;

import android.content.Context;
import android.database.Cursor;
import android.database.sqlite.SQLiteDatabase;

public class AppDBManager {
    private String DBName = "app.db";
    private SQLiteDatabase db;

    private static AppDBManager dbManager;

    public static AppDBManager getInstance(Context context) {
        if (dbManager == null) {
            dbManager = new AppDBManager(context);
        }
        return dbManager;
    }

    private AppDBManager(Context context) {
        db = context.openOrCreateDatabase(DBName, Context.MODE_PRIVATE, null);
        createTables();
    }

    private void createTables() {
        db.execSQL("CREATE TABLE IF NOT EXISTS AppInfo (name VARCHAR NOT NULL UNIQUE, value VARCHAR);");
        Cursor cursor = db.rawQuery("SELECT * FROM AppInfo WHERE name = 'firstLoad';", null);
        boolean existence = cursor.moveToFirst();
        if (!existence) {
            db.execSQL("INSERT INTO AppInfo VALUES ('firstLoad', '1');");
        }
        cursor.close();
    }

    public boolean isBool(String name) {
        Cursor cursor = db.rawQuery("SELECT * FROM AppInfo WHERE name = '" + name + "';", null);
        boolean line = cursor.moveToFirst();
        if (!line) { return false; }
        String value = cursor.getString(cursor.getColumnIndex("value"));
        cursor.close();
        return value.equals("1");
    }

    public void setBool(String name, boolean value) {
        String res;
        if (value) {    res = "1";
        } else {        res = "0";}
        db.execSQL("UPDATE AppInfo SET value='" + res + "' WHERE name = '" + name + "';");
    }
}
