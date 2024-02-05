#include "lib/adapter.h"
#include "lib/command_encoder.h"
#include "lib/command_queue.h"
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
  std::cout << "Device created: " << device << std::endl;

  WGPUQueue queue = createCommandQueue(device);
  std::cout << "Queue created: " << queue << std::endl;

  WGPUSwapChainDescriptor swapChainDescriptor = {};
  swapChainDescriptor.nextInChain = nullptr;
  swapChainDescriptor.label = "Swap Chain";
  swapChainDescriptor.width = 640;
  swapChainDescriptor.height = 480;
  WGPUTextureFormat format = WGPUTextureFormat_BGRA8Unorm;
  swapChainDescriptor.format = format;
  swapChainDescriptor.usage = WGPUTextureUsage_RenderAttachment;
  swapChainDescriptor.presentMode = WGPUPresentMode_Fifo;
  WGPUSwapChain swapChain =
      wgpuDeviceCreateSwapChain(device, surface, &swapChainDescriptor);
  std::cout << "Swap chain created: " << swapChain << std::endl;

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    WGPUTextureView nextTexture = wgpuSwapChainGetCurrentTextureView(swapChain);
    std::cout << "Next texture: " << nextTexture << std::endl;
    if (!nextTexture) {
      std::cerr << "Failed to get next texture" << std::endl;
      break;
    }

    WGPUCommandEncoder commandEncoder = createCommandEncoder(device);
    std::cout << "Command encoder created: " << commandEncoder << std::endl;

    WGPURenderPassColorAttachment renderPassColorAttachment = {};
    renderPassColorAttachment.view = nextTexture;
    renderPassColorAttachment.resolveTarget = nullptr;
    renderPassColorAttachment.loadOp = WGPULoadOp_Clear;
    renderPassColorAttachment.storeOp = WGPUStoreOp_Store;
    renderPassColorAttachment.clearValue = {0.9f, 0.1f, 0.2f, 1.0f};

    WGPURenderPassDescriptor renderPassDescriptor = {};
    renderPassDescriptor.colorAttachmentCount = 1;
    renderPassDescriptor.colorAttachments = &renderPassColorAttachment;
    renderPassDescriptor.depthStencilAttachment = nullptr;
    renderPassDescriptor.timestampWriteCount = 0;
    renderPassDescriptor.timestampWrites = nullptr;
    renderPassDescriptor.nextInChain = nullptr;

    WGPURenderPassEncoder renderPass = wgpuCommandEncoderBeginRenderPass(
        commandEncoder, &renderPassDescriptor);
    wgpuRenderPassEncoderEnd(renderPass);
    wgpuRenderPassEncoderRelease(renderPass);

    wgpuTextureViewRelease(nextTexture);

    WGPUCommandBufferDescriptor commandBufferDescriptor = {};
    commandBufferDescriptor.nextInChain = nullptr;
    commandBufferDescriptor.label = "Command Buffer";
    WGPUCommandBuffer commandBuffer =
        wgpuCommandEncoderFinish(commandEncoder, &commandBufferDescriptor);

    wgpuCommandEncoderRelease(commandEncoder);
    wgpuQueueSubmit(queue, 1, &commandBuffer);
    wgpuCommandBufferRelease(commandBuffer);

    /* Release and present the texture */
    wgpuSwapChainPresent(swapChain);
  }

  /* Releases wgpu objects */
  wgpuSwapChainRelease(swapChain);
  wgpuDeviceRelease(device);
  wgpuSurfaceRelease(surface);
  wgpuAdapterRelease(adapter);
  wgpuInstanceRelease(instance);

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
