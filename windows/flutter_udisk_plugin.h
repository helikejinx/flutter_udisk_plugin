#ifndef FLUTTER_PLUGIN_FLUTTER_UDISK_PLUGIN_H_
#define FLUTTER_PLUGIN_FLUTTER_UDISK_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>

namespace flutter_udisk_plugin {

class FlutterUdiskPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  FlutterUdiskPlugin(flutter::PluginRegistrarWindows* registrar);

  virtual ~FlutterUdiskPlugin();



  // The registrar for this plugin, for accessing the window.
  flutter::PluginRegistrarWindows* m_registrar;

  std::unique_ptr<flutter::MethodChannel<flutter::EncodableValue>> m_channel;
  // The ID of the WindowProc delegate registration.
  int m_windowProcId = -1;
  bool m_registered = false;

  // Disallow copy and assign.
  FlutterUdiskPlugin(const FlutterUdiskPlugin&) = delete;
  FlutterUdiskPlugin& operator=(const FlutterUdiskPlugin&) = delete;

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
  // Called for top-level WindowProc delegation.
  std::optional<LRESULT> HandleWindowProc(HWND hwnd, UINT message,
      WPARAM wparam, LPARAM lparam);
};

}  // namespace flutter_udisk_plugin

#endif  // FLUTTER_PLUGIN_FLUTTER_UDISK_PLUGIN_H_
