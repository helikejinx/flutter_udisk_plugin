#include "include/flutter_udisk_plugin/flutter_udisk_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "flutter_udisk_plugin.h"

void FlutterUdiskPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  flutter_udisk_plugin::FlutterUdiskPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
