#include <iostream>
#include <vector>
#include <webgpu/webgpu.h>

WGPUQueue createCommandQueue(WGPUDevice device) {
  auto onQueueWorkDone = [](WGPUQueueWorkDoneStatus status,
                            void * /*userdata*/) {
    std::cout << "Queue work done with status: " << status << std::endl;
  };

  WGPUQueue queue = wgpuDeviceGetQueue(device);

  wgpuQueueOnSubmittedWorkDone(queue, 0, onQueueWorkDone, nullptr);

  return queue;
}

void submitCommandBufferVector(WGPUQueue queue,
                               std::vector<WGPUCommandBuffer> commands) {
  wgpuQueueSubmit(queue, commands.size(), commands.data());
}
