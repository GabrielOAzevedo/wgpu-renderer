#ifndef WEBGPU_ADAPTER_H_
#define WEBGPU_ADAPTER_H_
#include <webgpu/webgpu.h>

WGPUAdapter requestAdapter(WGPUInstance instance,
                           WGPURequestAdapterOptions const *options);

#endif // WEBGPU_ADAPTER_H_
