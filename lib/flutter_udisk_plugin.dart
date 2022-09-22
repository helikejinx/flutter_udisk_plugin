
import 'package:flutter/services.dart';

class FlutterUdiskPlugin {

  final MethodChannel _channel = const MethodChannel('flutter_udisk_plugin');

  Future<String?> getPlatformVersion() async {
    return await _channel.invokeMethod('getPlatformVersion');
  }

  void setMethodCallHandler(Future<dynamic> Function(MethodCall call)? handler){
    _channel.setMethodCallHandler(handler);
  }
}
