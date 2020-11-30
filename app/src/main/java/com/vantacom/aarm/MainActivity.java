package com.vantacom.aarm;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.Toast;

import java.io.File;
import java.io.IOException;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {
    Button generate;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        generate = findViewById(R.id.generate);
        generate.setOnClickListener(this);
        Button b = new Button(this);
    }

    @Override
    public void onClick(View v) {
        if (v.getId() == R.id.generate) {
            try {
                File f = FileManager.createFile("file.png", this);
                Toast.makeText(this, "1", Toast.LENGTH_SHORT).show();
                FileManager.copy(this.getAssets().open("armwine/picture.jpg"), f);
                Toast.makeText(this, "2", Toast.LENGTH_SHORT).show();
            } catch (IOException e) {
                return;
            }
        }
    }
}