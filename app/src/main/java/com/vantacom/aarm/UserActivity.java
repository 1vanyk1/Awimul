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
    private Switch workInBG, showCursor;

    private SelectSizeDialog dialogSS;
    private RenamePackageDialog dialogRP;
    private ConfirmDeleteDialog dialogCD;
    private DeletingPackageDialog dialogDP;

    private String packageName = "prefix";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        try {this.getSupportActionBar().hide();}
        catch (NullPointerException e) {}
        setContentView(R.layout.activity_user);
        packageManager = PackageDBManager.getInstance(this);
        packageName = getIntent().getStringExtra("package");
        if (savedInstanceState != null) {
            packageName = savedInstanceState.getString("package", packageName);
            dialogSS = (SelectSizeDialog)getSupportFragmentManager().getFragment(savedInstanceState, "dialogSS");
            if (dialogSS != null) {
                dialogSS.init(this, packageManager, packageName);
            }
            dialogRP = (RenamePackageDialog) getSupportFragmentManager().getFragment(savedInstanceState, "dialogRP");
            if (dialogRP != null) {
                dialogRP.init(this, packageManager, packageName);
            }
            dialogCD = (ConfirmDeleteDialog) getSupportFragmentManager().getFragment(savedInstanceState, "dialogCD");
            if (dialogCD != null) {
                dialogCD.init(this);
            }
            dialogDP = (DeletingPackageDialog) getSupportFragmentManager().getFragment(savedInstanceState, "dialogDP");
            if (dialogDP != null) {
                dialogDP.init(this, packageName);
            }
        }
        loadButton = findViewById(R.id.load);
        loadButton.setOnClickListener(this);
        deleteButton = findViewById(R.id.delete);
        deleteButton.setOnClickListener(this);
        workInBG = findViewById(R.id.work_in_bg);
        workInBG.setChecked(packageManager.isBool(packageName, "workInBackground"));
        workInBG.setOnCheckedChangeListener(this);
        showCursor = findViewById(R.id.show_cursor);
        showCursor.setChecked(packageManager.isBool(packageName, "showCursor"));
        showCursor.setOnCheckedChangeListener(this);
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
        if (dialogSS != null) {
            getSupportFragmentManager().putFragment(savedInstanceState, "dialogSS", dialogSS);
        }
        if (dialogRP != null) {
            getSupportFragmentManager().putFragment(savedInstanceState, "dialogRP", dialogRP);
        }
        if (dialogCD != null) {
            getSupportFragmentManager().putFragment(savedInstanceState, "dialogCD", dialogCD);
        }
        if (dialogDP != null) {
            getSupportFragmentManager().putFragment(savedInstanceState, "dialogDP", dialogDP);
        }
    }

    public void deleteUser() {
        dialogCD = null;
        dialogDP = new DeletingPackageDialog(this, packageName);
        FragmentManager manager = getSupportFragmentManager();
        FragmentTransaction transaction = manager.beginTransaction();
        dialogDP.show(transaction, "dialog");
    }

    public void updateName(String packageName) {
        this.packageName = packageName;
        workInBG.setChecked(packageManager.isBool(packageName, "workInBackground"));
        String size = packageManager.getString(packageName, "size");
        updateChangeSizeText(size);
        textViewName.setText(packageName);
        dialogRP = null;
    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.load) {
            wineActivity = new Intent(UserActivity.this, com.vantacom.aarm.wine.WineActivity.class);
            wineActivity.putExtra("package", packageName);
            startActivity(wineActivity);
        } else if (v.getId() == R.id.changeSize) {
            dialogSS = new SelectSizeDialog(this, packageManager, packageName);
            FragmentManager manager = getSupportFragmentManager();
            FragmentTransaction transaction = manager.beginTransaction();
            dialogSS.show(transaction, "dialog");
        } else if (v.getId() == R.id.delete) {
            dialogCD = new ConfirmDeleteDialog(this);
            FragmentManager manager = getSupportFragmentManager();
            FragmentTransaction transaction = manager.beginTransaction();
            dialogCD.show(transaction, "dialog");
        } else if (v.getId() == R.id.rename) {
            dialogRP = new RenamePackageDialog(this, packageManager, packageName);
            FragmentManager manager = getSupportFragmentManager();
            FragmentTransaction transaction = manager.beginTransaction();
            dialogRP.show(transaction, "dialog");
        }
    }

    public void updateChangeSizeText(String size) {
        TextView changeSize2 = findViewById(R.id.changeSize2);
        changeSize2.setText(size);
        dialogSS = null;
    }

    @Override
    public void onCheckedChanged(CompoundButton buttonView, boolean isChecked) {
        if (buttonView.getId() == R.id.work_in_bg) {
            packageManager.setBool(packageName, "workInBackground", isChecked);
        } else if (buttonView.getId() == R.id.show_cursor) {
            packageManager.setBool(packageName, "showCursor", isChecked);
        }
    }
}
