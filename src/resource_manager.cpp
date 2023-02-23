#include "resource_manager.h"

#include <fstream>
#include <iostream>
#include <sstream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// static variables
std::map<std::string, Texture2D> ResourceManager::textures;
std::map<std::string, Shader> ResourceManager::shaders;

Shader
ResourceManager::load_shader(const char *vertex_shader_file, const char *fragment_shader_file, const char *geometry_shader_file, std::string name)
{
  shaders[name] = load_shader_from_file(vertex_shader_file, fragment_shader_file, geometry_shader_file);
  return shaders[name];
}

Shader
ResourceManager::get_shader(std::string name)
{
  return shaders[name];
}

Texture2D
ResourceManager::load_texture(const char *file, bool alpha, std::string name)
{
  textures[name] = load_texture_from_file(file, alpha);
  return textures[name];
}

Texture2D
ResourceManager::get_texture(std::string name)
{
  return textures[name];
}

void
ResourceManager::clear()
{
  // (properly) delete all shaders
  for(auto iter : shaders) {
    glDeleteProgram(iter.second.id);
  }

  // (properly) delete all textures
  for(auto iter : textures) {
    glDeleteTextures(1, &iter.second.id);
  }
}

Shader
ResourceManager::load_shader_from_file(const char *vertex_shader_file, const char *fragment_shader_file, const char *geometry_shader_file)
{
  // retrieve the vertex/fragment source code from filepath
  std::string vertex_code;
  std::string fragment_code;
  std::string geometry_code;
  try {
    // open files
    std::ifstream vertex_shader_ifstream(vertex_shader_file);
    std::ifstream fragment_shader_ifstream(fragment_shader_file);
    std::stringstream vertex_shader_stringstream, fragment_shader_stringstream;
    // read file's buffer contents into streams
    vertex_shader_stringstream << vertex_shader_ifstream.rdbuf();
    fragment_shader_stringstream << fragment_shader_ifstream.rdbuf();
    // close file handlers
    vertex_shader_ifstream.close();
    fragment_shader_ifstream.close();
    // convert stream into string
    vertex_code = vertex_shader_stringstream.str();
    fragment_code = fragment_shader_stringstream.str();
    // load geometry shader if available
    if(geometry_shader_file != nullptr) {
      std::ifstream geometry_shader_ifstream(geometry_shader_file);
      std::stringstream geometry_shader_stringstream;
      geometry_shader_stringstream << geometry_shader_ifstream.rdbuf();
      geometry_shader_ifstream.close();
      geometry_code = geometry_shader_stringstream.str();
    }
  } catch(const std::exception &e) {
    std::cerr << e.what() << '\n';
  }

  const char *vertex_shader_code = vertex_code.c_str();
  const char *fragment_shader_code = fragment_code.c_str();
  const char *geometry_shader_code = geometry_code.c_str();
  // now create shader objects from source code
  Shader shader;
  shader.compile(vertex_shader_code, fragment_shader_code, geometry_shader_file != nullptr ? geometry_shader_code : nullptr);
  return shader;
}

Texture2D
ResourceManager::load_texture_from_file(const char *file, bool alpha)
{
  // create texture object
  Texture2D texture;
  if(alpha) {
    texture.internal_format = GL_RGBA;
    texture.image_format = GL_RGBA;
  }
  // load image
  int width, height, num_color_channels;
  unsigned char *data = stbi_load(file, &width, &height, &num_color_channels, 0);
  // now generate texture
  texture.generate(width, height, data);
  // and finally free image data
  stbi_image_free(data);
  return texture;
}