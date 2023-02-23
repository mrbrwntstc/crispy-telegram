#ifndef _SPRITE_RENDERER_H_
#define _SPRITE_RENDERER_H_

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "texture.h"
#include "shader.h"

class SpriteRenderer
{
public: // methods
  // initialize shaders and shapes
  SpriteRenderer(Shader &shader);
  // destructor
  ~SpriteRenderer();
  // renders a defined quad textures with given sprite
  void draw_sprite(Texture2D &texture, glm::vec2 position, glm::vec2 size = glm::vec2(10.0f, 10.0f), float rotate = 0.0f, glm::vec3 color = glm::vec3(1.0f));

private: // fields
  // render state
  Shader shader;
  unsigned int quad_vao;

private: // methods
  // initializes and configures the quad's buffer and vertex attributes
  void init_render_data();
};

#endif