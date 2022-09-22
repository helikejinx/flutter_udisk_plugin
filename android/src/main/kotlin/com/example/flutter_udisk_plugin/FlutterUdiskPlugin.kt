package com.example.flutter_udisk_plugin

import android.content.BroadcastReceiver
import android.content.Context
import android.content.Intent
import android.content.IntentFilter
import android.text.TextUtils
import android.util.Log
import androidx.annotation.NonNull

import io.flutter.embedding.engine.plugins.FlutterPlugin
import io.flutter.plugin.common.MethodCall
import io.flutter.plugin.common.MethodChannel
import io.flutter.plugin.common.MethodChannel.MethodCallHandler
import io.flutter.plugin.common.MethodChannel.Result
import java.io.File

/** FlutterUdiskPlugin */
class FlutterUdiskPlugin : FlutterPlugin, MethodCallHandler {
    /// The MethodChannel that will the communication between Flutter and native Android
    ///
    /// This local reference serves to register the plugin with the Flutter Engine and unregister it
    /// when the Flutter Engine is detached from the Activity
    private lateinit var channel: MethodChannel
    private var mBroadcastReceiver: USBBroadCastReceiver? = null

    override fun onAttachedToEngine(@NonNull flutterPluginBinding: FlutterPlugin.FlutterPluginBinding) {
        channel = MethodChannel(flutterPluginBinding.binaryMessenger, "flutter_udisk_plugin")
        channel.setMethodCallHandler(this)
        channel.invokeMethod("device_removed", "")
        initIntentFilter(flutterPluginBinding.applicationContext)
    }

    override fun onMethodCall(@NonNull call: MethodCall, @NonNull result: Result) {
        if (call.method == "getPlatformVersion") {
            result.success("Android ${android.os.Build.VERSION.RELEASE}")
            channel.invokeMethod("device_removed", "")
            Log.e("MyTest", "onMethodCall: getPlatformVersion")
        } else {
            result.notImplemented()
        }
    }

    override fun onDetachedFromEngine(@NonNull binding: FlutterPlugin.FlutterPluginBinding) {
        channel.setMethodCallHandler(null)
        binding.applicationContext.unregisterReceiver(mBroadcastReceiver)
    }

    inner class USBBroadCastReceiver : BroadcastReceiver() {
        override fun onReceive(context: Context?, intent: Intent) {
            val action = intent.action
            if (action.equals(Intent.ACTION_MEDIA_EJECT)) {
                channel.invokeMethod("device_removed", "")
                Log.e("MyTest", "onReceive: device_removed")
            } else if (action.equals(Intent.ACTION_MEDIA_MOUNTED)) {
                val mountPath: String? = intent.data?.path
                val truePath = getCorrectPath(mountPath ?: "")
                Log.e(
                    "MyTest",
                    "ACTION_MEDIA_MOUNTED: mountPath${mountPath} truePath${truePath}"
                )
//        channel.invokeMethod("device_added", "$mountPath")
                channel.invokeMethod("find_device", "$truePath/")
            }
        }
    }

    /**
     *   部分Android设备系统广播返回的路径有缺失，此方法用于补全路径
     *   目前仅rk3288 需要此操作
     */
    private fun getCorrectPath(path: String): String {
        var temPath = path
        if (!TextUtils.isEmpty(temPath)) {
            val lastSeparator: Int = temPath.lastIndexOf(File.separator)
            val endStr = temPath.substring(lastSeparator + 1, temPath.length)
            if (endStr.isNotBlank() && (endStr.contains("USB_DISK") || endStr.contains("usb_disk"))) { //不区分大小写
                val file = File(temPath)
                if (file.exists() && file.listFiles() != null && (file.listFiles()?.size
                        ?: 0) == 1 && file.listFiles()
                        ?.get(0)?.isDirectory == true
                ) {
                    temPath = file.listFiles().first().absolutePath
                }
            }
        }
        return temPath
    }

    private fun initIntentFilter(context: Context) {
        Log.e("MyTest", "initIntentFilter: initIntentFilter")
        val iFilter = IntentFilter()
        iFilter.addAction(Intent.ACTION_MEDIA_EJECT)
        iFilter.addAction(Intent.ACTION_MEDIA_MOUNTED)
        iFilter.addAction(Intent.ACTION_MEDIA_REMOVED)
        iFilter.addAction(Intent.ACTION_MEDIA_UNMOUNTED)
        iFilter.addDataScheme("file")
        mBroadcastReceiver = USBBroadCastReceiver()
        context.registerReceiver(mBroadcastReceiver, iFilter)
    }
}
