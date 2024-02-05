#ifndef COMMAND_ENCODER_H
#define COMMAND_ENCODER_H
#include <webgpu/webgpu.h>

WGPUCommandEncoder createCommandEncoder(WGPUDevice device);
WGPUCommandBufferDescriptor makeCommandBufferDescriptor();
WGPUCommandBuffer finishCommandEncoder(WGPUCommandEncoder commandEncoder,
                                       WGPUCommandBufferDescriptor descriptor);

#endif // COMMAND_ENCODER_H
