package com.example.smarttrashcanapplication;

import static android.content.ContentValues.TAG;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.util.Log;
import android.widget.Button;

import com.dinuscxj.progressbar.CircleProgressBar;
import com.google.android.gms.tasks.Task;
import com.google.firebase.database.ChildEventListener;
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

    private ArrayList<Data> arrayList;

    private boolean isStarted = true;
    @SuppressLint("LongLogTag")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        arrayList = new ArrayList<>();

        circleProgressBar = findViewById(R.id.circleFill);
        circleProgressBar.setProgress(100);

        database = FirebaseDatabase.getInstance();
        databaseReference = database.getReference("Data");
        databaseReference.addListenerForSingleValueEvent(new ValueEventListener() {
            @Override
            public void onDataChange(DataSnapshot snapshot) {
                arrayList.clear();
                for(DataSnapshot snapshot1 : snapshot.getChildren()) {
                    Data data = snapshot1.getValue(Data.class);
                    arrayList.add(data);
                }
                circleProgressBar.setProgress(Integer.valueOf(arrayList.get(arrayList.toArray().length-1).getFillData()));
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {
                Log.e("MainActivity", error.toException().toString());
            }
        });


        databaseReference.addValueEventListener(new ValueEventListener() {
            @Override
            public void onDataChange(@NonNull DataSnapshot snapshot) {
                if (isStarted){
                    isStarted = false;
                } else {
                    recreate();
                }
            }

            @Override
            public void onCancelled(@NonNull DatabaseError error) {

            }
        });
    }
}