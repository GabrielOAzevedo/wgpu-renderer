#include <iostream>
#include <webgpu/webgpu.h>

WGPUCommandEncoder createCommandEncoder(WGPUDevice device) {
  WGPUCommandEncoderDescriptor desc = {};
  desc.nextInChain = nullptr;
  desc.label = "Command Encoder";
  WGPUCommandEncoder commandEncoder =
      wgpuDeviceCreateCommandEncoder(device, &desc);
  std::cout << "Command encoder created: " << commandEncoder << std::endl;
  return commandEncoder;
}
