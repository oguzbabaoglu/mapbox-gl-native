package com.mapbox.mapboxsdk.testapp;

import android.os.Bundle;
import android.support.design.widget.FloatingActionButton;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.Toolbar;
import android.view.MenuItem;
import android.view.View;
import android.widget.Button;
import android.widget.Toast;
import com.mapbox.mapboxsdk.camera.CameraPosition;
import com.mapbox.mapboxsdk.camera.CameraUpdateFactory;
import com.mapbox.mapboxsdk.constants.Style;
import com.mapbox.mapboxsdk.geometry.LatLng;
import com.mapbox.mapboxsdk.utils.ApiAccess;
import com.mapbox.mapboxsdk.views.MapView;

public class CameraActivity extends AppCompatActivity {

    private MapView mMapView;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_camera);

        Toolbar toolbar = (Toolbar) findViewById(R.id.toolbar);
        setSupportActionBar(toolbar);

        ActionBar actionBar = getSupportActionBar();
        if (actionBar != null) {
            actionBar.setDisplayHomeAsUpEnabled(true);
            actionBar.setDisplayShowHomeEnabled(true);
        }

        mMapView = (MapView) findViewById(R.id.cameraMapView);
        mMapView.setAccessToken(ApiAccess.getToken(this));
        mMapView.setStyle(Style.MAPBOX_STREETS);
        mMapView.setCompassEnabled(true);
        mMapView.onCreate(savedInstanceState);

        Button cameraButton = (Button) findViewById(R.id.cameraAnimateButton);
        cameraButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                CameraPosition cameraPosition = new CameraPosition.Builder()
                        .target(new LatLng(44.50128, -88.06216))    // Sets the center of the map to Lambeau Field
                        .zoom(14)                                   // Sets the zoom
                        .bearing(90)                                // Sets the orientation of the camera to east
                        .tilt(30)                                   // Sets the tilt of the camera to 30 degrees
                        .build();                                   // Creates a CameraPosition from the builder
                mMapView.animateCamera(CameraUpdateFactory.newCameraPosition(cameraPosition));
            }
        });

        Button cameraCallbackButton = (Button) findViewById(R.id.cameraAnimateCallbackButton);
        cameraCallbackButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {

                CameraPosition cameraPosition = new CameraPosition.Builder()
                        .target(new LatLng(48.21874, 11.62465))     // Sets the center of the map to Allianz Arena
                        .zoom(16)                                   // Sets the zoom
                        .bearing(180)                               // Sets the orientation of the camera to south
                        .tilt(40)                                   // Sets the tilt of the camera to 30 degrees
                        .build();                                   // Creates a CameraPosition from the builder

                MapView.CancelableCallback callback = new MapView.CancelableCallback() {
                    @Override
                    public void onCancel() {
                        // NOTE: This shouldn't appear
                        Toast.makeText(getApplicationContext(), "onCancel Callback called.", Toast.LENGTH_SHORT).show();
                    }

                    @Override
                    public void onFinish() {
                        Toast.makeText(getApplicationContext(), "onFinish Callback called.", Toast.LENGTH_SHORT).show();
                    }
                };

                mMapView.animateCamera(CameraUpdateFactory.newCameraPosition(cameraPosition), callback);

            }
        });

        Button cameraDurationButton = (Button) findViewById(R.id.cameraAnimateDurationButton);
        cameraDurationButton.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                CameraPosition cameraPosition = new CameraPosition.Builder()
                        .target(new LatLng(-22.91214, -43.23012))     // Sets the center of the map to Maracanã
                        .zoom(13)                                   // Sets the zoom
                        .bearing(270)                               // Sets the orientation of the camera to west
                        .tilt(20)                                   // Sets the tilt of the camera to 30 degrees
                        .build();                                   // Creates a CameraPosition from the builder


                MapView.CancelableCallback callback = new MapView.CancelableCallback() {
                    @Override
                    public void onCancel() {
                        Toast.makeText(getApplicationContext(), "Duration onCancel Callback called.", Toast.LENGTH_SHORT).show();
                    }

                    @Override
                    public void onFinish() {
                        Toast.makeText(getApplicationContext(), "Duration onFinish Callback called.", Toast.LENGTH_SHORT).show();
                    }
                };

                mMapView.animateCamera(CameraUpdateFactory.newCameraPosition(cameraPosition), 50000, callback);
            }
        });

    }

    @Override
    protected void onStart() {
        super.onStart();
        mMapView.onStart();
    }

    @Override
    public void onResume() {
        super.onResume();
        mMapView.onResume();
    }

    @Override
    public void onPause() {
        super.onPause();
        mMapView.onPause();
    }

    @Override
    protected void onStop() {
        super.onStop();
        mMapView.onStop();
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        mMapView.onSaveInstanceState(outState);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        mMapView.onDestroy();
    }

    @Override
    public void onLowMemory() {
        super.onLowMemory();
        mMapView.onLowMemory();
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
            case android.R.id.home:
                onBackPressed();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }
}
