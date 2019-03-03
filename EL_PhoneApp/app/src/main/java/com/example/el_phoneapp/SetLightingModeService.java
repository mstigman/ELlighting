package com.example.el_phoneapp;

import android.os.AsyncTask;
import android.util.JsonReader;
import android.util.Log;

import com.example.el_phoneapp.ELData;

import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.URL;

public class SetLightingModeService extends AsyncTask<String, String, String> {
    protected String doInBackground(String... url) {
        URL ELDevice;
        try {
            int counter = 0;
            ELDevice = new URL("http://192.168.4.1/" + url[0]);
            HttpURLConnection myConnection =
                    (HttpURLConnection) ELDevice.openConnection();
            InputStream responseBody = myConnection.getInputStream();
        } catch (Exception e) {
            return e.getMessage();
        }
        return "";
    }
}
