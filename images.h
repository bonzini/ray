// Example ray tracing program
// Written by Paolo Bonzini, 2005

#ifndef PTGEN_IMAGES_H
#define PTGEN_IMAGES_H

#include <iostream>
#include <fstream>
#include "v3d.h"

enum image_file_format { OUT_PPM, OUT_PNG };

class Image
  {
  protected:
    typedef unsigned char pixel;

    pixel *image_data;
    int width, height;

  public:
    Image() : image_data(NULL) { };
    ~Image()
      {
	if (image_data)
	  delete[] image_data;
      };

    bool write(std::ostream &os, enum image_file_format fmt) const;
    bool write(const char *file_name, enum image_file_format fmt) const {
      std::ofstream os (file_name, std::ofstream::binary);
      return write (os, fmt);
    }

    const pixel *get_image_data () const { return image_data; }
    pixel *get_image_data () { return image_data; }
    int get_width () const { return width; }
    int get_height () const { return height; }

    void set_size (int w, int h) {
      width = w;
      height = h;
      image_data = new unsigned char[3 * w * h];
    }

    void set_pixel (int x, int y, const Color &c) {
      unsigned char *p = image_data + 3 * (width * y + x);
      p[0] = c.r < 0 ? 0 : (unsigned char) (c.r * 255);
      p[1] = c.g < 0 ? 0 : (unsigned char) (c.g * 255);
      p[2] = c.b < 0 ? 0 : (unsigned char) (c.b * 255);
    }
  };

#endif
