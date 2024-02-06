#include "lib/adapter.h"
#include "lib/command_encoder.h"
#include "lib/command_queue.h"
#include "lib/device.h"
#include "lib/renderpass.h"
#include "lib/swapchain.h"
#include "lib/pipeline.h"
#include "lib/shaders.h"
#include <GLFW/glfw3.h>
#include <glfw3webgpu.h>
#include <iostream>
#include <vector>
#include <webgpu/webgpu.h>
#include <fstream>
#include <string>

int main(int, char **) {
  glfwInit();

  if (!glfwInit()) {
    std::cerr << "Failed to initialize GLFW" << std::endl;
    return -1;
  }

  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  GLFWwindow *window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

  if (!window) {
    std::cerr << "Failed to create window" << std::endl;
    glfwTerminate();
    return -1;
  }

  WGPUInstanceDescriptor instanceDescriptor = {};
  instanceDescriptor.nextInChain = nullptr;

  WGPUInstance instance = wgpuCreateInstance(&instanceDescriptor);

  if (!instance) {
    std::cerr << "Failed to create WebGPU instance" << std::endl;
    glfwDestroyWindow(window);
    glfwTerminate();
    return -1;
  }

  std::cout << "WebGPU instance created" << instance << std::endl;

  WGPUSurface surface = glfwGetWGPUSurface(instance, window);
  WGPUAdapter adapter = buildDefaultAdapter(instance);
  std::vector<WGPUFeatureName> features = getAdapterFeatures(adapter);

  WGPUDevice device = buildDefaultDevice(adapter);

  WGPUQueue queue = createCommandQueue(device);
  std::cout << "Queue created: " << queue << std::endl;

  WGPUTextureFormat textureFormat = WGPUTextureFormat_BGRA8Unorm;
  WGPUSwapChain swapChain =
      buildDefaultSwapChain(device, surface, textureFormat, 640, 480);
  std::cout << "Swap chain created: " << swapChain << std::endl;

  WGPUBlendState blendState = {};
  blendState.color.srcFactor = WGPUBlendFactor_SrcAlpha;
  blendState.color.dstFactor = WGPUBlendFactor_OneMinusSrcAlpha;
  blendState.color.operation = WGPUBlendOperation_Add;
  blendState.alpha.srcFactor = WGPUBlendFactor_Zero;
  blendState.alpha.dstFactor = WGPUBlendFactor_One;
  blendState.alpha.operation = WGPUBlendOperation_Add;

  WGPUColorTargetState colorTargetState = {};
  colorTargetState.format = textureFormat;
  colorTargetState.blend = &blendState;
  colorTargetState.writeMask = WGPUColorWriteMask_All;

  const std::string shaderSource = loadShaderFromFile("lib/shader/default.wsgl");
  std::cout << "Shader source: " << shaderSource << std::endl;

  WGPUShaderModuleWGSLDescriptor wgslDesc = {};
  wgslDesc.chain.next = nullptr;
  wgslDesc.chain.sType = WGPUSType_ShaderModuleWGSLDescriptor;
  wgslDesc.code = shaderSource.c_str();

  WGPUShaderModuleDescriptor shaderModuleDesc = {};
  shaderModuleDesc.nextInChain = &wgslDesc.chain;

  WGPUShaderModule shaderModule =
      wgpuDeviceCreateShaderModule(device, &shaderModuleDesc);

  WGPUFragmentState fragmentState = {};
  fragmentState.module = shaderModule;
  fragmentState.entryPoint = "fs_main";
  fragmentState.constantCount = 0;
  fragmentState.constants = nullptr;
  fragmentState.targetCount = 1;
  fragmentState.targets = &colorTargetState;

  WGPURenderPipelineDescriptor pipelineDesc = buildRenderPipelineDescriptor(
    shaderModule, fragmentState, "vs_main"
  );

  WGPURenderPipeline pipeline =
      wgpuDeviceCreateRenderPipeline(device, &pipelineDesc);

  while (!glfwWindowShouldClose(window)) {
    glfwPollEvents();
    WGPUTextureView nextTexture = wgpuSwapChainGetCurrentTextureView(swapChain);
    if (!nextTexture) {
      std::cerr << "Failed to get next texture" << std::endl;
      break;
    }

    WGPUCommandEncoder commandEncoder = createCommandEncoder(device);

    WGPURenderPassColorAttachment renderPassColorAttachment =
        buildColorAttachment(nextTexture);
    WGPURenderPassDescriptor renderPassDescriptor =
        buildRenderPassDescriptor(renderPassColorAttachment);
    WGPURenderPassEncoder renderPass =
        beginRenderPass(commandEncoder, &renderPassDescriptor);
    wgpuRenderPassEncoderSetPipeline(renderPass, pipeline);
    wgpuRenderPassEncoderDraw(renderPass, 3, 1, 0, 0);
    submitRenderPass(renderPass);
    wgpuTextureViewRelease(nextTexture);

    WGPUCommandBufferDescriptor commandBufferDescriptor =
        makeCommandBufferDescriptor();
    WGPUCommandBuffer commandBuffer =
        finishCommandEncoder(commandEncoder, commandBufferDescriptor);
    wgpuCommandEncoderRelease(commandEncoder);
    wgpuQueueSubmit(queue, 1, &commandBuffer);
    wgpuCommandBufferRelease(commandBuffer);

    /* Release and present the texture */
    wgpuSwapChainPresent(swapChain);
  }

  /* Releases wgpu objects */
  wgpuSwapChainRelease(swapChain);
  wgpuDeviceRelease(device);
  wgpuSurfaceRelease(surface);
  wgpuAdapterRelease(adapter);
  wgpuInstanceRelease(instance);

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
