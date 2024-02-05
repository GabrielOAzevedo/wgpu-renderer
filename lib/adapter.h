#ifndef WEBGPU_ADAPTER_H_
#define WEBGPU_ADAPTER_H_
#include <vector>
#include <webgpu/webgpu.h>

WGPUAdapter requestAdapter(WGPUInstance instance,
                           WGPURequestAdapterOptions const *options);
WGPURequestAdapterOptions requestAdapterOptions(WGPUSurface surface);
WGPUAdapter buildDefaultAdapter(WGPUInstance instance);
std::vector<WGPUFeatureName> getAdapterFeatures(WGPUAdapter adapter);

#endif // WEBGPU_ADAPTER_H_
