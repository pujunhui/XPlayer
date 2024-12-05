package com.pujh.example

import android.content.Intent
import android.os.Build
import android.os.Bundle
import android.os.Environment
import android.provider.Settings
import android.view.Menu
import android.view.MenuItem
import android.view.SurfaceHolder
import android.widget.Toast
import androidx.activity.enableEdgeToEdge
import androidx.appcompat.app.AppCompatActivity
import androidx.core.view.ViewCompat
import androidx.core.view.WindowInsetsCompat
import com.pujh.example.databinding.ActivityMainBinding
import com.pujh.xplayer.XPlayer

class MainActivity : AppCompatActivity(), SurfaceHolder.Callback {
    private lateinit var binding: ActivityMainBinding

    private lateinit var xPlayer: XPlayer
    private var isPlaying = false

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        enableEdgeToEdge()
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        ViewCompat.setOnApplyWindowInsetsListener(binding.root) { v, insets ->
            val systemBars = insets.getInsets(WindowInsetsCompat.Type.systemBars())
            v.setPadding(systemBars.left, systemBars.top, systemBars.right, systemBars.bottom)
            insets
        }

        setSupportActionBar(binding.toolbar)

        xPlayer = XPlayer(this)

        binding.surfaceView.holder.addCallback(this)
        binding.surfaceView.setOnClickListener {
            if (xPlayer.isPlaying) {
                xPlayer.pause()
            } else if (xPlayer.isPaused) {
                xPlayer.resume()
            }
        }

        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
            if (!Environment.isExternalStorageManager()) {
                val intent = Intent(Settings.ACTION_MANAGE_ALL_FILES_ACCESS_PERMISSION)
                startActivityForResult(intent, 0)
            }
        }
    }

    override fun onActivityResult(requestCode: Int, resultCode: Int, data: Intent?) {
        super.onActivityResult(requestCode, resultCode, data)
        if (requestCode == 0) {
            if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.R) {
                if (Environment.isExternalStorageManager()) {
                    Toast.makeText(this, "已获取MANAGE_EXTERNAL_STORAGE权限", Toast.LENGTH_SHORT)
                        .show()
                } else {
                    Toast.makeText(this, "未获取MANAGE_EXTERNAL_STORAGE权限", Toast.LENGTH_SHORT)
                        .show()
                    finish()
                }
            }
        }
    }

    override fun onCreateOptionsMenu(menu: Menu?): Boolean {
        menuInflater.inflate(R.menu.toolbar, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        if (item.itemId == R.id.open_btn) {
            xPlayer.setDataSource("/sdcard/1080.mp4")
            xPlayer.prepare()
            xPlayer.start()
            isPlaying = true
            return true
        }
        return super.onOptionsItemSelected(item)
    }

    override fun onRestart() {
        super.onRestart()
        if (xPlayer.isPaused) {
            xPlayer.resume()
        }
    }

    override fun onStop() {
        super.onStop()
        if (xPlayer.isPlaying) {
            xPlayer.pause()
        }
    }

    override fun onDestroy() {
        super.onDestroy()
        xPlayer.release()
    }

    override fun surfaceCreated(holder: SurfaceHolder) {
        xPlayer.setSurface(holder.surface)
    }

    override fun surfaceChanged(
        holder: SurfaceHolder,
        format: Int,
        width: Int,
        height: Int
    ) {
        xPlayer.setDisplaySize(width, height)
    }

    override fun surfaceDestroyed(holder: SurfaceHolder) {
        xPlayer.setSurface(null)
    }
}