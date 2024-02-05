#include "lib/adapter.h"
#include "lib/device.h"
#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>
#include <iostream>
#include <vector>
#include <webgpu/webgpu.h>

int main(int, char **) {
  glfwInit();

  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow *window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

  if (!window) {
    std::cerr << "Failed to create window" << std::endl;
    glfwTerminate();
    return -1;
  }

  WGPUInstanceDescriptor instanceDescriptor = {};
  instanceDescriptor.nextInChain = nullptr;

  WGPUInstance instance = wgpuCreateInstance(&instanceDescriptor);

  if (!instance) {
    std::cerr << "Failed to create WebGPU instance" << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
    return -1;
  }

  std::cout << "WebGPU instance created" << instance << std::endl;

  WGPUSurface surface = glfwGetWGPUSurface(instance, window);

  WGPURequestAdapterOptions adapterOpts = {};
  adapterOpts.nextInChain = nullptr;
  adapterOpts.compatibleSurface = surface;

  WGPUAdapter adapter = requestAdapter(instance, &adapterOpts);

  std::cout << "Adapter created" << adapter << std::endl;

  std::vector<WGPUFeatureName> features;
  size_t featureCount = wgpuAdapterEnumerateFeatures(adapter, nullptr);
  features.resize(featureCount);
  wgpuAdapterEnumerateFeatures(adapter, features.data());
  std::cout << "Adapter features: " << std::endl;
  for (auto feature : features) {
    std::cout << "  " << feature << std::endl;
  }

  std::cout << "Requesting device" << std::endl;
  WGPUDeviceDescriptor deviceDescriptor = {};
  deviceDescriptor.nextInChain = nullptr;
  deviceDescriptor.label = "Device";
  deviceDescriptor.requiredFeaturesCount = 0;
  deviceDescriptor.requiredLimits = nullptr;
  deviceDescriptor.defaultQueue.nextInChain = nullptr;
  deviceDescriptor.defaultQueue.label = "Default Queue";
  WGPUDevice device = requestDevice(adapter, &deviceDescriptor);
  std::cout << "Device created" << device << std::endl;

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
  }

  // Releases wgpu objects
  wgpuDeviceRelease(device);
  wgpuSurfaceRelease(surface);
  wgpuAdapterRelease(adapter);
  wgpuInstanceRelease(instance);

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
