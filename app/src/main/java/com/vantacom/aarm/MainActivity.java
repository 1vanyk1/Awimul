package com.vantacom.aarm;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.graphics.Color;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;

import com.vantacom.aarm.dialogs.LoadingDialog;
import com.vantacom.aarm.dialogs.NoObbDialog;
import com.vantacom.aarm.dialogs.SelectSizeDialog;
import com.vantacom.aarm.managers.FileManager;
import com.vantacom.aarm.managers.AppDBManager;
import com.vantacom.aarm.managers.PackageDBManager;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    private Button generate;
    private Button load;
    private View changeSize;
    private Intent wineActivity;
    private AppDBManager sqLiteManager;
    private PackageDBManager packageManager;
    public static String PACKAGE_NAME = "com.vantacom.aarm";

    private String packageName = "prefix";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        if(Build.VERSION.SDK_INT>22){
            requestPermissions(new String[] {"android.permission.WRITE_EXTERNAL_STORAGE"}, 1);
            requestPermissions(new String[] {"android.permission.READ_EXTERNAL_STORAGE"}, 1);
            requestPermissions(new String[] {"android.permission.INTERNET"}, 1);
            requestPermissions(new String[] {"android.permission.RECORD_AUDIO"}, 1);
        }
        sqLiteManager = AppDBManager.getInstance(this);
        packageManager = PackageDBManager.getInstance(this);
        generate = findViewById(R.id.generate);
        generate.setOnClickListener(this);
        load = findViewById(R.id.load);
        load.setOnClickListener(this);
        PACKAGE_NAME = getApplicationContext().getPackageName();

        changeSize = findViewById(R.id.changeSize);
        changeSize.setOnClickListener(this);
        String size = packageManager.getString(packageName, "size");
        updateChangeSizeText(size);
    }

    @Override
    protected void onResume() {
        super.onResume();
        if ((ContextCompat.checkSelfPermission( this, android.Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED ) && (ContextCompat.checkSelfPermission( this, Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED )) {
            return;
        }
        if (sqLiteManager.isBool("firstLoad")) {
            if (FileManager.checkAPKExpansionFiles(this)) {
                LoadingDialog progressDialog = new LoadingDialog(this);
                progressDialog.execute(this);
                sqLiteManager.setBool("firstLoad", false);
            } else {
                NoObbDialog dialog = new NoObbDialog(this);
                FragmentManager manager = getSupportFragmentManager();
                FragmentTransaction transaction = manager.beginTransaction();
                dialog.show(transaction, "dialog");
            }
        }
    }

    public void updateChangeSizeText(String size) {
        TextView changeSize2 = findViewById(R.id.changeSize2);
        changeSize2.setText(size);
    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.generate) {
            LoadingDialog progressDialog = new LoadingDialog(this);
            progressDialog.execute(this);
        } else if (v.getId() == R.id.load) {
            wineActivity = new Intent(MainActivity.this, com.vantacom.aarm.wine.WineActivity.class);
            ArrayList<String> names =  packageManager.getPackageNames();
            Log.e("names", names.toString());
            String name = packageManager.getPackageNames().get(packageManager.getPackageNames().size() - 1);
            wineActivity.putExtra("package", name);
            startActivity(wineActivity);
        }

        else if (v.getId() == R.id.changeSize) {
            SelectSizeDialog dialog = new SelectSizeDialog(this, packageManager);
            FragmentManager manager = getSupportFragmentManager();
            FragmentTransaction transaction = manager.beginTransaction();
            dialog.show(transaction, "dialog");
        }
    }
}