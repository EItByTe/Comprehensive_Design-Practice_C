package com.example.socketandroid;

import androidx.annotation.RequiresApi;
import androidx.appcompat.app.AppCompatActivity;
//import android.app.Activity;
import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.icu.math.BigDecimal;
import android.location.Criteria;
import android.location.Location;
import android.location.LocationListener;
import android.location.LocationManager;
import android.os.Build;
import android.os.Bundle;
import android.os.Handler;
import android.os.Message;
import android.os.StrictMode;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;

import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.List;

import android.os.Bundle;

public class MainActivity extends AppCompatActivity {

    private static final int CONNECTING = 0;
    private static final int SENDING = 1;
    private static final int CLOSE = 2;
    private static final int RECEIVE = 3;

    static TextView text;
    EditText input, address, port;
    Socket socket;

    static String addStr = "192.168.0.107";
    static String sendMsg,receiveMsg;

    int portStr = 8848;
    // Location
    private final String TAG = "sensor-sample";
    private TextView mAccelerometerSensorTextView;
    private TextView mMagneticSensorTextView;
    private TextView mGyroscopeSensorTextView;
    private TextView mOrientationSensorTextView;
    // GPS
    private TextView mGPSTextView;
    private LocationListener GPS_listener;
    private double locationx = 0.0;
    private double locationy = 0.0;
    //四元数
    private TextView mq0123TextView;

    private SensorManager mSensorManager;
    private MySensorEventListener mMySensorEventListener;
    private float[] mAccelerometerReading = new float[3];
    private float[] mMagneticFieldReading = new float[3];
    private int i = 1;


