#include <iostream>
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

WGPUColorTargetState buildColorTargetState(
  WGPUTextureFormat textureFormat,
  WGPUBlendState *blendState,
  WGPUColorWriteMask writeMask
) {
  WGPUColorTargetState colorTargetState = {};
  colorTargetState.format = textureFormat;
  colorTargetState.blend = blendState;
  colorTargetState.writeMask = writeMask;

  return colorTargetState;
}

WGPUFragmentState buildFragmentState(
  WGPUShaderModule shaderModule,
  const char* entryPoint,
  WGPUColorTargetState *colorTargetState
) {
  WGPUFragmentState fragmentState = {};
  fragmentState.module = shaderModule;
  fragmentState.entryPoint = entryPoint;
  fragmentState.constantCount = 0;
  fragmentState.constants = nullptr;
  fragmentState.targetCount = 1;
  fragmentState.targets = colorTargetState;

  return fragmentState;
}

auto onShaderCompilationInfo = [](WGPUCompilationInfoRequestStatus status, WGPUCompilationInfo const *info, void *userdata) {
  std::cout << "Compilation info request status: " << status << std::endl;
  std::cout << userdata << std::endl;
  for (uint32_t i = 0; i < info->messageCount; ++i) {
    WGPUCompilationMessage message = info->messages[i];
    std::cout << "Compilation info: " << message.message << std::endl;
    WGPUCompilationMessageType status = message.type;

    if (status == WGPUCompilationMessageType_Error) {
      std::cout << "Compilation failed: " << message.message << std::endl;
    } else {
      std::cout << "Compilation Info: " << message.message << std::endl;
    }
  }
};

WGPUShaderModule loadShaderModuleFromFile(
  WGPUDevice device,
  const char* path
) {
  const std::string shaderSource = loadShaderFromFile(path);

  WGPUShaderModuleWGSLDescriptor wgslDesc = {};
  wgslDesc.chain.next = nullptr;
  wgslDesc.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
  wgslDesc.code = shaderSource.c_str();

  WGPUShaderModuleDescriptor shaderModuleDesc = {};
  shaderModuleDesc.nextInChain = &wgslDesc.chain;

  WGPUShaderModule shaderModule =
      wgpuDeviceCreateShaderModule(device, &shaderModuleDesc);

  wgpuShaderModuleGetCompilationInfo(shaderModule, onShaderCompilationInfo, nullptr);

  return shaderModule;
}