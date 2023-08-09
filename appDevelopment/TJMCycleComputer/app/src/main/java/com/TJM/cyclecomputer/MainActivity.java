package com.TJM.cyclecomputer;


import static java.lang.Boolean.TRUE;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

// import needed items from spotify app remote, allows user to connect
import com.spotify.android.appremote.api.ConnectionParams;
import com.spotify.android.appremote.api.Connector;
import com.spotify.android.appremote.api.SpotifyAppRemote;

// import android bluetooth serial library
import com.harrysoft.androidbluetoothserial.SimpleBluetoothDeviceInterface;
import com.harrysoft.androidbluetoothserial.BluetoothManager;
import com.harrysoft.androidbluetoothserial.BluetoothSerialDevice;

import android.bluetooth.BluetoothDevice;
import android.content.pm.PackageManager;
import android.util.Log;  // import log functionality for debugging

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import java.util.Collection;
import java.util.Objects;

import io.reactivex.android.schedulers.AndroidSchedulers;
import io.reactivex.disposables.CompositeDisposable;
import io.reactivex.schedulers.Schedulers;

public class MainActivity extends AppCompatActivity {

    private static final String CLIENT_ID = "ec8291b4392a490b89d69e39556b5899";
    private static final String REDIRECT_URI = "com.TJM.cyclecomputer://callback";
    private SpotifyAppRemote mSpotifyAppRemote;

    boolean isPaused;
    public String songName;
    public String artistName;
    public long timeElapsed;
    public long totalTimeMillis;

    String BTName;
    String BTmac;
    private final CompositeDisposable compositeDisposable = new CompositeDisposable();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    @Override
    protected void onStart() {
        super.onStart();
        // Set the Connection Parameters
        ConnectionParams connectionParams =
                new ConnectionParams.Builder(CLIENT_ID)
                        .setRedirectUri(REDIRECT_URI)
                        .showAuthView(true)
                        .build();

        // Connect to App Remote
        SpotifyAppRemote.connect(this, connectionParams,
                new Connector.ConnectionListener() {

                    @Override
                    public void onConnected(SpotifyAppRemote spotifyAppRemote) {
                        mSpotifyAppRemote = spotifyAppRemote;
                        Log.d("MainActivity", "Connected! Yay!");

                        // Now interactions can be made with App Remote
                        connected();
                    }

                    @Override
                    public void onFailure(Throwable throwable) {
                        Log.e("MainActivity", throwable.getMessage(), throwable);

                        // Something went wrong with connection! Handle errors here
                    }
                });
    }

    @Override
    protected void onStop () {
        super.onStop();
        SpotifyAppRemote.disconnect(mSpotifyAppRemote);
    }

    // When Play/Pause is clicked
    public void onToggleClick (View view){
        if (isPaused == TRUE) {
            mSpotifyAppRemote.getPlayerApi().resume();
        } else {
            mSpotifyAppRemote.getPlayerApi().pause();
        }
    }

    // When Skip forward is clicked
    public void onSkipClick (View view){
        mSpotifyAppRemote.getPlayerApi().skipNext();
    }

    // When Skip backward is clicked
    public void onRewindClick (View view){
        mSpotifyAppRemote.getPlayerApi().skipPrevious();
    }

    // When connected to Bluetooth is Clicked
    public void onConnectBluetoothClick (View view) {
            setupBT();
    }

    // When Spotify Player is Connected
    private void connected() {
        // Code Here
        // Subscribe to PlayerState
        mSpotifyAppRemote.getPlayerApi()
                .subscribeToPlayerState()
                .setEventCallback(playerState -> {
                    isPaused = playerState.isPaused;
                    songName = playerState.track.name;
                    artistName = playerState.track.artist.name;
                    totalTimeMillis = playerState.track.duration;

                    updateMetadata();
                });
    }

