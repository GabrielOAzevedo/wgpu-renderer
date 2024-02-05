#include <webgpu/webgpu.h>

WGPUCommandEncoder createCommandEncoder(WGPUDevice device) {
  WGPUCommandEncoderDescriptor desc = {};
  desc.nextInChain = nullptr;
  desc.label = "Command Encoder";
  return wgpuDeviceCreateCommandEncoder(device, &desc);
}
