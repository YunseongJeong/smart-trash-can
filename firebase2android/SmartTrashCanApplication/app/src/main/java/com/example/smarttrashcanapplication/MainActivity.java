package com.example.smarttrashcanapplication;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;

import com.dinuscxj.progressbar.CircleProgressBar;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {

    private static final String DEFAULT_PATTERN="%d%%";
    CircleProgressBar circleProgressBar;

    private FirebaseDatabase database;
    private DatabaseReference databaseReference;

    private ArrayList<DataOfFilled> arrayListOfFill = null;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);



        circleProgressBar = findViewById(R.id.circleFill);
        circleProgressBar.setProgress(100);

        database = FirebaseDatabase.getInstance();
        databaseReference = database.getReference("Fill");
        databaseReference.addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                arrayListOfFill = null;
                for(DataSnapshot snapshot1 : snapshot.getChildren()) {
                    DataOfFilled dataOfFilled = snapshot1.getValue(DataOfFilled.class);
                    arrayListOfFill.add(dataOfFilled);
                }
                circleProgressBar.setProgress((int)arrayListOfFill.get(arrayListOfFill.toArray().length-1).getAmountOfFill());
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {
                Log.e("MainActivity", error.toException().toString());
            }
        });
    }


}