package com.lxc.jni_fmod;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import org.fmod.FMOD;

public class MainActivity extends AppCompatActivity {
    private static final int MODE_NORMAL = 0; // 正常
    private static final int MODE_LUOLI = 1; // 萝莉
    private static final int MODE_DASHU = 2; // 大叔
    private static final int MODE_JINGSONG = 3; // 惊悚
    private static final int MODE_GAOGUAI = 4; // 搞怪
    private static final int MODE_KONGLING = 5; // 空灵

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    // 播放的路径
    private String path;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        path = "file:///android_asset/derry.mp3";
        FMOD.init(this);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        FMOD.close();
    }

    // 六个 点击事件
    public void onFix(View view) {
        switch (view.getId()) {
            case R.id.btn_normal:
                play(MODE_NORMAL, path);
                break;
            case R.id.btn_luoli:
                play(MODE_LUOLI, path);
                break;
            case R.id.btn_dashu:
                play(MODE_DASHU, path);
                break;
            case R.id.btn_jingsong:
                play(MODE_JINGSONG, path);
                break;
            case R.id.btn_gaoguai:
                play(MODE_GAOGUAI, path);
                break;
            case R.id.btn_kongling:
                play(MODE_KONGLING, path);
                break;
        }
    }

    private void play(int mode, String path) {
        new Thread(() -> voiceChangeNative(mode, path)).start();
    }

    private native void voiceChangeNative(int mode, String path);

    void playFinish(String msg) {
        runOnUiThread(() -> Toast.makeText(this, msg, Toast.LENGTH_SHORT).show());
    }

}