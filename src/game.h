#ifndef _GAME_H_
#define _GAME_H_

#include <glad/glad.h>
#include <glfw/glfw3.h>

#include "game_level.h"

// currenet state of the game
enum GameState {
  Active,
  Menu,
  Win
};

enum Direction {
  Up,
  Right,
  Down,
  Left
};

// <collision occured?, what direction?, difference vector center - closest point>
typedef std::tuple<bool, Direction, glm::vec2> collision;

const glm::vec2 player_size(100.0f, 20.0f);
const float player_velocity(500.0f);

const glm::vec2 initial_ball_velocity(100.0f, -350.0f);
const float ball_radius = 12.5f;

class Game
{
public: // fields
  // game state
  GameState state;
  bool keys[1024];
  unsigned int width;
  unsigned int height;
  std::vector<GameLevel> levels;
  unsigned int level;

public: // methods
  Game(unsigned int width, unsigned int height);
  ~Game();
  // initialize game state (load all shaders and textures/levels)
  void init();
  // game loop
  void process_input(float delta);
  void update(float delta);
  void render();
  void do_collisions();
  // reset
  void reset_level();
  void reset_player();
};

#endif