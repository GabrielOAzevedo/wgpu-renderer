#include <iostream>
#include <webgpu/webgpu.h>

WGPUCommandEncoder createCommandEncoder(WGPUDevice device) {
  WGPUCommandEncoderDescriptor desc = {};
  desc.nextInChain = nullptr;
  desc.label = "Command Encoder";
  WGPUCommandEncoder commandEncoder =
      wgpuDeviceCreateCommandEncoder(device, &desc);
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
  return commandBuffer;
}
