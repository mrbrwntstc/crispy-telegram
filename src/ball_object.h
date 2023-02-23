#ifndef _BALL_OBJECT_H_
#define _BALL_OBJECT_H_

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "texture.h"

class BallObject : public GameObject
{
public: // fields
  float radius;
  bool stuck;

public: // methods
  BallObject();
  BallObject(glm::vec2 position, float radius, glm::vec2 velocity, Texture2D sprite);
  // moves the ball within the bounds of the window; returns the new position
  glm::vec2 move(float delta_time, unsigned int window_width);
  // resets the ball to original state within given position and velocity
  void reset(glm::vec2 position, glm::vec2 velocity);
};

#endif