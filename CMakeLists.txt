cmake_minimum_required(VERSION 3.25)
project(SDLMouseTest
	LANGUAGES C CXX
	VERSION 0.0.1
)
include(FetchContent)

FetchContent_Declare(GLAD
    GIT_REPOSITORY https://github.com/Dav1dde/glad.git
    GIT_TAG 73db193f853e2ee079bf3ca8a64aa2eaf6459043 # 2.0.8
    SOURCE_SUBDIR cmake
)
FetchContent_Declare(SDL3
    GIT_REPOSITORY https://github.com/libsdl-org/SDL.git
	GIT_TAG 877399b2b2cf21e67554ed9046410f268ce1d1b2 # 3.2.10
    FIND_PACKAGE_ARGS 3.2.10
)
FetchContent_MakeAvailable(GLAD)
FetchContent_MakeAvailable(SDL3)

find_package(OpenGL REQUIRED)
find_package(SDL3)

add_subdirectory(external/imgui)

glad_add_library(glad_gles3_2 API gles2=3.2)

add_executable(main WIN32 src/main.cpp)
if(MSVC)
    target_compile_options(main PRIVATE /W4 /WX)
else()
    target_compile_options(main PRIVATE -Wall -Wextra -Werror)
endif()

target_include_directories(main PRIVATE include)
target_link_libraries(main PRIVATE SDL3::SDL3 DearImGui glad_gles3_2)

set_target_properties(main PROPERTIES
    BUILD_WITH_INSTALL_RPATH ON
    INSTALL_RPATH \$ORIGIN
)

# Copy dlls to build directory
if(WIN32)
    add_custom_command(
        TARGET main POST_BUILD
        COMMAND "${CMAKE_COMMAND}" -E copy $<TARGET_RUNTIME_DLLS:main> $<TARGET_FILE_DIR:main>
        COMMAND_EXPAND_LISTS
    )
endif()

# Install Rules
include(external/get-all-dependencies.cmake)
get_all_dependencies(main main_deps NO_STATIC NO_SYSTEM)
message("Link Libraries (main): ${main_deps}")