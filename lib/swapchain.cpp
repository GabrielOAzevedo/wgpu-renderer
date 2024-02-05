#include <webgpu/webgpu.h>

WGPUSwapChainDescriptor
createDefaultSwapChainDescriptor(uint32_t width, uint32_t height,
                                 WGPUTextureFormat textureFormat) {
  WGPUSwapChainDescriptor swapChainDescriptor = {};
  swapChainDescriptor.nextInChain = nullptr;
  swapChainDescriptor.label = "Swap Chain";
  swapChainDescriptor.width = width;
  swapChainDescriptor.height = height;
  swapChainDescriptor.format = textureFormat;
  swapChainDescriptor.usage = WGPUTextureUsage_RenderAttachment;
  swapChainDescriptor.presentMode = WGPUPresentMode_Fifo;

  return swapChainDescriptor;
}

WGPUSwapChain createSwapChain(WGPUDevice device, WGPUSurface surface,
                              WGPUSwapChainDescriptor *swapChainDescriptor) {
  WGPUSwapChain swapChain =
      wgpuDeviceCreateSwapChain(device, surface, swapChainDescriptor);
  return swapChain;
}

WGPUSwapChain buildDefaultSwapChain(WGPUDevice device, WGPUSurface surface,
                                    WGPUTextureFormat textureFormat,
                                    uint32_t width, uint32_t height) {
  WGPUSwapChainDescriptor swapChainDescriptor =
      createDefaultSwapChainDescriptor(width, height, textureFormat);
  WGPUSwapChain swapChain =
      createSwapChain(device, surface, &swapChainDescriptor);
  return swapChain;
}
