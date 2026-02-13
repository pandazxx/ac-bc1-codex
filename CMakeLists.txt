cmake_minimum_required(VERSION 3.20)
project(raylib_dino_runner C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(raylib REQUIRED)

add_executable(raylib_dino_runner
    src/main.c
    src/game.c
    src/player.c
    src/obstacle.c
    src/score_store.c
)

target_include_directories(raylib_dino_runner PRIVATE include)
target_link_libraries(raylib_dino_runner PRIVATE raylib)

if(EMSCRIPTEN)
    set_target_properties(raylib_dino_runner PROPERTIES
        SUFFIX ".html"
        LINK_FLAGS "-s USE_GLFW=3 -s ASYNCIFY --shell-file=${CMAKE_SOURCE_DIR}/web/shell.html"
    )
endif()

include(CTest)
