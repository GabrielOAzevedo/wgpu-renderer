cmake_minimum_required(VERSION 3.0...3.25)
project(
  LearnWebGPU
  VERSION 0.1.0
  LANGUAGES CXX C
)
add_subdirectory(dependencies/glfw)
add_subdirectory(dependencies/webgpu)
add_subdirectory(dependencies/glfw3webgpu)

set(SOURCES main.cpp)

add_executable(App 
  ${SOURCES}
  lib/adapter.cpp
  lib/device.cpp
  lib/command_queue.cpp
  lib/command_encoder.cpp
  lib/swapchain.cpp
  lib/renderpass.cpp
  lib/pipeline.cpp
  lib/shaders.cpp
  lib/adapter.h
  lib/device.h
  lib/command_queue.h
  lib/command_encoder.h
  lib/swapchain.h
  lib/renderpass.h
  lib/pipeline.h
  lib/shaders.h
)
target_link_libraries(App PRIVATE glfw webgpu glfw3webgpu)
target_include_directories(App PUBLIC lib)

set_target_properties(App PROPERTIES
    CXX_STANDARD 17
    CXX_EXTENSIONS OFF
    COMPILE_WARNING_AS_ERROR ON
)
if (MSVC)
    target_compile_options(App PRIVATE /W4)
else()
    target_compile_options(App PRIVATE -Wall -Wextra -pedantic)
endif()
if(XCODE)
    set_target_properties(App PROPERTIES
        XCODE_GENERATE_SCHEME ON
        XCODE_SCHEME_ENABLE_GPU_FRAME_CAPTURE_MODE "Metal")
endif()
target_copy_webgpu_binaries(App)
