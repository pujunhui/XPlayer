package com.pujh.example

import android.os.Bundle
import android.view.Menu
import android.view.MenuItem
import android.view.SurfaceHolder
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

        binding.surfaceView.holder.addCallback(this)
        binding.surfaceView.setOnClickListener {
            if (xPlayer.isPlaying()) {
                xPlayer.pause()
            } else if (xPlayer.isPaused()) {
                xPlayer.resume()
            }
        }

        xPlayer = XPlayer(this)
    }

    override fun onCreateOptionsMenu(menu: Menu?): Boolean {
        menuInflater.inflate(R.menu.toolbar, menu)
        return true
    }

    override fun onOptionsItemSelected(item: MenuItem): Boolean {
        if (item.itemId == R.id.open_btn) {
            xPlayer.setDataSource("/sdcard/1080p.mp4")
            xPlayer.prepare()
            xPlayer.start()
            isPlaying = true
            return true
        }
        return super.onOptionsItemSelected(item)
    }

    override fun onRestart() {
        super.onRestart()
        if (xPlayer.isPaused()) {
            xPlayer.resume()
        }
    }

    override fun onStop() {
        super.onStop()
        if (xPlayer.isPlaying()) {
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