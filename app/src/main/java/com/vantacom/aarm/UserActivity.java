package com.vantacom.aarm;

import android.content.Intent;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;

import androidx.appcompat.app.AppCompatActivity;
import androidx.fragment.app.FragmentManager;
import androidx.fragment.app.FragmentTransaction;

import com.vantacom.aarm.dialogs.ConfirmDeleteDialog;
import com.vantacom.aarm.dialogs.DeletingPackageDialog;
import com.vantacom.aarm.dialogs.RenamePackageDialog;
import com.vantacom.aarm.dialogs.SelectSizeDialog;
import com.vantacom.aarm.managers.PackageDBManager;

public class UserActivity extends AppCompatActivity implements View.OnClickListener, CompoundButton.OnCheckedChangeListener {
    private Intent wineActivity;
    private PackageDBManager packageManager;
    private View changeSize;
    private TextView textViewName;
    private Button loadButton, deleteButton, renameButton;
    private Switch workInBG;

    private String packageName = "prefix";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        try {this.getSupportActionBar().hide();}
        catch (NullPointerException e) {}
        setContentView(R.layout.activity_user);
        packageName = getIntent().getStringExtra("package");
        if (savedInstanceState != null) {
            packageName = savedInstanceState.getString("package", packageName);
        }
        packageManager = PackageDBManager.getInstance(this);
        loadButton = findViewById(R.id.load);
        loadButton.setOnClickListener(this);
        deleteButton = findViewById(R.id.delete);
        deleteButton.setOnClickListener(this);
        workInBG = findViewById(R.id.workInBG);
        workInBG.setChecked(packageManager.isBool(packageName, "workInBackground"));
        workInBG.setOnCheckedChangeListener(this);
        changeSize = findViewById(R.id.changeSize);
        changeSize.setOnClickListener(this);
        renameButton = findViewById(R.id.rename);
        renameButton.setOnClickListener(this);
        textViewName = findViewById(R.id.prefixName);
        textViewName.setText(packageName);
        String size = packageManager.getString(packageName, "size");
        updateChangeSizeText(size);
    }

    @Override
    public void onSaveInstanceState(Bundle savedInstanceState) {
        super.onSaveInstanceState(savedInstanceState);
        savedInstanceState.putString("package", packageName);
    }

    public void deleteUser() {
        DeletingPackageDialog dialog = new DeletingPackageDialog(this, packageName);
        FragmentManager manager = getSupportFragmentManager();
        FragmentTransaction transaction = manager.beginTransaction();
        dialog.show(transaction, "dialog");
    }

    public void updateName(String packageName) {
        this.packageName = packageName;
        workInBG.setChecked(packageManager.isBool(packageName, "workInBackground"));
        String size = packageManager.getString(packageName, "size");
        updateChangeSizeText(size);
        textViewName.setText(packageName);
    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.load) {
            wineActivity = new Intent(UserActivity.this, com.vantacom.aarm.wine.WineActivity.class);
            wineActivity.putExtra("package", packageName);
            startActivity(wineActivity);
        } else if (v.getId() == R.id.changeSize) {
            SelectSizeDialog dialog = new SelectSizeDialog(this, packageManager, packageName);
            FragmentManager manager = getSupportFragmentManager();
            FragmentTransaction transaction = manager.beginTransaction();
            dialog.show(transaction, "dialog");
        } else if (v.getId() == R.id.delete) {
            ConfirmDeleteDialog dialog = new ConfirmDeleteDialog(this);
            FragmentManager manager = getSupportFragmentManager();
            FragmentTransaction transaction = manager.beginTransaction();
            dialog.show(transaction, "dialog");
        } else if (v.getId() == R.id.rename) {
            RenamePackageDialog dialog = new RenamePackageDialog(this, packageName, packageManager);
            FragmentManager manager = getSupportFragmentManager();
            FragmentTransaction transaction = manager.beginTransaction();
            dialog.show(transaction, "dialog");
        }
    }

    public void updateChangeSizeText(String size) {
        TextView changeSize2 = findViewById(R.id.changeSize2);
        changeSize2.setText(size);
    }

    @Override
    public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
        if (buttonView.getId() == R.id.workInBG) {
            packageManager.setBool(packageName, "workInBackground", isChecked);
        }
    }
}
