#version 330 core
layout (location = 0) in vec4 vertex; // <vec2 position, vec2 textureCoordinates>

out vec2 textureCoordinates;
out vec4 particleColor;

uniform mat4 projection;
uniform vec2 offset;
uniform vec4 color;

void main()
{
  float scale = 10.0f;
  textureCoordinates = vertex.zw;
  particleColor = color;
  gl_Position = projection * vec4((vertex.xy * scale) + offset, 0.0, 1.0);
}