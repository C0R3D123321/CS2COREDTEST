cmake_minimum_required(VERSION 3.10)
project(CS2Cheat)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS TRUE)
set(BUILD_SHARED_LIBS TRUE)

include_directories(${CMAKE_SOURCE_DIR}/imgui ${CMAKE_SOURCE_DIR}/kiero)

add_library(CS2Cheat SHARED main.cpp math.cpp visuals.cpp
    imgui/imgui.cpp imgui/imgui_draw.cpp imgui/imgui_tables.cpp imgui/imgui_widgets.cpp
    imgui/imgui_impl_win32.cpp imgui/imgui_impl_dx11.cpp
    kiero/kiero.cpp)

target_link_libraries(CS2Cheat d3d11 dxgi)