    // Update Metadata displayed in app
    public void updateMetadata() {
        long minutes = (totalTimeMillis / 1000)  / 60;
        int seconds = (int)((totalTimeMillis / 1000) % 60);
        String totalTime = minutes+":"+seconds;

        Log.d("MainActivity", ("name:" + songName + "  artist:" + artistName +
                "  timeElapsed:" + timeElapsed + "  totalTime:" + totalTime));

        TextView dispSongName = findViewById(R.id.songName);
        dispSongName.setText(songName);

        TextView dispArtistName = findViewById(R.id.artistName);
        dispArtistName.setText(artistName);

        TextView dispTotalTime = findViewById(R.id.totalTime);
        dispTotalTime.setText(totalTime);
    }

    //
    public void setupBT() {
        // Check for app Bluetooth Permission
        String[] requestConnectPermission = {android.Manifest.permission.BLUETOOTH_CONNECT};
        if (ActivityCompat.checkSelfPermission(this, android.Manifest.permission.BLUETOOTH_CONNECT) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, requestConnectPermission, 1);
            return;
        }
        String[] requestScanPermission = {android.Manifest.permission.BLUETOOTH_SCAN};
        if (ActivityCompat.checkSelfPermission(this, android.Manifest.permission.BLUETOOTH_SCAN) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, requestScanPermission, 2);
            return;
        }

        // Setup Bluetooth Manager
        BluetoothManager bluetoothManager = BluetoothManager.getInstance();
        boolean bluetoothAvailable = getPackageManager().hasSystemFeature(PackageManager.FEATURE_BLUETOOTH);
        if (!bluetoothAvailable) {
            // Bluetooth is not available on this mobile device, tell the user
            Toast.makeText(this, "Bluetooth not available on this device.", Toast.LENGTH_LONG).show();
            finish();
        }

        // show list of paired bluetooth devices
        try {
            Collection<BluetoothDevice> pairedDevices = bluetoothManager.getPairedDevicesList();
            for (BluetoothDevice device : pairedDevices) {
                Log.d("My Bluetooth App", device.getName());
                Log.d("My Bluetooth App", device.getAddress());

                if (Objects.equals(device.getName(), "ESP32testAA")) {
                    Log.d("My Bluetooth App", "Device Paired! :0");
                    BTName = device.getName();
                    BTmac = device.getAddress();
                    break;
                }
            }
        } catch (Exception e) {
            // Handle Error
            Toast.makeText(this, "No Bluetooth Devices Paired", Toast.LENGTH_LONG).show();
            Log.d("My Bluetooth App", "Error: " + e);
        }
        // Call method to connect
        compositeDisposable.add(bluetoothManager.openSerialDevice(BTmac)
                .subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(this::onBTConnected, this::onBTError));
    }

    private void onBTConnected(BluetoothSerialDevice connectedDevice) {
        // Code
        SimpleBluetoothDeviceInterface deviceInterface = connectedDevice.toSimpleDeviceInterface();  // Keep instance of device
        deviceInterface.setListeners(this::onBTMessageReceived, this::onBTMessageSent, this::onBTError);  // Listen to bluetooth events
        Log.d("My Bluetooth App", "Successfully Connected");
        deviceInterface.sendMessage("Successfully Connected!");  // Send confirmation message

        TextView deviceName = findViewById(R.id.deviceName);
        TextView deviceMac = findViewById(R.id.deviceMac);
        deviceName.setText("Device Name: " + BTName);
        deviceMac.setText("Device Address: " + BTmac);
    }

    private void onBTMessageReceived(String message) {
        Toast.makeText(this, "Received a message! Message was: " + message, Toast.LENGTH_LONG).show();

        if(Objects.equals(message, "Toggle")){
            if (isPaused == TRUE) {
                mSpotifyAppRemote.getPlayerApi().resume();
            } else {
                mSpotifyAppRemote.getPlayerApi().pause();
            }
        } else if (Objects.equals(message, "Skip Next")){
            mSpotifyAppRemote.getPlayerApi().skipNext();
        } else if (Objects.equals(message, "Skip Previous")) {
            mSpotifyAppRemote.getPlayerApi().skipPrevious();
        }
    }

    private void onBTMessageSent(String message) {
        Toast.makeText(this, "Sent a message! Message was: " + message, Toast.LENGTH_LONG).show();
    }

    private void onBTError(Throwable error){
        // Code
        Log.d("My Bluetooth App", "error: " + error);
    }
}