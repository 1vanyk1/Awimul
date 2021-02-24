package com.vantacom.aarm;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

import com.vantacom.aarm.dialogs.LoadingDialog;
import com.vantacom.aarm.dialogs.NoObbDialog;
import com.vantacom.aarm.managers.FileManager;
import com.vantacom.aarm.managers.SaveFilesManager;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    private Button generate;
    private Button load;
    private Intent wineActivity;
    private SaveFilesManager saveFilesManager;
    public static String PACKAGE_NAME = "com.vantacom.aarm";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        System.setProperty("jna.debug_load", "true");
        System.setProperty("jna.dump_memory", "true");
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        if(Build.VERSION.SDK_INT>22){
            requestPermissions(new String[] {"android.permission.WRITE_EXTERNAL_STORAGE"}, 1);
            requestPermissions(new String[] {"android.permission.READ_EXTERNAL_STORAGE"}, 1);
            requestPermissions(new String[] {"android.permission.INTERNET"}, 1);
            requestPermissions(new String[] {"android.permission.RECORD_AUDIO"}, 1);
        }
        saveFilesManager = new SaveFilesManager(this, "app");
        generate = findViewById(R.id.generate);
        generate.setOnClickListener(this);
        load = findViewById(R.id.load);
        load.setOnClickListener(this);
        PACKAGE_NAME = getApplicationContext().getPackageName();
    }

    @Override
    protected void onResume() {
        super.onResume();
        if ((ContextCompat.checkSelfPermission( this, android.Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED ) && (ContextCompat.checkSelfPermission( this, Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED )) {
            return;
        }
        if (saveFilesManager.getIsFirstLoad()) {
            if (FileManager.checkAPKExpansionFiles(this)) {
                LoadingDialog progressDialog = new LoadingDialog(this);
                progressDialog.execute(this);
                saveFilesManager.setIsFirstLoad(false);
            } else {
                NoObbDialog dialog = new NoObbDialog(this);
                FragmentManager manager = getSupportFragmentManager();
                FragmentTransaction transaction = manager.beginTransaction();
                dialog.show(transaction, "dialog");
            }
        }
    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.generate) {
            LoadingDialog progressDialog = new LoadingDialog(this);
            progressDialog.execute(this);
        } else if (v.getId() == R.id.load) {
            wineActivity = new Intent(MainActivity.this, com.vantacom.aarm.wine.WineActivity.class);
            wineActivity.putExtra("width", 1280);
            wineActivity.putExtra("height", 720);
            startActivity(wineActivity);
        }
    }
}