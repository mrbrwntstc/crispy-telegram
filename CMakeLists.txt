cmake_minimum_required(VERSION 3.24)
project("breakout")

set(SOURCE_FILES
  "src/game.cpp" "src/game.h"
  "src/shader.cpp" "src/shader.h"
  "src/texture.cpp" "src/texture.h"
  "src/resource_manager.cpp" "src/resource_manager.h"
  "src/sprite_renderer.cpp" "src/sprite_renderer.h"
  "src/game_object.cpp" "src/game_object.h"
  "src/game_level.cpp" "src/game_level.h"
  "src/ball_object.cpp" "src/ball_object.h"
  "src/stb_image.h"
  "src/program.cpp"
)

find_package(glad CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)

add_executable(breakout ${SOURCE_FILES})

target_link_libraries(breakout
PRIVATE
  glad::glad
  glfw
  glm::glm
)
