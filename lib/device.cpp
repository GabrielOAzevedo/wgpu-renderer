#include <cassert>
#include <iostream>
#include <webgpu/webgpu.h>

WGPUDevice requestDevice(WGPUAdapter adapter,
                         WGPUDeviceDescriptor const *descriptor) {
  struct UserData {
    WGPUDevice device = nullptr;
    bool requestEnded = false;
  };

  UserData userData;

  auto onDeviceRequestEnded = [](WGPURequestDeviceStatus status,
                                 WGPUDevice device, char const *message,
                                 void *pUserData) {
    UserData &userData = *reinterpret_cast<UserData *>(pUserData);
    if (status == WGPURequestDeviceStatus_Success) {
      userData.device = device;
    } else {
      std::cerr << "Failed to create device: " << message << std::endl;
    }
    userData.requestEnded = true;
  };

  auto onDeviceError = [](WGPUErrorType type, char const *message,
                          void * /*pUserData*/) {
    std::cout << "Error type:" << type << std::endl;
    if (message) {
      std::cout << "  " << message << std::endl;
    }
  };

  wgpuAdapterRequestDevice(adapter, descriptor, onDeviceRequestEnded,
                           &userData);
  wgpuDeviceSetUncapturedErrorCallback(userData.device, onDeviceError, nullptr);

  assert(userData.requestEnded);

  return userData.device;
}

WGPUDeviceDescriptor createDeviceDescriptor() {
  WGPUDeviceDescriptor deviceDescriptor = {};
  deviceDescriptor.nextInChain = nullptr;
  deviceDescriptor.label = "Device";
  deviceDescriptor.requiredFeaturesCount = 0;
  deviceDescriptor.requiredLimits = nullptr;
  deviceDescriptor.defaultQueue.nextInChain = nullptr;
  deviceDescriptor.defaultQueue.label = "Default Queue";

  return deviceDescriptor;
}

WGPUDevice buildDefaultDevice(WGPUAdapter adapter) {
  std::cout << "Requesting device" << std::endl;
  WGPUDeviceDescriptor deviceDescriptor = createDeviceDescriptor();
  WGPUDevice device = requestDevice(adapter, &deviceDescriptor);
  std::cout << "Device created: " << device << std::endl;
  return device;
}
