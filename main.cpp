#include "lib/adapter.h"
#include "lib/command_encoder.h"
#include "lib/command_queue.h"
#include "lib/device.h"
#include "lib/renderpass.h"
#include "lib/swapchain.h"
#include "lib/pipeline.h"
#include "lib/shaders.h"
#include "lib/buffer.h"
#include "lib/vertex.h"
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

  WGPUBlendState blendState = buildBlendState(
    WGPUBlendFactor_SrcAlpha,
    WGPUBlendFactor_OneMinusSrcAlpha,
    WGPUBlendOperation_Add,
    WGPUBlendFactor_Zero,
    WGPUBlendFactor_One,
    WGPUBlendOperation_Add
  );

  WGPUColorTargetState colorTargetState = buildColorTargetState(
    textureFormat, 
    &blendState, 
    WGPUColorWriteMask_All
  );

  WGPUShaderModule shaderModule = loadShaderModuleFromFile(device, "lib/shader/default.wsgl");

  WGPUFragmentState fragmentState = buildFragmentState(
    shaderModule, "fs_main", &colorTargetState
  );

  std::vector<float> vertexData = {
    -0.5, -0.5,
    +0.5, -0.5,
    +0.0, +0.5,

    -0.55f, -0.5,
    -0.05f, +0.5,
    -0.55f, +0.5
  };
  int vertexCount = static_cast<int>(vertexData.size() / 2);
  WGPUBuffer vertexBuffer = createBuffer(device, "Vertex Buffer", vertexData.size() * sizeof(float), WGPUBufferUsage_CopyDst | WGPUBufferUsage_Vertex);
  writeToBuffer(queue, vertexBuffer, vertexData.data(), vertexData.size() * sizeof(float));

  WGPUVertexAttribute vertexAttribute = createVertexAttribute(
    0, WGPUVertexFormat_Float32x2, 0
  );
  std::vector<WGPUVertexAttribute> attributes = {vertexAttribute};

  WGPUVertexBufferLayout vertexBufferLayout = createVertexBufferLayout(
    1, &attributes, 2 * sizeof(float), WGPUVertexStepMode_Vertex
  );

  WGPURenderPipelineDescriptor pipelineDesc = buildRenderPipelineDescriptor(
    shaderModule, fragmentState, "vs_main"
  );

  pipelineDesc.vertex.bufferCount = 1;
  pipelineDesc.vertex.buffers = &vertexBufferLayout;

  WGPURenderPipeline pipeline =
      wgpuDeviceCreateRenderPipeline(device, &pipelineDesc);

  while (!glfwWindowShouldClose(window)) {
    wgpuDeviceTick(device);
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
    wgpuRenderPassEncoderSetVertexBuffer(renderPass, 0, vertexBuffer, 0, vertexData.size() * sizeof(float));
    wgpuRenderPassEncoderDraw(renderPass, vertexCount, 1, 0, 0);
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
