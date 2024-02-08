#ifndef VERTEX_H
#define VERTEX_H
#include <webgpu/webgpu.h>

WGPUVertexAttribute createVertexAttribute(
    uint32_t shaderLocation,
    WGPUVertexFormat format,
    uint64_t offset
);

WGPUVertexBufferLayout createVertexBufferLayout(
    uint32_t attributeCount,
    std::vector<WGPUVertexAttribute> const * attributes,
    uint64_t arrayStride,
    WGPUVertexStepMode stepMode
);

#endif // VERTEX_H
