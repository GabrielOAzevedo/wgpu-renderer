#ifndef DEVICE_H
#define DEVICE_H
#include <webgpu/webgpu.h>

WGPUDevice requestDevice(WGPUAdapter adapter,
                         WGPUDeviceDescriptor const *descriptor);

#endif // DEVICE_H
