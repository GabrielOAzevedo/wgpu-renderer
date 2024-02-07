#ifndef SHADERS_H
#define SHADERS_H
#include <string>

const std::string loadShaderFromFile(const char* path);
WGPUBlendState buildBlendState(
  WGPUBlendFactor colorSrcFactor,
  WGPUBlendFactor colorDstFactor,
  WGPUBlendOperation colorBlendOp,
  WGPUBlendFactor alphaSrcFactor,
  WGPUBlendFactor alphaDstFactor,
  WGPUBlendOperation alphaBlendOp
);

#endif
