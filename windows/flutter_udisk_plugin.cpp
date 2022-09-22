#include "flutter_udisk_plugin.h"

// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <memory>
#include <sstream>
#include <map>
#include <dbt.h>

namespace flutter_udisk_plugin {

// static
void FlutterUdiskPlugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {


  auto plugin = std::make_unique<FlutterUdiskPlugin>(registrar);
  plugin.get()->m_channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "flutter_udisk_plugin",
          &flutter::StandardMethodCodec::GetInstance());
  plugin.get()->m_channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

FlutterUdiskPlugin::FlutterUdiskPlugin(flutter::PluginRegistrarWindows *registrar) : m_registrar(registrar) {
    m_windowProcId = m_registrar->RegisterTopLevelWindowProcDelegate(
        [this](HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
            return HandleWindowProc(hwnd, message, wparam, lparam);
        });
}

FlutterUdiskPlugin::~FlutterUdiskPlugin() {
    m_registrar->UnregisterTopLevelWindowProcDelegate(m_windowProcId);
}

void FlutterUdiskPlugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (method_call.method_name().compare("getPlatformVersion") == 0) {
    std::ostringstream version_stream;
    //m_channel->InvokeMethod("device_added", nullptr, nullptr);
    version_stream << "Windows ";
    if (IsWindows10OrGreater()) {
      version_stream << "11";
    } else if (IsWindows8OrGreater()) {
      version_stream << "8";
    } else if (IsWindows7OrGreater()) {
      version_stream << "7";
    }
    result->Success(flutter::EncodableValue(version_stream.str()));
  } else {
    result->NotImplemented();
  }
}

std::optional<LRESULT> FlutterUdiskPlugin::HandleWindowProc(HWND hwnd,
    UINT message,
    WPARAM wparam,
    LPARAM lparam) {
    std::optional<LRESULT> result;
    switch (message) {
    case WM_ACTIVATE:
    {
        if (!m_registered)
        {
            m_registered = true;

            DEV_BROADCAST_DEVICEINTERFACE notificationFilter;
            ZeroMemory(&notificationFilter, sizeof(notificationFilter));
            notificationFilter.dbcc_size = sizeof(DEV_BROADCAST_DEVICEINTERFACE);
            notificationFilter.dbcc_devicetype = DBT_DEVTYP_DEVICEINTERFACE;

            RegisterDeviceNotification(
                hwnd,
                &notificationFilter,
                DEVICE_NOTIFY_ALL_INTERFACE_CLASSES
            );
        }
    }
    break;
    case WM_DEVICECHANGE:
    {
        if ((DWORD)wparam == DBT_DEVICEARRIVAL) {
            DEV_BROADCAST_VOLUME* p = (DEV_BROADCAST_VOLUME*)lparam;
            if (p->dbcv_devicetype == DBT_DEVTYP_VOLUME) {
                int l = (int)(log(double(p->dbcv_unitmask)) / log(double(2)));
                std::string tmp_string(1, (char)('A' + l));
                std::unique_ptr<flutter::EncodableValue> pv(new flutter::EncodableValue(tmp_string + ":\\"));
                m_channel->InvokeMethod("device_added", std::move(pv), nullptr);
            }
        }
        else if ((DWORD)wparam == DBT_DEVICEREMOVECOMPLETE) {
            DEV_BROADCAST_VOLUME* p = (DEV_BROADCAST_VOLUME*)lparam;
            if (p->dbcv_devicetype == DBT_DEVTYP_VOLUME) {
                //int l = (int)(log(double(p->dbcv_unitmask)) / log(double(2)));
                m_channel->InvokeMethod("device_removed", nullptr, nullptr);
            }
        }
    }
    break;
    }
    return result;
}

}  // namespace flutter_udisk_plugin
