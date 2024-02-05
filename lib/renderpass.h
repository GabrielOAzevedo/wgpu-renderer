#ifndef RENDERPASS_H
#define RENDERPASS_H
#include <webgpu/webgpu.h>

WGPURenderPassColorAttachment buildColorAttachment(WGPUTextureView textureView);
WGPURenderPassDescriptor
buildRenderPassDescriptor(WGPURenderPassColorAttachment colorAttachment);
WGPURenderPassEncoder
beginRenderPass(WGPUCommandEncoder commandEncoder,
                WGPURenderPassDescriptor const *renderPassDescriptor);
void submitRenderPass(WGPURenderPassEncoder renderPass);

#endif // RENDERPASS_H
