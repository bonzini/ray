// Example ray tracing program
// Written by Paolo Bonzini, 2005

#ifndef PTGEN_MATERIAL_H
#define PTGEN_MATERIAL_H

#include "config.h"
#include "v3d.h"
#include "geom.h"

namespace colors {
  extern Color black;
  extern Color white;
  extern Color red;
  extern Color green;
  extern Color lightBlue;
  extern Color blue;
  extern Color yellow;
}


struct Texture {
  virtual Color get_color (const Point3D &p3d, const Entity &e) const = 0;
};

struct MonoTexture : Texture {
  Color pigment;

  explicit MonoTexture (const Color &c) : pigment (c) {}
  MonoTexture (real r_, real g_, real b_) : pigment (r_, g_, b_) {}
  Color get_color (const Point3D &p3d, const Entity &e) const;

  static MonoTexture black;
  static MonoTexture white;
  static MonoTexture red;
  static MonoTexture green;
  static MonoTexture lightBlue;
  static MonoTexture blue;
  static MonoTexture yellow;
};

struct UVTexture : Texture {
  virtual Color get_color (real u, real v) const;
  Color get_color (const Point3D &p3d, const Entity &e) const;
};

struct CheckerTexture : UVTexture {
  Color a, b;
  real scale;

  CheckerTexture (const Color &c1, const Color &c2, real scale_ = 1.0) :
    a (c1), b(c2), scale (scale_) {}
  CheckerTexture (real r1, real g1, real b1, real r2, real g2, real b2,
		  real scale_ = 1.0) :
    a (r1, g1, b1), b(r2, g2, b2), scale (scale_) {}
  Color get_color (real u, real v) const;
};

#endif
