// Example ray tracing program
// Written by Paolo Bonzini, 2005

#include "texture.h"

Color MonoTexture::get_color (const Point3D &p3d) const
{
  return pigment;
}
