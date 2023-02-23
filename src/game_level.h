#ifndef _GAME_LEVEL_H_
#define _GAME_LEVEL_H_

#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"

// holds all of the tiles as part of a breakout level
// also hosts functionality to load/render levels from the hard disk
class GameLevel
{
public: // fields
  // level state
  std::vector<GameObject> bricks;

public: // methods
  GameLevel() {}
  // loads level from file
  void load(const char *file, unsigned int level_width, unsigned int level_height);
  // render level
  void draw(SpriteRenderer &renderer);
  // check if all non-solid tiles are destroyed, completing the level
  bool is_completed();

private: // methods
  // initialize level from tile data
  void init(std::vector<std::vector<unsigned int> > tile_data, unsigned int level_width, unsigned int level_height);
};

#endif // _GAME_LEVEL_H_