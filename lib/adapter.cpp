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

WGPURequestAdapterOptions requestAdapterOptions(WGPUSurface surface) {
  WGPURequestAdapterOptions options = {};
  options.nextInChain = nullptr;
  options.compatibleSurface = surface;
  return options;
}

WGPUAdapter buildDefaultAdapter(WGPUInstance instance) {
  WGPUSurfaceDescriptor surfaceDescriptor = {};
  surfaceDescriptor.nextInChain = nullptr;
  surfaceDescriptor.label = "WGPU Surface";
  WGPUSurface surface = wgpuInstanceCreateSurface(instance, &surfaceDescriptor);

  WGPURequestAdapterOptions adapterOpts = requestAdapterOptions(surface);
  WGPUAdapter adapter = requestAdapter(instance, &adapterOpts);
  std::cout << "Adapter created" << adapter << std::endl;
  return adapter;
}

std::vector<WGPUFeatureName> getAdapterFeatures(WGPUAdapter adapter) {
  std::vector<WGPUFeatureName> features;
  size_t featureCount = wgpuAdapterEnumerateFeatures(adapter, nullptr);
  features.resize(featureCount);
  wgpuAdapterEnumerateFeatures(adapter, features.data());
  std::cout << "Adapter features: " << std::endl;
  for (auto feature : features) {
    std::cout << "  " << feature << std::endl;
  }
  return features;
}
