cmake_minimum_required(VERSION 3.20)
project(Rubik3D VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(RUBIK3D_BUILD_APP "Build the FreeGLUT desktop application" ON)
option(RUBIK3D_BUILD_TESTS "Build headless unit tests" ON)

add_library(rubik_core
    src/Math/Matrix3x3.cpp
    src/Math/Matrix4x4.cpp
    src/Geometry/Mesh.cpp
    src/Rubik/RubikMoves.cpp
    src/Rubik/RubiksCube.cpp
    src/Rubik/RotationAnimator.cpp
    src/Graphics/Camera3D.cpp
    src/Graphics/Lighting.cpp
    src/Algorithms/RasterAlgorithms.cpp
    src/Algorithms/Bezier.cpp
)
target_include_directories(rubik_core PUBLIC include)
target_compile_features(rubik_core PUBLIC cxx_std_17)
if(MSVC)
    target_compile_options(rubik_core PRIVATE /W4 /permissive- /utf-8)
else()
    target_compile_options(rubik_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

if(RUBIK3D_BUILD_APP)
    find_package(OpenGL REQUIRED)
    find_package(FreeGLUT CONFIG QUIET)
    if(FreeGLUT_FOUND)
        add_executable(Rubik3D
            src/main.cpp
            src/Graphics/Renderer.cpp
            src/UI/InputManager.cpp
        )
        target_link_libraries(Rubik3D PRIVATE rubik_core OpenGL::GL FreeGLUT::freeglut)
        target_compile_definitions(Rubik3D PRIVATE FREEGLUT_STATIC=0)
        if(MSVC)
            target_compile_options(Rubik3D PRIVATE /W4 /permissive- /utf-8)
        endif()
        add_custom_command(TARGET Rubik3D POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_if_different
                "$<TARGET_FILE:FreeGLUT::freeglut>"
                "$<TARGET_FILE_DIR:Rubik3D>"
        )
    else()
        message(WARNING "FreeGLUT was not found: only rubik_core and tests will be built. Configure with -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake")
    endif()
endif()

if(RUBIK3D_BUILD_TESTS)
    enable_testing()
    add_executable(rubik_tests
        tests/test_main.cpp
        tests/test_matrix.cpp
        tests/test_rubik.cpp
        tests/test_algorithms.cpp
    )
    target_link_libraries(rubik_tests PRIVATE rubik_core)
    add_test(NAME rubik_tests COMMAND rubik_tests)
endif()

