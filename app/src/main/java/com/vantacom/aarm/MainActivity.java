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
import com.vantacom.aarm.dialogs.DownloadFilesDialog;
import com.vantacom.aarm.dialogs.LoadingFilesDialog;
import com.vantacom.aarm.dialogs.NoObbDialog;
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
    private NoObbDialog dialogNO;
    private DownloadFilesDialog dialogFD;
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

        if (savedInstanceState != null) {
            dialogLL = (LoadingFilesDialog) getSupportFragmentManager().getFragment(savedInstanceState, "dialogLL");
            if (dialogLL != null) {
                dialogLL.init(this);
            }
            dialogFD = (DownloadFilesDialog) getSupportFragmentManager().getFragment(savedInstanceState, "dialogFD");
            if (dialogFD != null) {
                dialogFD.init(this);
            }
        }

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
    public void onSaveInstanceState(Bundle savedInstanceState) {
        super.onSaveInstanceState(savedInstanceState);
        if (dialogLL != null && dialogLL.isAdded()) {
            getSupportFragmentManager().putFragment(savedInstanceState, "dialogLL", dialogLL);
        }
        if (dialogFD != null && dialogFD.isAdded()) {
            getSupportFragmentManager().putFragment(savedInstanceState, "dialogFD", dialogFD);
        }
    }

    public void downloadObb() {
        if (dialogFD == null || !dialogFD.isAdded()) {
            dialogFD = new DownloadFilesDialog(this);
            FragmentManager manager = getSupportFragmentManager();
            FragmentTransaction transaction = manager.beginTransaction();
            dialogFD.show(transaction, "dialog");
        }
    }

    public void loadObb() {
        if (dialogLL == null || !dialogLL.isAdded()) {
            dialogLL = new LoadingFilesDialog(this);
            FragmentManager manager = getSupportFragmentManager();
            FragmentTransaction transaction = manager.beginTransaction();
            dialogLL.show(transaction, "dialog");
        }
    }

    public void noObb() {
        if (dialogNO == null || !dialogNO.isAdded()) {
            dialogNO = new NoObbDialog(this, R.string.obbWarning);
            FragmentManager manager = getSupportFragmentManager();
            FragmentTransaction transaction = manager.beginTransaction();
            dialogNO.show(transaction, "dialog");
        }
    }

    @Override
    protected void onResume() {
        super.onResume();
        if ((ContextCompat.checkSelfPermission( this, android.Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED ) && (ContextCompat.checkSelfPermission( this, Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED )) {
            if (permissionNotGranted >= 8) {
                NoObbDialog dialog = new NoObbDialog(this, R.string.permissionWarning);
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
        if (sqLiteManager.isBool("downloading")) {
            if (dialogNO == null && dialogFD == null) {
                dialogNO = new NoObbDialog(this, R.string.obbWarning);
                FragmentManager manager = getSupportFragmentManager();
                FragmentTransaction transaction = manager.beginTransaction();
                dialogNO.show(transaction, "dialog");
            }
        } else if (sqLiteManager.isBool("firstLoad")) {
            if (FileManager.checkAPKExpansionFiles(this)) {
                if (dialogLL == null && dialogFD == null) {
                    dialogLL = new LoadingFilesDialog(this);
                    FragmentManager manager = getSupportFragmentManager();
                    FragmentTransaction transaction = manager.beginTransaction();
                    dialogLL.show(transaction, "dialog");
                }
            } else {
                if (dialogNO == null && dialogFD == null) {
                    dialogNO = new NoObbDialog(this, R.string.obbWarning);
                    FragmentManager manager = getSupportFragmentManager();
                    FragmentTransaction transaction = manager.beginTransaction();
                    dialogNO.show(transaction, "dialog");
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