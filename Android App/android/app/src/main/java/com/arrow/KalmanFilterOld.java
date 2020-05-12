package com.arrow;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.hardware.SensorManager;
import android.location.Location;
import android.location.LocationManager;
import android.os.Environment;
import android.util.Log;

import androidx.core.app.ActivityCompat;

import com.elvishew.xlog.LogLevel;
import com.elvishew.xlog.XLog;
import com.elvishew.xlog.printer.AndroidPrinter;
import com.elvishew.xlog.printer.Printer;
import com.elvishew.xlog.printer.file.FilePrinter;
import com.elvishew.xlog.printer.file.backup.FileSizeBackupStrategy;
import com.elvishew.xlog.printer.file.naming.FileNameGenerator;
import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.LifecycleEventListener;
import com.facebook.react.bridge.ReactApplicationContext;
import com.facebook.react.bridge.ReactContextBaseJavaModule;
import com.facebook.react.bridge.ReactMethod;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.modules.core.DeviceEventManagerModule;

import java.io.File;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Locale;
import java.util.TimeZone;
import java.util.Timer;

import mad.location.manager.lib.Commons.Utils;
import mad.location.manager.lib.Interfaces.ILogger;
import mad.location.manager.lib.Interfaces.LocationServiceInterface;
import mad.location.manager.lib.Loggers.GeohashRTFilter;
import mad.location.manager.lib.SensorAux.SensorCalibrator;
import mad.location.manager.lib.Services.KalmanLocationService;
import mad.location.manager.lib.Services.ServicesHelper;
import timber.log.Timber;


public class KalmanFilterOld extends ReactContextBaseJavaModule implements LocationServiceInterface, ILogger {

    private final ReactApplicationContext reactContext;
    private GeohashRTFilter m_geoHashRTFilter;
    private SensorCalibrator m_sensorCalibrator = null;
    private boolean m_isLogging = false;
    private boolean m_isCalibrating = false;
    private GPSDataLogger m_logger;



    public KalmanFilterOld(ReactApplicationContext reactContext) {
        super(reactContext);
        this.reactContext = reactContext;
    }

    @Override
    public String getName() {
        return "KalmanFilter";
    }

    private void initActivity() {

        String[] interestedPermissions;
        interestedPermissions = new String[]{
                Manifest.permission.ACCESS_FINE_LOCATION,
                Manifest.permission.WRITE_EXTERNAL_STORAGE,
                Manifest.permission.READ_EXTERNAL_STORAGE
        };

        ArrayList<String> lstPermissions = new ArrayList<>(interestedPermissions.length);
        for (String perm : interestedPermissions) {
            if (ActivityCompat.checkSelfPermission(reactContext, perm) != PackageManager.PERMISSION_GRANTED) {
                lstPermissions.add(perm);
            }
        }

        if (!lstPermissions.isEmpty()) {
            ActivityCompat.requestPermissions(reactContext.getCurrentActivity(), lstPermissions.toArray(new String[0]),
                    100);
        }

        SensorManager sensorManager = (SensorManager) reactContext.getSystemService(Context.SENSOR_SERVICE);
        LocationManager locationManager = (LocationManager) reactContext.getSystemService(Context.LOCATION_SERVICE);
        m_logger = new GPSDataLogger(locationManager, reactContext, m_geoHashRTFilter);

        if (sensorManager == null || locationManager == null) {
            System.exit(1);
        }



        m_sensorCalibrator = new SensorCalibrator(sensorManager);
        ServicesHelper.getLocationService(reactContext, value -> {
            set_isLogging(value.IsRunning());
        });
        set_isCalibrating(false, true);
    }


    @ReactMethod
    public void startService() {
        set_isLogging(true);
    }

    @ReactMethod
    public void endService(){
        set_isLogging(false);
    }

    @Override
    public void locationChanged(Location loc){
        Log.i("KALMAN","LOCATION CHANGED");
        if (m_logger != null){
            m_logger.locationChanged(loc);
        }

    }

    private String xLogFolderPath;
    private static SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd", Locale.US);
    class ChangableFileNameGenerator implements FileNameGenerator {
        private String fileName;
        public void setFileName(String fileName) {
            this.fileName = fileName;
        }
        public ChangableFileNameGenerator() {
        }
        @Override
        public boolean isFileNameChangeable() {
            return true;
        }
        @Override
        public String generateFileName(int logLevel, long timestamp) {
            return fileName;
        }
    }

    ChangableFileNameGenerator xLogFileNameGenerator = new ChangableFileNameGenerator();
    public void initXlogPrintersFileName() {
        sdf.setTimeZone(TimeZone.getDefault());
        String dateStr = sdf.format(System.currentTimeMillis());
        String fileName = dateStr;
        final int secondsIn24Hour = 86400; //I don't think that it's possible to press button more frequently
        for (int i = 0; i < secondsIn24Hour; ++i) {
            fileName = String.format("%s_%d", dateStr, i);
            File f = new File(xLogFolderPath, fileName);
            if (!f.exists())
                break;
        }
        xLogFileNameGenerator.setFileName(fileName);
    }

    @Override
    public void log2file(String format, Object... args) {
        XLog.i(format, args);
    }

    protected void onCreate() {


        m_geoHashRTFilter = new GeohashRTFilter(Utils.GEOHASH_DEFAULT_PREC, Utils.GEOHASH_DEFAULT_MIN_POINT_COUNT);
        ServicesHelper.addLocationServiceInterface(this);


        File esd = Environment.getExternalStorageDirectory();
        String storageState = Environment.getExternalStorageState();
        if (storageState != null && storageState.equals(Environment.MEDIA_MOUNTED)) {
            xLogFolderPath = String.format("%s/%s/", esd.getAbsolutePath(), "SensorDataCollector");
            Printer androidPrinter = new AndroidPrinter();             // Printer that print the log using android.util.Log
            initXlogPrintersFileName();
            Printer xLogFilePrinter = new FilePrinter
                    .Builder(xLogFolderPath)
                    .fileNameGenerator(xLogFileNameGenerator)
                    .backupStrategy(new FileSizeBackupStrategy(1024 * 1024 * 100)) //100MB for backup files
                    .build();
            XLog.init(LogLevel.ALL, androidPrinter, xLogFilePrinter);
        } else {
            //todo set some status
        }

        initActivity();
    }


    private void set_isLogging(boolean isLogging) {

        if (isLogging) {
            m_logger.stop();
            m_logger.start();
            m_geoHashRTFilter.stop();
            m_geoHashRTFilter.reset(this);
            ServicesHelper.getLocationService(reactContext, value -> {
                if (value.IsRunning()) {
                    return;
                }
                value.stop();
                initXlogPrintersFileName();
                KalmanLocationService.Settings settings =
                        new KalmanLocationService.Settings(
                                Utils.ACCELEROMETER_DEFAULT_DEVIATION,
                                0,
                                2000,
                                6,
                                2,
                                10,
                                this,
                                true,
                                Utils.DEFAULT_VEL_FACTOR,
                                Utils.DEFAULT_POS_FACTOR
                        );
                value.reset(settings); //warning!! here you can adjust your filter behavior
                value.start();
            });

        } else {
            m_logger.stop();
            ServicesHelper.getLocationService(reactContext, KalmanLocationService::stop);
        }
        m_isLogging = isLogging;
    }

    private void set_isCalibrating(boolean isCalibrating, boolean byUser) {

        if (isCalibrating) {

            m_sensorCalibrator.reset();
            m_sensorCalibrator.start();
        } else {

            m_sensorCalibrator.stop();
        }

        m_isCalibrating = isCalibrating;
    }




}
