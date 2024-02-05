#include <webgpu/webgpu.h>

WGPURenderPassColorAttachment
buildColorAttachment(WGPUTextureView textureView) {
  WGPURenderPassColorAttachment renderPassColorAttachment = {};
  renderPassColorAttachment.view = textureView;
  renderPassColorAttachment.resolveTarget = nullptr;
  renderPassColorAttachment.loadOp = WGPULoadOp_Clear;
  renderPassColorAttachment.storeOp = WGPUStoreOp_Store;
  renderPassColorAttachment.clearValue = {0.9f, 0.1f, 0.2f, 1.0f};
  return renderPassColorAttachment;
}

WGPURenderPassDescriptor
buildRenderPassDescriptor(WGPURenderPassColorAttachment colorAttachment) {
  WGPURenderPassDescriptor renderPassDescriptor = {};
  renderPassDescriptor.colorAttachmentCount = 1;
  renderPassDescriptor.colorAttachments = &colorAttachment;
  renderPassDescriptor.depthStencilAttachment = nullptr;
  renderPassDescriptor.timestampWriteCount = 0;
  renderPassDescriptor.timestampWrites = nullptr;
  renderPassDescriptor.nextInChain = nullptr;
  return renderPassDescriptor;
}

WGPURenderPassEncoder
beginRenderPass(WGPUCommandEncoder commandEncoder,
                WGPURenderPassDescriptor const *renderPassDescriptor) {
  return wgpuCommandEncoderBeginRenderPass(commandEncoder,
                                           renderPassDescriptor);
}

void submitRenderPass(WGPURenderPassEncoder renderPass) {
  wgpuRenderPassEncoderEnd(renderPass);
  wgpuRenderPassEncoderRelease(renderPass);
}
