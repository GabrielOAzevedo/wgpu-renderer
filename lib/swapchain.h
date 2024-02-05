#ifndef SWAPCHAIN_H
#define SWAPCHAIN_H
#include <webgpu/webgpu.h>

WGPUSwapChainDescriptor createDefaultSwapChainDescriptor(uint32_t width,
                                                         uint32_t height);
WGPUSwapChain createSwapChain(WGPUDevice device, WGPUSurface surface,
                              WGPUSwapChainDescriptor *swapChainDescriptor);
WGPUSwapChain buildDefaultSwapChain(WGPUDevice device, WGPUSurface surface,
                                    WGPUTextureFormat textureFormat,
                                    uint32_t width, uint32_t height);
#endif // SWAPCHAIN_H
