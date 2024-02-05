#include <cassert>
#include <iostream>
#include <webgpu/webgpu.h>

WGPUAdapter requestAdapter(WGPUInstance instance,
                           WGPURequestAdapterOptions const *options) {
  struct UserData {
    WGPUAdapter adapter;
    bool requestEnded = false;
  };

  UserData userData;

  auto onAdapterRequestEnded = [](WGPURequestAdapterStatus status,
                                  WGPUAdapter adapter, char const *message,
                                  void *pUserData) {
    UserData &userData = *reinterpret_cast<UserData *>(pUserData);
    if (status == WGPURequestAdapterStatus_Success) {
      userData.adapter = adapter;
    } else {
      std::cerr << "Failed to request adapter: " << message << std::endl;
    }
    userData.requestEnded = true;
  };

  wgpuInstanceRequestAdapter(instance, options, onAdapterRequestEnded,
                             (void *)&userData);

  assert(userData.requestEnded);

  return userData.adapter;
}
