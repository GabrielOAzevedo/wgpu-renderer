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

WGPUCommandBufferDescriptor makeCommandBufferDescriptor() {
  WGPUCommandBufferDescriptor desc = {};
  desc.nextInChain = nullptr;
  desc.label = "Command Buffer";
  return desc;
}

WGPUCommandBuffer finishCommandEncoder(WGPUCommandEncoder commandEncoder,
                                       WGPUCommandBufferDescriptor descriptor) {
  WGPUCommandBuffer commandBuffer =
      wgpuCommandEncoderFinish(commandEncoder, &descriptor);
  std::cout << "Command buffer created: " << commandBuffer << std::endl;
  return commandBuffer;
}
