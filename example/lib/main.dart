import 'package:flutter/material.dart';
import 'dart:async';

import 'package:flutter/services.dart';
import 'package:flutter_udisk_plugin/flutter_udisk_plugin.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  String _platformVersion = 'Unknown';
  final _flutterUdiskPlugin = FlutterUdiskPlugin();

  @override
  void initState() {
    super.initState();
    initPlatformState();
    debugPrint('MyTest initState');
    _flutterUdiskPlugin
        .setMethodCallHandler((call) => handleChannelMethod(call));
  }

  Future<void> handleChannelMethod(MethodCall call) async {
    debugPrint('MyTest handleChannelMethod');
    switch (call.method) {
      case "device_removed":
        debugPrint('MyTest 设备被移除:${call.arguments.toString()}');
        setState(() {
          _platformVersion = 'MyTest 设备被移除:${call.arguments.toString()}';
        });
        break;
      case "device_added":
        setState(() {
          _platformVersion = 'MyTest 发现设备,路径为:${call.arguments.toString()}';
        });
        debugPrint('MyTest 发现设备,路径为:${call.arguments.toString()}');
        break;
    }
  }

  // Platform messages are asynchronous, so we initialize in an async method.
  Future<void> initPlatformState() async {
    String platformVersion;
    // Platform messages may fail, so we use a try/catch PlatformException.
    // We also handle the message potentially returning null.
    try {
      platformVersion = await _flutterUdiskPlugin.getPlatformVersion() ??
          'Unknown platform version';
    } on PlatformException {
      platformVersion = 'Failed to get platform version.';
    }

    // If the widget was removed from the tree while the asynchronous platform
    // message was in flight, we want to discard the reply rather than calling
    // setState to update our non-existent appearance.
    if (!mounted) return;
    debugPrint('MyTest platformVersion $platformVersion');

    // setState(() {
    //   _platformVersion = platformVersion;
    // });
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              Text('Running on: $_platformVersion\n'),
              InkWell(
                onTap: () {
                  debugPrint('MyTest getPlatformVersion');
                  initPlatformState();
                },
                child: const Text('getPlatformVersion'),
              )
            ],
          ),
        ),
      ),
    );
  }
}
