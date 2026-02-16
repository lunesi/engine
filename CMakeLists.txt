set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.10)
project(engine)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Vulkan REQUIRED)
find_package(glfw3 CONFIG REQUIRED)


set(IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/external/imgui)


set(IMGUI_SOURCES
    ${IMGUI_DIR}/imgui.cpp
    ${IMGUI_DIR}/imgui_draw.cpp
    ${IMGUI_DIR}/imgui_widgets.cpp
    ${IMGUI_DIR}/imgui_tables.cpp
    ${IMGUI_DIR}/imgui_demo.cpp
    ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
    ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
)

add_executable(engine
    src/main.cpp
    src/VulcanApp.cpp
    src/lve_window.cpp
    src/lve_pipeline.cpp
    ${IMGUI_SOURCES}
)


target_include_directories(engine PRIVATE
    src
    ${Vulkan_INCLUDE_DIRS}
    ${IMGUI_DIR}/
    ${IMGUI_DIR}/backends
)


target_link_libraries(engine PRIVATE Vulkan::Vulkan glfw)

if(WIN32)
    target_link_libraries(engine PRIVATE user32 gdi32 shell32)
endif()
