#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <glad/glad.h>

// solely designed for 2D textures
class Texture2D
{
public: // fields
  // unique ID of texture object
  unsigned int id;
  // texture image dimensions
  // ------------------------
  unsigned int width;
  unsigned int height;
  // texture format
  // --------------
  // format of texture object
  unsigned int internal_format;
  // format of loaded image
  unsigned int image_format;
  // texture configuration
  // ---------------------
  // wrap on s-axis (x-axis)
  unsigned int wrap_s;
  // wrap on t-axis (y-axis)
  unsigned int wrap_t;
  // how to filter if texture pixels < screen pixels
  unsigned int filter_min;
  // how to filter if texture pixels > screen pixels
  unsigned int filter_max;

public: // methods
  Texture2D();
  // generate texture from image data
  void generate(unsigned int width, unsigned int height, unsigned char *data);
  // binds the texture as the current active GL_TEXTURE_2D texture object
  void bind() const;
};

#endif