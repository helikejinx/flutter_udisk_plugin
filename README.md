# flutter_udisk_plugin

A Flutter project about udisk.

## Getting Started

```dart
  final _flutterUdiskPlugin = FlutterUdiskPlugin();

_flutterUdiskPlugin
    .setMethodCallHandler((call) => handleChannelMethod(call));

Future<void> handleChannelMethod(MethodCall call) async {
  switch (call.method) {
    case "device_removed":
      todo("usb设备被移除");
      break;
    case "device_added":
      todo("usb设备插入");
      break;
  }
}
```