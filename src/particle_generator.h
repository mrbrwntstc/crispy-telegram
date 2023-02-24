#ifndef _PARTICLE_GENERATOR_H_
#define _PARTICLE_GENERATOR_H_

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"
#include "game_object.h"

struct Particle {
  glm::vec2 position;
  glm::vec2 velocity;
  glm::vec4 color;
  float life;

  Particle()
      : position(0.0f),
        velocity(0.0f),
        color(1.0f),
        life(0.0f)
  {
  }
};

class ParticleGenerator
{
public: // methods
  ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);
  // update all particles
  void update(float delta, GameObject &object, unsigned int new_particles, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
  // render all particles
  void draw();

private: // fields
  // state
  std::vector<Particle> particles;
  unsigned int amount;
  // render state
  Shader shader;
  Texture2D texture;
  unsigned int vao;

private: // methods
  // initializes buffer and vertex attributes
  void init();
  // returns the first particle index that's currently unused (life <= 0.0 or 0 if no particle is currently inactive)
  unsigned int first_unused_particle();
  // respawns particles
  void respawn_particles(Particle &particle, GameObject &object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};

#endif