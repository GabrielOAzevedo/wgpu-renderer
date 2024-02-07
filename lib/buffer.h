#ifndef BUFFER_H
#define BUFFER_H
#include <webgpu/webgpu.h>

WGPUBuffer createBuffer(
    WGPUDevice device, 
    const char* label,
    size_t size, 
    WGPUBufferUsageFlags usage
);
void writeToBuffer(
    WGPUQueue queue, 
    WGPUBuffer buffer, 
    void const *data, 
    size_t size
);

#endif // BUFFER_H
