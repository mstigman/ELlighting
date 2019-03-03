package com.example.el_phoneapp;

public class ELData {

    protected int accelX;
    protected int accelY;
    protected int accelZ;

    protected int gryoX;
    protected int gryoY;
    protected int gryoZ;

    protected int temp;

    public ELData(int[] data) {
        if (data.length != 7) {
            throw new IllegalArgumentException("Array for data is wrong size");
        }
        accelX = data[0];
        accelX = data[1];
        accelX = data[2];

        gryoX = data[3];
        gryoX = data[4];
        gryoX = data[5];

        temp = data[6];
    }

    public int getAccelX() {
        return accelX;
    }

    public int getAccelY() {
        return accelY;
    }

    public int getAccelZ() {
        return accelZ;
    }

    public int getGryoX() {
        return gryoX;
    }

    public int getGryoY() {
        return gryoY;
    }

    public int getGryoZ() {
        return gryoZ;
    }

    public int getTemp() {
        return temp;
    }
}
