package com.example.smarttrashcanapplication;

import static android.content.ContentValues.TAG;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.util.Log;
import android.view.MenuItem;
import android.widget.Button;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import com.dinuscxj.progressbar.CircleProgressBar;
import com.google.android.gms.tasks.OnCompleteListener;
import com.google.android.gms.tasks.Task;
import com.google.android.material.bottomnavigation.BottomNavigationView;
import com.google.android.material.navigation.NavigationBarMenuView;
import com.google.android.material.navigation.NavigationView;
import com.google.firebase.database.ChildEventListener;
import com.google.firebase.database.DataSnapshot;
import com.google.firebase.database.DatabaseError;
import com.google.firebase.database.DatabaseReference;
import com.google.firebase.database.FirebaseDatabase;
import com.google.firebase.database.ValueEventListener;
import com.google.firebase.messaging.FirebaseMessaging;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {

    private static final String DEFAULT_PATTERN="%d%%";
    CircleProgressBar circleProgressBar;

    private BottomNavigationView navigationView;

    private FirebaseDatabase database;
    private DatabaseReference databaseReference;

    private ScrollView scrollView;

    private ArrayList<Data> arrayList;

    private int curFillData=0;
    private int curAirDate=0;

    private TextView tvAir;

    private boolean isStarted = true;
    @SuppressLint("LongLogTag")
    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        FirebaseMessaging.getInstance().getToken()
                .addOnCompleteListener(new OnCompleteListener<String>() {
                    @Override
                    public void onComplete(@NonNull Task<String> task) {
                        if (!task.isSuccessful()) {
                            Log.w(TAG, "Fetching FCM registration token failed", task.getException());
                            return;
                        }

                        // Get new FCM registration token
                        String token = task.getResult();

                        // Log and toast
                        Log.d(TAG, "token : "+token);
                        //Toast.makeText(MainActivity.this, msg, Toast.LENGTH_SHORT).show();
                    }
                });

        scrollView  = findViewById(R.id.main_sv);
        navigationView = findViewById(R.id.bottom_navi);
        navigationView.setOnNavigationItemSelectedListener(new BottomNavigationView.OnNavigationItemSelectedListener() {
            @Override
            public boolean onNavigationItemSelected(@NonNull MenuItem item) {
                switch (item.getItemId()) {
                    case R.id.action_trash:
                        scrollView.scrollTo(0, scrollView.getTop());
                        break;
                    case R.id.action_air:
                        scrollView.scrollTo(0, scrollView.getBottom());
                        break;
                    case R.id.action_noti:
                        break;
                    case R.id.action_time:
                        break;
                }
                return true;
            }
        });

        tvAir = findViewById(R.id.tv_air);

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
                curFillData = Integer.valueOf(arrayList.get(arrayList.toArray().length-1).getFillData());
                curAirDate = Integer.valueOf(arrayList.get(arrayList.toArray().length-1).getAirData());
                circleProgressBar.setProgress(curFillData);
                displayAboutAir(curAirDate);
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

    @SuppressLint("ResourceAsColor")
    private void displayAboutAir(int airData) {
        if (airData<=100){
            Log.e("tlqkf", "good2");
            tvAir.setText("[좋음]");
            //tvAir.setTextColor(android.R.color.holo_blue_light);
        } else if(airData<=200) {
            Log.e("tlqkf", "good-1");
            tvAir.setText("[보통]");
            //tvAir.setTextColor(android.R.color.holo_green_light);
        } else {
            Log.e("tlqkf", "good-2");
            tvAir.setText("[나쁨]");
            //tvAir.setTextColor(android.R.color.holo_red_dark);
        }
    }
}