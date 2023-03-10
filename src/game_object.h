#ifndef _GAME_OBJECT_H_
#define _GAME_OBJECT_H_

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "texture.h"
#include "sprite_renderer.h"

/**
 * Container object for holding all state relevant for a single game object entity
 * Each object in the game likely needs the minimal state as described within GameObject
 */
class GameObject
{
public: // fields
  // object state
  // ------------
  glm::vec2 position;
  glm::vec2 size;
  glm::vec2 velocity;
  glm::vec3 color;
  float rotation;
  bool is_solid;
  bool destroyed;

  // render state
  // ------------
  Texture2D sprite;

public: // methods
  GameObject();
  GameObject(glm::vec2 position, glm::vec2 size, Texture2D sprite, glm::vec3 color = glm::vec3(1.0f), glm::vec2 velocity = glm::vec2(0.0f, 0.0f));
  // draw sprite
  virtual void draw(SpriteRenderer &renderer);
};

#endif // _GAME_OBJECT_H_