package com.example.smarttrashcanapplication;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;

import com.dinuscxj.progressbar.CircleProgressBar;

public class MainActivity extends AppCompatActivity {

    private static final String DEFAULT_PATTERN="%d%%";
    CircleProgressBar circleProgressBar;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        circleProgressBar = findViewById(R.id.circleFill);
        circleProgressBar.setProgress(70);
    }


}