#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game.h"
#include "resource_manager.h"

#include <iostream>

// glfw funtion declarations
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void key_callback(GLFWwindow *window, int key, int scancode, int action, int mode);

const unsigned int SCREEN_WIDTH = 1280;
const unsigned int SCREEN_HEIGHT = 720;

Game breakout_game(SCREEN_WIDTH, SCREEN_HEIGHT);

int
main(int argc, char *argv[])
{
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
  glfwWindowHint(GLFW_RESIZABLE, false);

  GLFWwindow *window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Breakout", nullptr, nullptr);
  glfwMakeContextCurrent(window);

  if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    std::cout << "Failed to initialize GLAD\n";
    return -1;
  }

  glfwSetKeyCallback(window, key_callback);
  glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

  // OpenGL configuration
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  // initialize game
  breakout_game.init();

  // delta variables
  // ---------------
  float delta_time = 0.0f;
  float last_frame = 0.0f;

  while(!glfwWindowShouldClose(window)) {
    // calculate delta time
    // --------------------
    float current_frame = glfwGetTime();
    delta_time = current_frame - last_frame;
    last_frame = current_frame;
    glfwPollEvents();

    // manage user input
    // -----------------
    breakout_game.process_input(delta_time);

    // update game state
    // -----------------
    breakout_game.update(delta_time);

    // render
    // ------
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    breakout_game.render();

    glfwSwapBuffers(window);
  }

  // delete all loaded resources
  ResourceManager::clear();

  glfwTerminate();
  return 0;
}

void
key_callback(GLFWwindow *window, int key, int scancode, int action, int mode)
{
  if((key == GLFW_KEY_ESCAPE) && (action == GLFW_PRESS)) {
    glfwSetWindowShouldClose(window, true);
  }
  if((key >= 0) && (key <= 1024)) {
    if(action == GLFW_PRESS) {
      breakout_game.keys[key] = true;
    } else if(action == GLFW_RELEASE) {
      breakout_game.keys[key] = false;
    }
  }
}

void
framebuffer_size_callback(GLFWwindow *window, int width, int height)
{
  glViewport(0, 0, width, height);
}