package com.example.el_phoneapp;

import android.content.Context;
import android.os.AsyncTask;
import android.util.JsonReader;
import android.util.Log;
import android.view.View;
import android.widget.TextView;

import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

import javax.net.ssl.HttpsURLConnection;

public class GetDataService extends AsyncTask<String, String, String> {

    //private Context mContext;
    private View rootView;

    public GetDataService(View rootView){
        //this.mContext = context;
        this.rootView = rootView;
    }

    ELData fullData;
    protected String doInBackground(String... url) {
        URL ELDevice;
        int[] data = new int[7];
        try {
            int counter = 0;
            ELDevice = new URL("http://192.168.4.1/data");
            HttpURLConnection myConnection =
                    (HttpURLConnection) ELDevice.openConnection();
            Log.d("maxwell", "checkpoint 1");
            InputStream responseBody = myConnection.getInputStream();
            InputStreamReader responseBodyReader =
                    new InputStreamReader(responseBody, "UTF-8");

            JsonReader jsonReader = new JsonReader(responseBodyReader);

            jsonReader.beginObject(); // Start processing the JSON object
            while (jsonReader.hasNext()) { // Loop through all keys
                String key = jsonReader.nextName(); // Fetch the next key
                String value = jsonReader.nextString();
                Log.d("maxwell", value);
                data[counter] = Integer.parseInt(value);
                counter++;
            }
            fullData = new ELData(data);
            jsonReader.close();
        } catch (Exception e) {
            //e.printStackTrace();
            return e.getMessage();
        }
        return data[0] + "";
    }

    protected void onPostExecute(String result) {
        TextView tv1 = (TextView) rootView.findViewById(R.id.textView1);
        tv1.setText(result);
    }

}
