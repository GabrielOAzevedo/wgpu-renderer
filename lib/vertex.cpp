#include <webgpu/webgpu.h>
#include <vector>

WGPUVertexAttribute createVertexAttribute(
    uint32_t shaderLocation,
    WGPUVertexFormat format,
    uint64_t offset
) {
    WGPUVertexAttribute vertexAttribute;
    vertexAttribute.shaderLocation = shaderLocation;
    vertexAttribute.format = format;
    vertexAttribute.offset = offset;
    return vertexAttribute;
}

WGPUVertexBufferLayout createVertexBufferLayout(
    uint32_t attributeCount,
    std::vector<WGPUVertexAttribute> const * attributes,
    uint64_t arrayStride,
    WGPUVertexStepMode stepMode
) {
  WGPUVertexBufferLayout vertexBufferLayout = {};
  vertexBufferLayout.attributeCount = attributeCount;
  vertexBufferLayout.attributes = (*attributes).data();
  vertexBufferLayout.arrayStride = arrayStride;
  vertexBufferLayout.stepMode = stepMode;

  return vertexBufferLayout;
}