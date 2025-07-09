cmake_minimum_required(VERSION 3.25)
project(DearImGui)

FetchContent_Declare(IMGUI
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG f5befd2d29e66809cd1110a152e375a7f1981f06 # 1.91.9b
)
FetchContent_MakeAvailable(IMGUI)
find_package(OpenGL REQUIRED)

add_library(DearImGui
    STATIC
    ${imgui_SOURCE_DIR}/imgui.cpp
    ${imgui_SOURCE_DIR}/imgui_draw.cpp
    ${imgui_SOURCE_DIR}/imgui_tables.cpp
    ${imgui_SOURCE_DIR}/imgui_widgets.cpp

    ${imgui_SOURCE_DIR}/backends/imgui_impl_sdl3.cpp
    ${imgui_SOURCE_DIR}/backends/imgui_impl_opengl3.cpp
    
    ${imgui_SOURCE_DIR}/imgui_demo.cpp
)

target_include_directories(DearImGui PUBLIC 
    ${imgui_SOURCE_DIR}
    ${imgui_SOURCE_DIR}/backends
)
target_link_libraries(DearImGui PUBLIC SDL3::SDL3 OpenGL::GL)
