#include "shader.h"

#include <iostream>

Shader &
Shader::use()
{
  glUseProgram(this->id);
  return *this;
}

void
Shader::compile(const char *vertex_source, const char *fragment_source, const char *geometry_source)
{
  // vertex shader
  unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vertex_shader, 1, &vertex_source, NULL);
  glCompileShader(vertex_shader);
  check_compile_errors(vertex_shader, "VERTEX");

  // fragment shader
  unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fragment_shader, 1, &fragment_source, NULL);
  glCompileShader(fragment_shader);
  check_compile_errors(fragment_shader, "FRAGMENT");

  // geometry shader
  unsigned int geometry_shader;
  if(geometry_source != nullptr) {
    geometry_shader = glCreateShader(GL_GEOMETRY_SHADER);
    glShaderSource(geometry_shader, 1, &geometry_source, NULL);
    glCompileShader(geometry_shader);
    check_compile_errors(geometry_shader, "GEOMETRY");
  }

  // shader program
  this->id = glCreateProgram();
  glAttachShader(this->id, vertex_shader);
  glAttachShader(this->id, fragment_shader);
  if(geometry_source != nullptr)
    glAttachShader(this->id, geometry_shader);
  glLinkProgram(this->id);
  check_compile_errors(this->id, "PROGRAM");
  // delete the shaders as they're linked to our program now and no longer necessary
  glDeleteShader(vertex_shader);
  glDeleteShader(fragment_shader);
  if(geometry_source != nullptr)
    glDeleteShader(geometry_shader);
}

void
Shader::set_float(const char *name, float value, bool use_shader)
{
  if(use_shader)
    this->use();
  glUniform1f(glGetUniformLocation(this->id, name), value);
}

void
Shader::set_integer(const char *name, int value, bool use_shader)
{
  if(use_shader)
    this->use();
  glUniform1i(glGetUniformLocation(this->id, name), value);
}

void
Shader::set_vector2f(const char *name, float x, float y, bool use_shader)
{
  if(use_shader)
    this->use();
  glUniform2f(glGetUniformLocation(this->id, name), x, y);
}

void
Shader::set_vector2f(const char *name, const glm::vec2 &value, bool use_shader)
{
  if(use_shader)
    this->use();
  glUniform2f(glGetUniformLocation(this->id, name), value.x, value.y);
}

void
Shader::set_vector3f(const char *name, float x, float y, float z, bool use_shader)
{
  if(use_shader)
    this->use();
  glUniform3f(glGetUniformLocation(this->id, name), x, y, z);
}

void
Shader::set_vector3f(const char *name, const glm::vec3 &value, bool use_shader)
{
  if(use_shader)
    this->use();
  glUniform3f(glGetUniformLocation(this->id, name), value.x, value.y, value.z);
}

void
Shader::set_vector4f(const char *name, float x, float y, float z, float w, bool use_shader)
{
  if(use_shader)
    this->use();
  glUniform4f(glGetUniformLocation(this->id, name), x, y, z, w);
}

void
Shader::set_vector4f(const char *name, const glm::vec4 &value, bool use_shader)
{
  if(use_shader)
    this->use();
  glUniform4f(glGetUniformLocation(this->id, name), value.x, value.y, value.z, value.w);
}

void
Shader::set_matrix4(const char *name, const glm::mat4 &value, bool use_shader)
{
  if(use_shader)
    this->use();
  glUniformMatrix4fv(glGetUniformLocation(this->id, name), 1, false, glm::value_ptr(value));
}

void
Shader::check_compile_errors(unsigned int object, std::string type)
{
  int success;
  char info_log[1024];
  if(type != "PROGRAM") {
    glGetShaderiv(object, GL_COMPILE_STATUS, &success);
    if(!success) {
      glGetShaderInfoLog(object, 1024, NULL, info_log);
      std::cout << type << " shader error: " << info_log << std::endl;
    }
  } else {
    glGetProgramiv(object, GL_LINK_STATUS, &success);
    if(!success) {
      glGetProgramInfoLog(object, 1024, NULL, info_log);
      std::cout << "shader program link error: " << info_log << std::endl;
    }
  }
}