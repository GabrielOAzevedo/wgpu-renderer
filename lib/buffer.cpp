#include <webgpu/webgpu.h>

WGPUBuffer createBuffer(
    WGPUDevice device, 
    const char* label,
    size_t size, 
    WGPUBufferUsageFlags usage
) {
    WGPUBufferDescriptor bufferDesc = {};
    bufferDesc.nextInChain = nullptr;
    bufferDesc.label = label;
    bufferDesc.usage = usage;
    bufferDesc.size = size;
    bufferDesc.mappedAtCreation = false;

    WGPUBuffer buffer = wgpuDeviceCreateBuffer(device, &bufferDesc);
    return buffer;
}

void writeToBuffer(
    WGPUQueue queue, 
    WGPUBuffer buffer, 
    void const *data, 
    size_t size
) {
    wgpuQueueWriteBuffer(
        queue,
        buffer,
        0,
        data,
        size
    );
}