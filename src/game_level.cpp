#include "game_level.h"

#include <fstream>
#include <sstream>

void
GameLevel::load(const char *file, unsigned int level_width, unsigned int level_height)
{
  // clear old data
  this->bricks.clear();
  // load from file
  unsigned int tile_code;
  GameLevel level;
  std::string line;
  std::ifstream level_ifstream(file);
  std::vector<std::vector<unsigned int> > tile_data;
  if(level_ifstream) {
    while(std::getline(level_ifstream, line)) {
      std::istringstream level_istringstream(line);
      std::vector<unsigned int> row;
      while(level_istringstream >> tile_code) {
        row.push_back(tile_code);
      }
      tile_data.push_back(row);
    }
    if(tile_data.size() > 0) {
      this->init(tile_data, level_width, level_height);
    }
  }
}

void
GameLevel::draw(SpriteRenderer &renderer)
{
  for(GameObject &tile : this->bricks) {
    if(!tile.destroyed) {
      tile.draw(renderer);
    }
  }
}

bool
GameLevel::is_completed()
{
  for(GameObject &tile : this->bricks) {
    if(!tile.is_solid && !tile.destroyed) {
      return false;
    }
  }
  return true;
}

void
GameLevel::init(std::vector<std::vector<unsigned int> > tile_data, unsigned int level_width, unsigned int level_height)
{
  // calculate dimensions
  // --------------------
  unsigned int height = tile_data.size();
  unsigned int width = tile_data[0].size(); // we can do this because init() only gets called when height > 0
  float unit_width = level_width / static_cast<float>(width);
  float unit_height = level_height / static_cast<float>(height);

  // initalize level tiles based on tile_data
  // ----------------------------------------
  for(unsigned int row = 0; row < height; row++) {
    for(unsigned int column = 0; column < width; column++) {
      // check block type from level data (2D array)
      if(tile_data[row][column] == 1) {
        // solid brick; cannot be destroyed
        glm::vec2 position(unit_width * column, unit_height * row);
        glm::vec2 size(unit_width, unit_height);
        GameObject brick(position, size, ResourceManager::get_texture("block_solid"), glm::vec3(0.8f, 0.8f, 0.7f));
        brick.is_solid = true;
        this->bricks.push_back(brick);
      } else if(tile_data[row][column] > 1) {
        // can be destroyed; determine color based on level data
        glm::vec3 color = glm::vec3(1.0f); // white
        if(tile_data[row][column] == 2) {
          color = glm::vec3(0.2f, 0.6f, 1.0f);
        } else if(tile_data[row][column] == 3) {
          color = glm::vec3(0.0f, 0.7f, 0.0f);
        } else if(tile_data[row][column] == 4) {
          color = glm::vec3(0.8f, 0.8f, 0.4f);
        } else if(tile_data[row][column] == 5) {
          color = glm::vec3(1.0f, 0.5f, 0.0f);
        }

        glm::vec2 position(unit_width * column, unit_height * row);
        glm::vec2 size(unit_width, unit_height);
        this->bricks.push_back(GameObject(position, size, ResourceManager::get_texture("block"), color));
      }
    }
  }
}