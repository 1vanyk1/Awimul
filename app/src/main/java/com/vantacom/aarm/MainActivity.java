package com.vantacom.aarm;

import androidx.appcompat.app.AppCompatActivity;

import android.content.Intent;
import android.os.Build;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    private Button generate;
    private Button load;
    private Intent wineActivity;


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        generate = findViewById(R.id.generate);
        generate.setOnClickListener(this);
        load = findViewById(R.id.load);
        load.setOnClickListener(this);
        if(Build.VERSION.SDK_INT>22){
            requestPermissions(new String[] {"android.permission.WRITE_EXTERNAL_STORAGE"}, 1);
            requestPermissions(new String[] {"android.permission.READ_EXTERNAL_STORAGE"}, 1);
            requestPermissions(new String[] {"android.permission.INTERNET"}, 1);
            requestPermissions(new String[] {"android.permission.RECORD_AUDIO"}, 1);
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