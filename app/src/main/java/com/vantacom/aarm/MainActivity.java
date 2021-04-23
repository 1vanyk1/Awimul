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
import android.util.Log;
import android.view.View;
import android.widget.Button;

import com.vantacom.aarm.dialogs.LoadingDialog;
import com.vantacom.aarm.dialogs.NoObbDialog;
import com.vantacom.aarm.managers.FileManager;
import com.vantacom.aarm.managers.AppDBManager;
import com.vantacom.aarm.managers.PackageDBManager;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity implements View.OnClickListener, UserAdapter.ItemClickListener {
    private Button generate, add;
    private Intent wineActivity;
    private RecyclerView recyclerView;
    private UserAdapter adapter;
    private AppDBManager sqLiteManager;
    private PackageDBManager packageManager;
    private ArrayList<String> names;
    public static String PACKAGE_NAME = "com.vantacom.aarm";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
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

        generate = findViewById(R.id.generate);
        generate.setOnClickListener(this);
        add = findViewById(R.id.add);
        add.setOnClickListener(this);

        names = packageManager.getPackageNames();

        recyclerView = findViewById(R.id.users_list);
        adapter = new UserAdapter(names);
        adapter.setClickListener(this);
        recyclerView.setAdapter(adapter);

        Log.e("names", names.toString());
    }

    @Override
    protected void onResume() {
        super.onResume();
        if ((ContextCompat.checkSelfPermission( this, android.Manifest.permission.WRITE_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED ) && (ContextCompat.checkSelfPermission( this, Manifest.permission.READ_EXTERNAL_STORAGE) != PackageManager.PERMISSION_GRANTED )) {
            if (Build.VERSION.SDK_INT>22){
                requestPermissions(new String[] {"android.permission.WRITE_EXTERNAL_STORAGE"}, 1);
                requestPermissions(new String[] {"android.permission.READ_EXTERNAL_STORAGE"}, 1);
                requestPermissions(new String[] {"android.permission.INTERNET"}, 1);
                requestPermissions(new String[] {"android.permission.RECORD_AUDIO"}, 1);
            }
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
        names = packageManager.getPackageNames();
        adapter.update(names);
    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.generate) {
            LoadingDialog progressDialog = new LoadingDialog(this);
            progressDialog.execute(this);
        }
        else if (v.getId() == R.id.add) {
            packageManager.addPackage();
            names = packageManager.getPackageNames();
            adapter.update(names);
        }
    }

    @Override
    public void onItemClick(View view, int position) {
        wineActivity = new Intent(MainActivity.this, UserActivity.class);
        String name = names.get(position);
        wineActivity.putExtra("package", name);
        startActivity(wineActivity);
    }
}