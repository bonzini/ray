// Example ray tracing program
// Written by Paolo Bonzini, 2005

#include "texture.h"
#include "light.h"

Color MonoTexture::get_color (const Point3D &p3d, const Entity &e) const
{
  return pigment;
}

Color UVTexture::get_color (const Point3D &p3d, const Entity &e) const
{
  real u = e.texture_u (p3d);
  real v = e.texture_v (p3d);
  return get_color (u, v);
}

Color CheckerTexture::get_color (real u, real v) const
{
  u /= scale;
  v /= scale;
  bool u_pos = (u - floor (u)) < 0.5;
  bool v_pos = (v - floor (v)) < 0.5;
  return (u_pos == v_pos) ? a : b;
}

namespace colors {
  Color black = Color ();
  Color white = Color (1.0, 1.0, 1.0);
  Color red = Color (1.0, 0.0, 0.0);
  Color green = Color (0.0, 1.0, 0.0);
  Color lightBlue = Color (0.0, 0.0, 1.0);
  Color blue = Color (0.0, 0.0, 0.5);
  Color yellow = Color (1.0, 1.0, 0.0);
}

MonoTexture MonoTexture::black (colors::black);
MonoTexture MonoTexture::white (colors::white);
MonoTexture MonoTexture::red (colors::red);
MonoTexture MonoTexture::green (colors::green);
MonoTexture MonoTexture::lightBlue (colors::lightBlue);
MonoTexture MonoTexture::blue (colors::blue);
MonoTexture MonoTexture::yellow (colors::yellow);

