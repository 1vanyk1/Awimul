package com.vantacom.aarm;

import android.content.Intent;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import com.vantacom.aarm.dialogs.ConfirmDeleteDialog;
import com.vantacom.aarm.dialogs.SelectSizeDialog;
import com.vantacom.aarm.managers.PackageDBManager;

import java.util.ArrayList;

public class UserActivity extends AppCompatActivity implements View.OnClickListener {
    private Intent wineActivity;
    private PackageDBManager packageManager;
    private View changeSize;
    private Button load, delete;

    private String packageName = "prefix";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_user);
        packageName = getIntent().getStringExtra("package");
        packageManager = PackageDBManager.getInstance(this);
        load = findViewById(R.id.load);
        load.setOnClickListener(this);
        delete = findViewById(R.id.delete);
        delete.setOnClickListener(this);

        ArrayList<String> names =  packageManager.getPackageNames();
        Log.e("names", names.toString());

        changeSize = findViewById(R.id.changeSize);
        changeSize.setOnClickListener(this);
        String size = packageManager.getString(packageName, "size");
        updateChangeSizeText(size);
    }

    public void deleteUser() {
        packageManager.deletePackage(packageName);
        finish();
    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.load) {
            wineActivity = new Intent(UserActivity.this, com.vantacom.aarm.wine.WineActivity.class);
            wineActivity.putExtra("package", packageName);
            startActivity(wineActivity);
        } else if (v.getId() == R.id.changeSize) {
            SelectSizeDialog dialog = new SelectSizeDialog(this, packageManager);
            FragmentManager manager = getSupportFragmentManager();
            FragmentTransaction transaction = manager.beginTransaction();
            dialog.show(transaction, "dialog");
        } else if (v.getId() == R.id.delete) {
            ConfirmDeleteDialog dialog = new ConfirmDeleteDialog(this);
            FragmentManager manager = getSupportFragmentManager();
            FragmentTransaction transaction = manager.beginTransaction();
            dialog.show(transaction, "dialog");
        }
    }

    public void updateChangeSizeText(String size) {
        TextView changeSize2 = findViewById(R.id.changeSize2);
        changeSize2.setText(size);
    }
}
