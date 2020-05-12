package com.arrow.Mapbox;

import android.content.SharedPreferences;
import android.location.Location;
import android.os.Bundle;
import android.os.Parcelable;
import android.preference.PreferenceManager;
import android.util.Log;
import androidx.annotation.Nullable;
import com.arrow.R;
import com.facebook.react.ReactActivity;
import com.facebook.react.bridge.Arguments;
import com.facebook.react.bridge.WritableMap;
import com.facebook.react.modules.core.DeviceEventManagerModule;
import com.mapbox.api.directions.v5.models.DirectionsRoute;
import com.mapbox.mapboxsdk.camera.CameraPosition;
import com.mapbox.services.android.navigation.ui.v5.MapOfflineOptions;
import com.mapbox.services.android.navigation.ui.v5.NavigationView;
import com.mapbox.services.android.navigation.ui.v5.NavigationViewOptions;
import com.mapbox.services.android.navigation.ui.v5.OnNavigationReadyCallback;
import com.mapbox.services.android.navigation.ui.v5.listeners.NavigationListener;
import com.mapbox.services.android.navigation.v5.navigation.MapboxNavigationOptions;
import com.mapbox.services.android.navigation.v5.navigation.NavigationConstants;
import com.mapbox.services.android.navigation.v5.routeprogress.ProgressChangeListener;
import com.mapbox.services.android.navigation.v5.routeprogress.RouteProgress;
import com.mapbox.services.android.navigation.v5.utils.abbreviation.StringAbbreviator;

public class MapNavActivity extends ReactActivity implements OnNavigationReadyCallback,
        NavigationListener, ProgressChangeListener {

    private NavigationView navigationView;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        setTheme(R.style.Theme_AppCompat_NoActionBar);
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_navigation);
        navigationView = findViewById(R.id.navigationView);
        navigationView.onCreate(savedInstanceState);
        initialize();
    }

    private void sendEvent(String eventName,
                           WritableMap params) {

        try {
            while (getReactInstanceManager().getCurrentReactContext() == null) ;  // Busy wait.
            getReactInstanceManager().getCurrentReactContext()
                    .getJSModule(DeviceEventManagerModule.RCTDeviceEventEmitter.class)
                    .emit(eventName, params);
        } catch (Exception e){
            Log.e("ReactNative", "Caught Exception: " + e.getMessage());
        }
    }

    @Override
    public void onProgressChange(Location location, RouteProgress routeProgress){
        //ETA
        //MIN REMAINING
        //TOTAL DISTANCE REMAINING
        //Current Road
        //Next Manuever
        //Next manuever Distance
        //seconday manuever if there is any
        String direction;

        double change = Math.abs(routeProgress.currentLegProgress().upComingStep().maneuver().bearingAfter() - routeProgress.currentLegProgress().upComingStep().maneuver().bearingBefore());
        if (change > 180) {
            direction = "Left";
        }else if(change < 180){
            direction = "Right";
        }else{
            direction = "U-Turn";
        }
        //Log.i("VALUE",Double.toString(change));
        WritableMap params = Arguments.createMap();
        params.putString("nextStepInstruction", direction);
        params.putDouble("nextStepDistance", routeProgress.currentLegProgress().currentStepProgress().distanceRemaining() * 0.00062137119);
        params.putString("nextStepName", routeProgress.currentLegProgress().upComingStep().name());
        params.putDouble("distanceRemaining", routeProgress.distanceRemaining() * 0.00062137119);
        params.putDouble("TimeRemaining", routeProgress.durationRemaining()/60);
        sendEvent("Navigation", params);
    }

    @Override
    public void onStart() {
        super.onStart();
        navigationView.onStart();
    }

    @Override
    public void onResume() {
        super.onResume();
        navigationView.onResume();
    }

    @Override
    public void onLowMemory() {
        super.onLowMemory();
        navigationView.onLowMemory();
    }

    @Override
    public void onBackPressed() {
        // If the navigation view didn't need to do anything, call super
        if (!navigationView.onBackPressed()) {
            super.onBackPressed();
        }
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        navigationView.onSaveInstanceState(outState);
        super.onSaveInstanceState(outState);
    }

    @Override
    protected void onRestoreInstanceState(Bundle savedInstanceState) {
        super.onRestoreInstanceState(savedInstanceState);
        navigationView.onRestoreInstanceState(savedInstanceState);
    }

    @Override
    public void onPause() {
        super.onPause();
        navigationView.onPause();
    }

    @Override
    public void onStop() {
        super.onStop();
        navigationView.onStop();
        finishNavigation();
        WritableMap params = Arguments.createMap();
        params.putBoolean("navigationCancelled", true);
        sendEvent("NavCancel", params);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        navigationView.onDestroy();
    }

    @Override
    public void onNavigationReady(boolean isRunning) {
        NavigationViewOptions.Builder options = NavigationViewOptions.builder();
        options.navigationListener(this).progressChangeListener(this);
        extractRoute(options);
        extractConfiguration(options);
        options.navigationOptions(MapboxNavigationOptions.builder().build());
        navigationView.startNavigation(options.build());
    }

    @Override
    public void onCancelNavigation() {
        finishNavigation();
        WritableMap params = Arguments.createMap();
        params.putBoolean("navigationCancelled", true);
        sendEvent("NavCancel", params);
    }

    @Override
    public void onNavigationFinished() {
        finishNavigation();
    }

    @Override
    public void onNavigationRunning() {
        // Intentionally empty
    }

    private void initialize() {
        Parcelable position = getIntent().getParcelableExtra(NavigationConstants.NAVIGATION_VIEW_INITIAL_MAP_POSITION);
        if (position != null) {
            navigationView.initialize(this, (CameraPosition) position);
        } else {
            navigationView.initialize(this);
        }
    }

    private void extractRoute(NavigationViewOptions.Builder options) {
        DirectionsRoute route = NavLauncher.extractRoute(this);
        options.directionsRoute(route);
    }

    private void extractConfiguration(NavigationViewOptions.Builder options) {
        SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(this);
        options.shouldSimulateRoute(preferences.getBoolean(NavigationConstants.NAVIGATION_VIEW_SIMULATE_ROUTE, false));
        String offlinePath = preferences.getString(NavigationConstants.OFFLINE_PATH_KEY, "");
        if (!offlinePath.isEmpty()) {
            options.offlineRoutingTilesPath(offlinePath);
        }
        String offlineVersion = preferences.getString(NavigationConstants.OFFLINE_VERSION_KEY, "");
        if (!offlineVersion.isEmpty()) {
            options.offlineRoutingTilesVersion(offlineVersion);
        }
        String offlineMapDatabasePath = preferences.getString(NavigationConstants.MAP_DATABASE_PATH_KEY, "");
        String offlineMapStyleUrl = preferences.getString(NavigationConstants.MAP_STYLE_URL_KEY, "");
        if (!offlineMapDatabasePath.isEmpty() && !offlineMapStyleUrl.isEmpty()) {
            MapOfflineOptions mapOfflineOptions = new MapOfflineOptions(offlineMapDatabasePath, offlineMapStyleUrl);
            options.offlineMapOptions(mapOfflineOptions);
        }
    }

    private void finishNavigation() {
        NavLauncher.cleanUpPreferences(this);
        finish();
    }
}