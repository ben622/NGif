package com.ben.android.ngif;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;

import com.ben.android.ngif.component.RenderView;

import java.io.File;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        RenderView renderView = findViewById(R.id.imageView);
        renderView.update("/sdcard/Download/capture2.gif");
    }
}
