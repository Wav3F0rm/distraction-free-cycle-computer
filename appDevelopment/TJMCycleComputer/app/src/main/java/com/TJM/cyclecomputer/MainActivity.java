package com.TJM.cyclecomputer;

import static java.lang.Boolean.TRUE;

import androidx.appcompat.app.AppCompatActivity;

// import needed items from spotify app remote, allows user to connect
import com.spotify.android.appremote.api.ConnectionParams;
import com.spotify.android.appremote.api.Connector;
import com.spotify.android.appremote.api.SpotifyAppRemote;

import android.util.Log;  // import log functionality for debugging

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    private static final String CLIENT_ID = "ec8291b4392a490b89d69e39556b5899";
    private static final String REDIRECT_URI = "com.TJM.cyclecomputer://callback";
    private SpotifyAppRemote mSpotifyAppRemote;

    public boolean isPaused;
    public String songName;
    public String artistName;
    public long timeElapsed;
    public long totalTimeMillis;

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

    private void connected() {
        // Code Here
        //mSpotifyAppRemote.getPlayerApi().play("spotify:track:6nFYXpBgrNcZjbtNEuc6yR");

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

    @Override
    protected void onStop () {
        super.onStop();
        SpotifyAppRemote.disconnect(mSpotifyAppRemote);
    }

    public void onToggleClick (View view){
        if (isPaused == TRUE) {
            mSpotifyAppRemote.getPlayerApi().resume();
        } else {
            mSpotifyAppRemote.getPlayerApi().pause();
        }
    }

    public void onSkipClick (View view){
        mSpotifyAppRemote.getPlayerApi().skipNext();
    }

    public void onRewindClick (View view){
        mSpotifyAppRemote.getPlayerApi().skipPrevious();
    }

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
}