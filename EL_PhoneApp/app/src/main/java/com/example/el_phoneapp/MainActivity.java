package com.example.el_phoneapp;

import android.os.AsyncTask;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.JsonReader;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.TextView;


public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        Button updatebutton = (Button) findViewById(R.id.button1);
        updatebutton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                GetDataService getData = new GetDataService(findViewById(android.R.id.content));
                getData.execute();
            }
        });

        Button normalModebtn = (Button) findViewById(R.id.normalBtn);
        normalModebtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                SetLightingModeService lighting = new SetLightingModeService();
                lighting.execute("normal");
            }
        });

        Button wavebtn = (Button) findViewById(R.id.Wavebtn);
        wavebtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                SetLightingModeService lighting = new SetLightingModeService();
                lighting.execute("waves");
            }
        });

        Button fadebtn = (Button) findViewById(R.id.Fadebtn);
        fadebtn.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
                SetLightingModeService lighting = new SetLightingModeService();
                lighting.execute("fade");
            }
        });
        TextView tv1 = (TextView)findViewById(R.id.textView1);
        tv1.setText("test");
    }


}
