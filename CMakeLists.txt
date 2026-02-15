cmake_minimum_required(VERSION 3.20)
project(raylib_dino_runner C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(raylib QUIET)

if(NOT raylib_FOUND)
    include(FetchContent)
    if(EMSCRIPTEN)
        set(PLATFORM Web CACHE STRING "" FORCE)
    endif()
    set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.5
    )
    FetchContent_MakeAvailable(raylib)
endif()

add_executable(raylib_dino_runner
    src/main.c
    src/animation.c
    src/art_assets.c
    src/audio_events.c
    src/game.c
    src/player.c
    src/obstacle.c
    src/score_store.c
)

target_include_directories(raylib_dino_runner PRIVATE include)
target_link_libraries(raylib_dino_runner PRIVATE raylib)
if(UNIX AND NOT APPLE)
    target_link_libraries(raylib_dino_runner PRIVATE m)
endif()

if(EMSCRIPTEN)
    set_target_properties(raylib_dino_runner PROPERTIES
        SUFFIX ".html"
        LINK_FLAGS "-s USE_GLFW=3 -s ASYNCIFY --shell-file=${CMAKE_SOURCE_DIR}/web/shell.html --preload-file ${CMAKE_SOURCE_DIR}/assets/runtime@assets/runtime"
    )
endif()

include(CTest)

if(BUILD_TESTING)
    add_executable(animation_test
        tests/animation_test.c
        src/animation.c
    )
    target_include_directories(animation_test PRIVATE include)
    add_test(NAME animation_test COMMAND animation_test)
endif()
