#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 poition, vec2 textureCoordinates>

out vec2 textureCoordinates;

uniform mat4 model;
// note that we're omitting the view matrix; the view never changes
uniform mat4 projection;

void main()
{
  textureCoordinates = vertex.zw;
  gl_Position = projection * model * vec4(vertex.xy, 0.0, 1.0);
}