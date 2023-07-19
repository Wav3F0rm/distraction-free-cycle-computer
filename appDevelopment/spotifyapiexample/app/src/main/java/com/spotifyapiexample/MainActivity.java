package com.spotifyapiexample;

import androidx.appcompat.app.AppCompatActivity;

import com.spotify.android.appremote.api.ConnectionParams;
import com.spotify.android.appremote.api.Connector;
import com.spotify.android.appremote.api.SpotifyAppRemote;

import com.spotify.protocol.client.Subscription;
import com.spotify.protocol.types.PlayerState;
import com.spotify.protocol.types.Track;

import android.os.Bundle;
import android.util.Log;

public class MainActivity extends AppCompatActivity {

    private static final String CLIENT_ID = "ec8291b4392a490b89d69e39556b5899";
    private static final String REDIRECT_URI = "com.spotifyapiexample://callback";
    private SpotifyAppRemote mSpotifyAppRemote;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    @Override
    protected void onStart() {
        super.onStart();
        // Set the connection parameters
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

    private void connected() {
        // Code here
        mSpotifyAppRemote.getPlayerApi().play("spotify:album:1bwbZJ6khPJyVpOaqgKsoZ");
    }

    @Override
    protected void onStop() {
        super.onStop();
        // Code Here
    }
}