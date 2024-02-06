#ifndef PIPELINE_H
#define PIPELINE_H
#include <webgpu/webgpu.h>

WGPURenderPipelineDescriptor buildRenderPipelineDescriptor(
    WGPUShaderModule shaderModule,
    WGPUFragmentState fragmentState,
    char const *vertexEntryPoint
);

#endif // PIPELINE_H