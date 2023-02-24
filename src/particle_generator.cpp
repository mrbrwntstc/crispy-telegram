#include "particle_generator.h"

ParticleGenerator::ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount)
    : shader(shader),
      texture(texture),
      amount(amount)
{
  this->init();
}

void
ParticleGenerator::update(float delta, GameObject &object, unsigned int new_particles, glm::vec2 offset)
{
  // add new particle
  for(unsigned int i = 0; i < new_particles; ++i) {
    int unused_particle = this->first_unused_particle();
    this->respawn_particles(this->particles[unused_particle], object, offset);
  }
  // update all particles
  for(unsigned int i = 0; i < this->amount; ++i) {
    Particle &p = this->particles[i];
    p.life -= delta; // reduce life
    if(p.life > 0.0f) {
      // particle is alive, so update
      p.position -= p.velocity * delta;
      p.color.a -= delta * 2.5f;
    }
  }
}

void
ParticleGenerator::draw()
{
  // use additive blending to give it a 'glow' effect
  glBlendFunc(GL_SRC_ALPHA, GL_ONE);
  this->shader.use();
  for(Particle particle : this->particles) {
    if(particle.life > 0.0f) {
      this->shader.set_vector2f("offset", particle.position);
      this->shader.set_vector4f("color", particle.color);
      this->texture.bind();
      glBindVertexArray(this->vao);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);
    }
  }
  // reset to default blending mode
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void
ParticleGenerator::init()
{
  // set up mesh and attribute properties
  unsigned int vbo;
  float particle_quad[] = {
    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 0.0f,

    0.0f, 1.0f, 0.0f, 1.0f,
    1.0f, 1.0f, 1.0f, 1.0f,
    1.0f, 0.0f, 1.0f, 0.0f
  };
  glGenVertexArrays(1, &this->vao);
  glGenBuffers(1, &vbo);
  glBindVertexArray(this->vao);
  // fill mesh buffer
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
  // set mesh attributes
  glEnableVertexAttribArray(0);
  glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void *)0);
  glBindVertexArray(0);

  // create this->amount default particle instances
  for(unsigned int i = 0; i < this->amount; ++i) {
    this->particles.push_back(Particle());
  }
}

// stores the index of the last particle used
// this will provide quick access to the next dead particle
unsigned int last_used_particle = 0;

unsigned int
ParticleGenerator::first_unused_particle()
{
  // first search from last_used_particle; this will usually return almost instantly
  for(unsigned int i = last_used_particle; i < this->amount; ++i) {
    if(this->particles[i].life <= 0.0f) {
      last_used_particle = i;
      return i;
    }
  }

  // otherwise, do a linear search
  for(unsigned int i = 0; i < last_used_particle; ++i) {
    if(this->particles[i].life <= 0.0f) {
      last_used_particle = i;
      return i;
    }
  }

  // all particles are taken
  // override the first one
  // if this method repeatedly hits this case, more particles should be reserved
  last_used_particle = 0;
  return 0;
}

void
ParticleGenerator::respawn_particles(Particle &particle, GameObject &object, glm::vec2 offset)
{
  float random = ((rand() % 100) - 50) / 10.0f;
  float random_color = 0.5f + ((rand() % 100) / 100.0f);
  particle.position = object.position + random + offset;
  particle.color = glm::vec4(random_color, random_color, random_color, 1.0f);
  particle.life = 1.0f;
  particle.velocity = object.velocity * 0.1f;
}