    private static Handler handler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            if (msg.what == CONNECTING) {
                text.setText("正在连接中......");
            }else if(msg.what==SENDING){
                text.setText("Client Sending: '" + sendMsg + "'");
            }else if(msg.what==CLOSE){
                text.append("socket close");
            }else if(msg.what==RECEIVE){
                text.setText("Client Receiveing: '" + receiveMsg + "'");
            }
        }
    };

    @Override
        public void onCreate(Bundle savedInstanceState) {
            super.onCreate(savedInstanceState);
            //方法一、用StrictMode模式，调用以下码块，删除Thread和handle方式
            // setInternet()；
            setContentView(R.layout.activity_main);
            Button button = (Button) this.findViewById(R.id.btn_send);
            text = (TextView) findViewById(R.id.receive);
//            input = (EditText) findViewById(R.id.input);
            address = (EditText) findViewById(R.id.address);
            port = (EditText) findViewById(R.id.port);
            //location
            mAccelerometerSensorTextView = findViewById(R.id.accelerometer_sensor);
            mMagneticSensorTextView = findViewById(R.id.magnetic_sensor);
            mGyroscopeSensorTextView = findViewById(R.id.gyroscope_sensor);
    //        mOrientationSensorTextView = findViewById(R.id.orientation_sensor);
            this.mSensorManager = (SensorManager) getSystemService(SENSOR_SERVICE);
            this.mMySensorEventListener = new MySensorEventListener();
            //GPS
            mGPSTextView = findViewById(R.id.GPS_sensor);
            mq0123TextView = findViewById((R.id.q0123));

            final LocationManager mlocationManager = (LocationManager) getSystemService(Context.LOCATION_SERVICE);
            Location location = mlocationManager.getLastKnownLocation(LocationManager.GPS_PROVIDER);
            initLocation(mlocationManager);
            mlocationManager.requestLocationUpdates(LocationManager.GPS_PROVIDER, 100, 2, GPS_listener);

//        address.setText("162.30.200.26");
//        port.setText("205");
            address.setText("192.168.0.107");
            port.setText("8848");
//            button.setOnClickListener(new View.OnClickListener() {

//                @Override
//                public void onClick(View v) {
//
//                    addStr = address.getText().toString().trim();
//                    portStr = Integer.parseInt(port.getText().toString().trim());
//                    //方法二、用多线程的方式
//                    new WorkThread().start();
//                }
//            });
        }

        //工作线程
        private class WorkThread extends Thread {
            @Override
            public void run() {
                //处理比较耗时的操作
                //数据处理完成后，关于UI的更新要通过handler发送消息
                Message msg = new Message();
                Message msg1 = new Message();
                Message msg2 = new Message();
                msg.what = CONNECTING;
                handler.sendMessage(msg);
                try {
                    socket = new Socket(addStr, portStr);
                    if (socket == null) {
                        Log.e("error", "socket  null");
                        return;
                    }
                    //发送给服务端的消息
                    //修改这个sengMsg即可
                    //发送一个坐标，比如(120,0)，发送四元数，比如(1,0,0,0)
                    if(Math.abs(locationx)>=1e-6 && Math.abs(locationy)>=1e-6) {
                        sendMsg = "";
                        sendMsg +="(";
                        sendMsg +=locationx;
                        sendMsg +=",";
                        sendMsg +=locationy;
                        sendMsg +=")";
                        msg1.what = SENDING;
                        handler.sendMessage(msg1);

                        PrintWriter out = new PrintWriter(
                                new BufferedWriter(new OutputStreamWriter(
                                        socket.getOutputStream())), true);
                        out.println(sendMsg);
                    }

                } catch (IOException e) {
                    e.printStackTrace();
                }finally {
                    //关闭Socket
                    if (socket != null) {
                        try {
                            socket.close();
                        } catch (IOException e) {
                            e.printStackTrace();
                        }
                    }
                    msg2.what = CLOSE;
                    handler.sendMessage(msg2);
                }



            }
        }

    @RequiresApi(api = Build.VERSION_CODES.N)
    public void showLocation(Location currentLocation) {
        if (currentLocation != null) {
            String s = "";
            s += " [GPS] : (";
            s += currentLocation.getLongitude();
            locationx = currentLocation.getLongitude();
            BigDecimal bd1 = new BigDecimal(locationx);
            locationx = bd1.setScale(5, BigDecimal.ROUND_DOWN).doubleValue(); // 保留五位小数，不四舍五入(可选舍入模式)
            s += ",";
            s += currentLocation.getLatitude();
            locationy = currentLocation.getLatitude();
            BigDecimal bd2 = new BigDecimal(locationy);
            locationy = bd2.setScale(5, BigDecimal.ROUND_DOWN).doubleValue(); // 保留五位小数，不四舍五入(可选舍入模式)
            s += ")  Speed: ";
            s += currentLocation.getSpeed();
            s += "  Direction: ";
            s += currentLocation.getBearing();
            mGPSTextView.setText(s);
        } else {
            mGPSTextView.setText("");
        }
    }

    private void initLocation(LocationManager mlocationManager) {
        //获取定位方式
        List<String> providers = mlocationManager.getProviders(true);
        for (String s : providers) {
            Log.e(TAG, s);
        }

        Criteria criteria = new Criteria();
        // 查询精度：高，Criteria.ACCURACY_COARSE比较粗略，Criteria.ACCURACY_FINE则比较精确
        criteria.setAccuracy(Criteria.ACCURACY_FINE);
        // 是否查询海拨：否
        criteria.setAltitudeRequired(true);
        // 是否查询方位角 : 否
        criteria.setBearingRequired(true);
        // 设置是否要求速度
        criteria.setSpeedRequired(true);
        // 电量要求：高
        criteria.setPowerRequirement(Criteria.POWER_LOW);
//        bestProvider = mlocationManager.getBestProvider(criteria, true);  //获取最佳定位

        GPS_listener = new LocationListener() {
            public void onLocationChanged(Location location) {
//                Log.d(TAG, "gps data:come in1111");
                // TODO Auto-generated method stub
                showLocation(location);
            }

            public void onProviderDisabled(String provider) {
                // TODO Auto-generated method stub
                showLocation(null);
            }

            public void onProviderEnabled(String provider) {
                // TODO Auto-generated method stub
//                Log.d(TAG, "gps data:come in222222");
                showLocation(mlocationManager.getLastKnownLocation(provider));
            }

            public void onStatusChanged(String provider, int status, Bundle extras) {
                // TODO Auto-generated method stub
            }
        };
    }
    @Override
    protected void onResume() {
        super.onResume();
        if (mSensorManager == null) {
            return;
        }

        Sensor accelerometerSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER);
        if (accelerometerSensor != null) {
            //register accelerometer sensor listener
            mSensorManager.registerListener((SensorEventListener) mMySensorEventListener, accelerometerSensor, SensorManager.SENSOR_DELAY_NORMAL);
        } else {
            Log.d(TAG, "Accelerometer sensors are not supported on current devices.");
        }

        Sensor magneticSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_MAGNETIC_FIELD);
        if (magneticSensor != null) {
            //register magnetic sensor listener
            mSensorManager.registerListener(mMySensorEventListener, magneticSensor, SensorManager.SENSOR_DELAY_NORMAL);
        } else {
            Log.d(TAG, "Magnetic sensors are not supported on current devices.");
        }

        Sensor gyroscopeSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE);
        if (gyroscopeSensor != null) {
            //register gyroscope sensor listener
            mSensorManager.registerListener(mMySensorEventListener, gyroscopeSensor, SensorManager.SENSOR_DELAY_NORMAL);
        } else {
            Log.d(TAG, "Gyroscope sensors are not supported on current devices.");
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        if (mSensorManager == null) {
            return;
        }
        //unregister all listener
        mSensorManager.unregisterListener(mMySensorEventListener);
    }

    /*
    This orientation sensor was deprecated in Android 2.2 (API level 8), and this sensor type was deprecated in Android 4.4W (API level 20).
    The sensor framework provides alternate methods for acquiring device orientation.
     */
    private void calculateOrientation() {
        final float[] rotationMatrix = new float[9];
        SensorManager.getRotationMatrix(rotationMatrix, null, mAccelerometerReading, mMagneticFieldReading);

        final float[] orientationAngles = new float[3];
        SensorManager.getOrientation(rotationMatrix, orientationAngles);
        Log.d(TAG, "orientation data[x:" + String.format("%.4f", orientationAngles[0]) + ", y:" + String.format("%.4f", orientationAngles[1]) + ", z:" + String.format("%.4f", orientationAngles[2]) + "]");
        mOrientationSensorTextView.setText("[x:" + String.format("%.4f", orientationAngles[0]) + ", y:" + String.format("%.4f", orientationAngles[1]) + ", z:" + String.format("%.4f", orientationAngles[2]) + "]");
    }

    private class MySensorEventListener implements SensorEventListener {
        public float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f;
        private final float twoKp = 5.0f;
        private final float twoKi = 1.0f;
        private float integralFBx = 0.0f;
        private float integralFBy = 0.0f;
        private float integralFBz = 0.0f;
        private final float sampleFreq = 1.0f;
        private float gx_last = 0.0f;
        private float gy_last = 0.0f;
        private float gz_last = 0.0f;
        private float ax_last = 0.0f;
        private float ay_last = 0.0f;
        private float az_last = 0.0f;
        private float mx_last = 0.0f;
        private float my_last = 0.0f;
        private float mz_last = 0.0f;

        @Override
        public void onSensorChanged(SensorEvent event) {
            if ((i % 49) == 0) {
                if (event.sensor.getType() == Sensor.TYPE_ACCELEROMETER) {
                    mAccelerometerReading = event.values;
                    ax_last = event.values[0];
                    ay_last = event.values[1];
                    az_last = event.values[2];
                    Log.d(TAG, "accelerometer data[x:" + event.values[0] + ", y:" + event.values[1] + ", z:" + event.values[2] + "]");
                    String accelerometerString = "[加速度计]: ";
                    accelerometerString += "[x:" + String.format("%.4f", event.values[0]) + ", y:" + String.format("%.4f", event.values[1]) + ", z:" + String.format("%.4f", event.values[2]) + "]";
                    mAccelerometerSensorTextView.setText(accelerometerString);
                } else if (event.sensor.getType() == Sensor.TYPE_MAGNETIC_FIELD) {
                    mMagneticFieldReading = event.values;
                    mx_last = event.values[0];
                    my_last = event.values[1];
                    mz_last = event.values[2];
                    Log.d(TAG, "magnetic data[x:" + event.values[0] + ", y:" + event.values[1] + ", z:" + event.values[2] + "]");
                    String magneticString = "[磁力计]: ";
                    magneticString += "[x:" + String.format("%.4f", event.values[0]) + ", y:" + String.format("%.4f", event.values[1]) + ", z:" + String.format("%.4f", event.values[2]) + "]";
                    mMagneticSensorTextView.setText(magneticString);
                } else if (event.sensor.getType() == Sensor.TYPE_GYROSCOPE) {
                    Log.d(TAG, "gyroscope data[x:" + event.values[0] + ", y:" + event.values[1] + ", z:" + event.values[2] + "]");
                    gx_last = event.values[0];
                    gy_last = event.values[1];
                    gz_last = event.values[2];
                    String mGyroscopeSensorString = "[陀螺仪]: ";
                    mGyroscopeSensorString += "[x:" + String.format("%.4f", event.values[0]) + ", y:" + String.format("%.4f", event.values[1]) + ", z:" + String.format("%.4f", event.values[2]) + "]";
                    mGyroscopeSensorTextView.setText(mGyroscopeSensorString);
                }
//                calculateOrientation();
                if(Math.abs(gx_last)>=1e-6 && Math.abs(gy_last)>=1e-6 && Math.abs(gz_last)>=1e-6 &&
                        Math.abs(ax_last)>=1e-6 && Math.abs(ay_last)>=1e-6 && Math.abs(az_last)>=1e-6 &&
                        Math.abs(mx_last)>=1e-6 && Math.abs(my_last)>=1e-6 && Math.abs(mz_last)>=1e-6) {
                    MySensorEventListener mySensorEventListener = new MySensorEventListener();
                    mySensorEventListener.MahonyAHRSupdate(gx_last,gy_last,gz_last,ax_last,ay_last,az_last,mx_last,my_last,mz_last);
                    String output = "[q0:" + String.format("%.8f", mySensorEventListener.q0) + "\n               q1:" + String.format("%.8f", mySensorEventListener.q1) + "\n               q2:" + String.format("%.8f", mySensorEventListener.q2)+ "\n               q3:" + String.format("%.8f", mySensorEventListener.q3) + "]";
//                    Log.d("四元数",output);
                    String mq0123String = "[四元数]:";
                    mq0123String += output;
                    mq0123TextView.setText(mq0123String);
                }
                // 网络通信，传输数据
                //方法二、用多线程的方式
                    new WorkThread().start();

                i = 1;
            } else {
                i++;
            }
        }
        //String.format("%.2f", pi)

        @Override
        public void onAccuracyChanged(Sensor sensor, int accuracy) {
            Log.d(TAG, "onAccuracyChanged:" + sensor.getType() + "->" + accuracy);
        }

        private void delay(int ms) {
            try {
                Thread.currentThread();
                Thread.sleep(ms);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }

        private void MahonyAHRSupdateIMU(float gx, float gy, float gz, float ax, float ay, float az) {
            float halfvx, halfvy, halfvz;
            float halfex, halfey, halfez;
            float qa, qb, qc;

            //如果加速计各轴的数均是0，那么忽略该加速度数据。否则在加速计数据归一化处理的时候，会导致除以0的错误。
            // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
            float recipNorm;
            if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {

                //把加速度计的数据进行归一化处理。其中invSqrt是平方根的倒数，使用平方根的倒数而不是直接使用平方根的原因是使得下面的ax，ay，az的运算速度更快。通过归一化处理后，ax，ay，az的数值范围变成-1到+1之间。
                // Normalise accelerometer measurement
                recipNorm = invSqrt(ax * ax + ay * ay + az * az);
                ax *= recipNorm;
                ay *= recipNorm;
                az *= recipNorm;

                //根据当前四元数的姿态值来估算出各重力分量。用于和加速计实际测量出来的各重力分量进行对比，从而实现对四轴姿态的修正。
                // Estimated direction of gravity and vector perpendicular to magnetic flux
                halfvx = q1 * q3 - q0 * q2;
                halfvy = q0 * q1 + q2 * q3;
                halfvz = q0 * q0 - 0.5f + q3 * q3;

                //使用叉积来计算估算的重力和实际测量的重力这两个重力向量之间的误差。
                // Error is sum of cross product between estimated and measured direction of gravity
                halfex = (ay * halfvz - az * halfvy);
                halfey = (az * halfvx - ax * halfvz);
                halfez = (ax * halfvy - ay * halfvx);

                //把上述计算得到的重力差进行积分运算，积分的结果累加到陀螺仪的数据中，用于修正陀螺仪数据。积分系数是Ki，如果Ki参数设置为0，则忽略积分运算。
                // Compute and apply integral feedback if enabled
                if (twoKi > 0.0f) {
                    integralFBx += twoKi * halfex * (1.0f / sampleFreq); // integral error scaled by Ki
                    integralFBy += twoKi * halfey * (1.0f / sampleFreq);
                    integralFBz += twoKi * halfez * (1.0f / sampleFreq);
                    gx += integralFBx;                                  // apply integral feedback
                    gy += integralFBy;
                    gz += integralFBz;
                } else {
                    integralFBx = 0.0f;                                 // prevent integral windup
                    integralFBy = 0.0f;
                    integralFBz = 0.0f;
                }

                //把上述计算得到的重力差进行比例运算。比例的结果累加到陀螺仪的数据中，用于修正陀螺仪数据。比例系数为Kp。
                // Apply proportional feedback
                gx += twoKp * halfex;
                gy += twoKp * halfey;
                gz += twoKp * halfez;
            }
            //通过上述的运算，我们得到了一个由加速计修正过后的陀螺仪数据。接下来要做的就是把修正过后的陀螺仪数据整合到四元数中。
            // Integrate rate of change of quaternion
            gx *= (0.5f * (1.0f / sampleFreq));                           // pre-multiply common factors
            gy *= 0.5f * (1.0f / sampleFreq);
            gz *= (0.5f * (1.0f / sampleFreq));
            qa = q0;
            qb = q1;
            qc = q2;
            q0 += (-qb * gx - qc * gy - q3 * gz);
            q1 += (qa * gx + qc * gz - q3 * gy);
            q2 += (qa * gy - qb * gz + q3 * gx);
            q3 += (qa * gz + qb * gy - qc * gx);


            //把上述运算后的四元数进行归一化处理。得到了物体经过旋转后的新的四元数。
            // Normalise quaternion
            recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);
            q0 *= recipNorm;
            q1 *= recipNorm;
            q2 *= recipNorm;
            q3 *= recipNorm;
        }

        private void MahonyAHRSupdate(float gx, float gy, float gz, float ax, float ay, float az, float mx, float my, float mz) {
            float recipNorm = 0;
            float q0q0, q0q1, q0q2, q0q3, q1q1, q1q2, q1q3, q2q2, q2q3, q3q3;
            float hx, hy, bx, bz;
            float halfvx, halfvy, halfvz, halfwx, halfwy, halfwz;
            float halfex, halfey, halfez;
            float qa, qb, qc;

            //如果地磁传感器各轴的数均是0，那么忽略该地磁数据。否则在地磁数据归一化处理的时候，会导致除以0的错误。
            // Use IMU algorithm if magnetometer measurement invalid (avoids NaN in magnetometer normalisation)
            if ((mx == 0.0f) && (my == 0.0f) && (mz == 0.0f)) {
                MahonyAHRSupdateIMU(gx, gy, gz, ax, ay, az);
                return;
            }
            //如果加速计各轴的数均是0，那么忽略该加速度数据。否则在加速计数据归一化处理的时候，会导致除以0的错误。
            // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
            if (!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f))) {
                //把加速度计的数据进行归一化处理。
                // Normalise accelerometer measurement
                recipNorm = invSqrt(ax * ax + ay * ay + az * az);
                ax *= recipNorm;
                ay *= recipNorm;
                az *= recipNorm;

                //把地磁的数据进行归一化处理。
                // Normalise magnetometer measurement
                recipNorm = invSqrt(mx * mx + my * my + mz * mz);
                mx *= recipNorm;
                my *= recipNorm;
                mz *= recipNorm;

                //预先进行四元数数据运算，以避免重复运算带来的效率问题。
                // Auxiliary variables to avoid repeated arithmetic
                q0q0 = q0 * q0;
                q0q1 = q0 * q1;
                q0q2 = q0 * q2;
                q0q3 = q0 * q3;
                q1q1 = q1 * q1;
                q1q2 = q1 * q2;
                q1q3 = q1 * q3;
                q2q2 = q2 * q2;
                q2q3 = q2 * q3;
                q3q3 = q3 * q3;

                // Reference direction of Earth’s magnetic field
                hx = 2.0f * (mx * (0.5f - q2q2 - q3q3) + my * (q1q2 - q0q3) + mz * (q1q3 + q0q2));
                hy = 2.0f * (mx * (q1q2 + q0q3) + my * (0.5f - q1q1 - q3q3) + mz * (q2q3 - q0q1));
                bx = (float) Math.sqrt(hx * hx + hy * hy);
                bz = 2.0f * (mx * (q1q3 - q0q2) + my * (q2q3 + q0q1) + mz * (0.5f - q1q1 - q2q2));

                //根据当前四元数的姿态值来估算出各重力分量Vx，Vy，Vz和各地磁分量Wx，Wy，Wz。
                // Estimated direction of gravity and magnetic field
                halfvx = q1q3 - q0q2;
                halfvy = q0q1 + q2q3;
                halfvz = q0q0 - 0.5f + q3q3;
                halfwx = bx * (0.5f - q2q2 - q3q3) + bz * (q1q3 - q0q2);
                halfwy = bx * (q1q2 - q0q3) + bz * (q0q1 + q2q3);
                halfwz = bx * (q0q2 + q1q3) + bz * (0.5f - q1q1 - q2q2);

                //使用叉积来计算重力和地磁误差。
                // Error is sum of cross product between estimated direction and measured direction of field vectors
                halfex = (ay * halfvz - az * halfvy) + (my * halfwz - mz * halfwy);
                halfey = (az * halfvx - ax * halfvz) + (mz * halfwx - mx * halfwz);
                halfez = (ax * halfvy - ay * halfvx) + (mx * halfwy - my * halfwx);
//                Log.d("halfex",Float.toString(halfex));
//                Log.d("halfey",Float.toString(halfey));
//                Log.d("halfez",Float.toString(halfez));

                //把上述计算得到的重力和磁力差进行积分运算，
                // Compute and apply integral feedback if enabled
                if (twoKi > 0.0f) {
                    integralFBx += twoKi * halfex * (1.0f / sampleFreq); // integral error scaled by Ki
                    integralFBy += twoKi * halfey * (1.0f / sampleFreq);
                    integralFBz += twoKi * halfez * (1.0f / sampleFreq);
                    gx += integralFBx; // apply integral feedback
                    gy += integralFBy;
                    gz += integralFBz;
                } else {
                    integralFBx = 0.0f; // prevent integral windup
                    integralFBy = 0.0f;
                    integralFBz = 0.0f;
                }

                //把上述计算得到的重力差和磁力差进行比例运算。
                // Apply proportional feedback
                gx += twoKp * halfex;
                gy += twoKp * halfey;
                gz += twoKp * halfez;
            }

            //把由加速计和磁力计修正过后的陀螺仪数据整合到四元数中。
            // Integrate rate of change of quaternion
            gx *= (0.5f * (1.0f / sampleFreq)); // pre-multiply common factors
            gy *= (0.5f * (1.0f / sampleFreq));
            gz *= (0.5f * (1.0f / sampleFreq));
            qa = q0;
            qb = q1;
            qc = q2;
            q0 += (-qb * gx - qc * gy - q3 * gz);
            q1 += (qa * gx + qc * gz - q3 * gy);
            q2 += (qa * gy - qb * gz + q3 * gx);
            q3 += (qa * gz + qb * gy - qc * gx);
//            Log.d("q0",Float.toString(q0));
//            Log.d("q1",Float.toString(q1));
//            Log.d("q2",Float.toString(q2));
//            Log.d("q3",Float.toString(q3));
            //把上述运算后的四元数进行归一化处理。得到了物体经过旋转后的新的四元数。
            // Normalise quaternion
            recipNorm = invSqrt(q0 * q0 + q1 * q1 + q2 * q2 + q3 * q3);

            q0 *= recipNorm;
            q1 *= recipNorm;
            q2 *= recipNorm;
            q3 *= recipNorm;
        }

        private float invSqrt(float x) {
            float xhalf = 0.5f * x;
            int i = Float.floatToIntBits(x);
            i = 0x5f3759df - (i >> 1);
            x = Float.intBitsToFloat(i);
            x *= (1.5f - xhalf * x * x);
            return x;
        }

    }
}






