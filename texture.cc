// Example ray tracing program
// Written by Paolo Bonzini, 2005

#include "texture.h"
#include "light.h"

Color MonoTexture::get_color (const Point3D &p3d) const
{
  return pigment;
}

namespace colors {
  Color black = Color ();
  Color white = Color (1.0, 1.0, 1.0);
  Color red = Color (1.0, 0.0, 0.0);
  Color green = Color (0.0, 1.0, 0.0);
  Color lightBlue = Color (0.0, 0.0, 1.0);
  Color blue = Color (0.0, 0.0, 0.5);
}

MonoTexture MonoTexture::black (colors::black);
MonoTexture MonoTexture::white (colors::white);
MonoTexture MonoTexture::red (colors::red);
MonoTexture MonoTexture::green (colors::green);
MonoTexture MonoTexture::lightBlue (colors::lightBlue);
MonoTexture MonoTexture::blue (colors::blue);

