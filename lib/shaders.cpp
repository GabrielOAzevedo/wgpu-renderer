#include <fstream>
#include <string>
#include <webgpu/webgpu.h>

const std::string loadShaderFromFile(const char* path) {
  std::ifstream in(path);
  std::string contents((std::istreambuf_iterator<char>(in)),
                       std::istreambuf_iterator<char>());

  return contents;
}

WGPUBlendState buildBlendState(
  WGPUBlendFactor colorSrcFactor,
  WGPUBlendFactor colorDstFactor,
  WGPUBlendOperation colorBlendOp,
  WGPUBlendFactor alphaSrcFactor,
  WGPUBlendFactor alphaDstFactor,
  WGPUBlendOperation alphaBlendOp
) {
  WGPUBlendState blendState = {};
  blendState.color.srcFactor = colorSrcFactor;
  blendState.color.dstFactor = colorDstFactor;
  blendState.color.operation = colorBlendOp;
  blendState.alpha.srcFactor = alphaSrcFactor;
  blendState.alpha.dstFactor = alphaDstFactor;
  blendState.alpha.operation = alphaBlendOp;

  return blendState;
}