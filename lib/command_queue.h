#ifndef COMMAND_QUEUE_H
#define COMMAND_QUEUE_H
#include <vector>
#include <webgpu/webgpu.h>

WGPUQueue createCommandQueue(WGPUDevice device);
void submitCommandBufferVector(WGPUQueue queue,
                               std::vector<WGPUCommandBuffer> commands);

#endif // COMMAND_QUEUE_H
