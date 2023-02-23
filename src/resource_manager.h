#ifndef _RESOURCE_MANAGER_H_
#define _RESOURCE_MANAGER_H_

#include <map>
#include <string>

#include "shader.h"
#include "texture.h"

class ResourceManager
{
public: // methods
  // resource storage
  static std::map<std::string, Shader> shaders;
  static std::map<std::string, Texture2D> textures;
  // loads (and generates) a shader program from loading the source code of vertex, fragment, and (optionally) geometry shaders
  static Shader load_shader(const char *vertex_shader_file, const char *fragment_shader_file, const char *geometry_shader_file, std::string name);
  // retrieves a stored shader
  static Shader get_shader(std::string name);
  // loads (and generates) a texture from file
  static Texture2D load_texture(const char *file, bool alpha, std::string name);
  // retrieves a stored texture
  static Texture2D get_texture(std::string name);
  // properly de-allocates all loaded resources
  static void clear();

private: // methods
  // static class; no instantation necessary or desired
  ResourceManager() {}
  // loads and generates a shader from file
  static Shader load_shader_from_file(const char *vertex_shader_file, const char *fragment_shader_file, const char *geometry_shader_file = nullptr);
  // loads a single texture from file
  static Texture2D load_texture_from_file(const char *file, bool alpha);
};

#endif
