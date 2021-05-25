package com.vantacom.aarm;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.content.ContextCompat;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;
import androidx.recyclerview.widget.RecyclerView;

import android.Manifest;
import android.content.Intent;
import android.content.pm.PackageManager;
import android.os.Build;
import android.os.Bundle;
import android.view.View;

import com.vantacom.aarm.adapters.UserAdapter;
import com.vantacom.aarm.dialogs.LoadingFilesDialog;
import com.vantacom.aarm.dialogs.WarningDialog;
import com.vantacom.aarm.managers.FileManager;
import com.vantacom.aarm.managers.AppDBManager;
import com.vantacom.aarm.managers.PackageDBManager;
import com.vantacom.aarm.wine.WineActivity;
import com.vantacom.aarm.wine.WineService;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity implements View.OnClickListener, UserAdapter.ItemClickListener {
    private View add;
    private Intent userActivity;
    private RecyclerView recyclerView;
    private UserAdapter adapter;
    private AppDBManager sqLiteManager;
    private PackageDBManager packageManager;
    private ArrayList<String> names;
    private static int permissionNotGranted = 0;
    private LoadingFilesDialog dialogLL;
    private WarningDialog dialogW;
    public static String PACKAGE_NAME = "com.vantacom.aarm";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        try {this.getSupportActionBar().hide();}
        catch (NullPointerException e) {}
        setContentView(R.layout.activity_main);
        PACKAGE_NAME = getApplicationContext().getPackageName();
        if (Build.VERSION.SDK_INT>22){
            requestPermissions(new String[] {"android.permission.WRITE_EXTERNAL_STORAGE"}, 1);
            requestPermissions(new String[] {"android.permission.READ_EXTERNAL_STORAGE"}, 1);
            requestPermissions(new String[] {"android.permission.INTERNET"}, 1);
            requestPermissions(new String[] {"android.permission.RECORD_AUDIO"}, 1);
        }
        sqLiteManager = AppDBManager.getInstance(this);
        packageManager = PackageDBManager.getInstance(this);

        add = findViewById(R.id.add);
        add.setOnClickListener(this);

        names = packageManager.getPackageNames();

        recyclerView = findViewById(R.id.users_list);
        adapter = new UserAdapter(names);
        adapter.setClickListener(this);
        recyclerView.setAdapter(adapter);
        if (WineService.isServiceRunning(this)) {
            Intent notifyIntent = new Intent(MainActivity.this, WineActivity.class);
            notifyIntent.setFlags(Intent.FLAG_ACTIVITY_CLEAR_TOP | Intent.FLAG_ACTIVITY_SINGLE_TOP);
            startActivity(notifyIntent);
            return;
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if ((ContextCompat.checkSelfPermission( this, android.Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED ) && (ContextCompat.checkSelfPermission( this, Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED )) {
            if (permissionNotGranted >= 8) {
                WarningDialog dialog = new WarningDialog(this, R.string.permissionWarning);
                FragmentManager manager = getSupportFragmentManager();
                FragmentTransaction transaction = manager.beginTransaction();
                dialog.show(transaction, "dialog");
            } else {
                if (Build.VERSION.SDK_INT > 22) {
                    requestPermissions(new String[]{"android.permission.WRITE_EXTERNAL_STORAGE"}, 1);
                    requestPermissions(new String[]{"android.permission.READ_EXTERNAL_STORAGE"}, 1);
                    requestPermissions(new String[]{"android.permission.INTERNET"}, 1);
                    requestPermissions(new String[]{"android.permission.RECORD_AUDIO"}, 1);
                    requestPermissions(new String[]{"android.permission.FOREGROUND_SERVICE"}, 1);
                }
                permissionNotGranted += 1;
            }
            return;
        }
        if (sqLiteManager.isBool("firstLoad")) {
            if (FileManager.checkAPKExpansionFiles(this)) {
                if (dialogLL == null) {
                    dialogLL = new LoadingFilesDialog(this);
                    FragmentManager manager = getSupportFragmentManager();
                    FragmentTransaction transaction = manager.beginTransaction();
                    dialogLL.show(transaction, "dialog");
                }

            } else {
                if (dialogW == null) {
                    dialogW = new WarningDialog(this, R.string.obbWarning);
                    FragmentManager manager = getSupportFragmentManager();
                    FragmentTransaction transaction = manager.beginTransaction();
                    dialogW.show(transaction, "dialog");
                }
            }
        }
        names = packageManager.getPackageNames();
        adapter.update(names);

    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.add) {
            packageManager.addPackage();
            names = packageManager.getPackageNames();
            adapter.update(names);
        }
    }

    @Override
    public void onItemClick(View view, int position) {
        userActivity = new Intent(MainActivity.this, UserActivity.class);
        String name = names.get(position);
        userActivity.putExtra("package", name);
        startActivity(userActivity);
    }
}