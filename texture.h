// Example ray tracing program
// Written by Paolo Bonzini, 2005

#ifndef PTGEN_MATERIAL_H
#define PTGEN_MATERIAL_H

#include "config.h"
#include "v3d.h"

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
  virtual Color get_color (const Point3D &p3d) const = 0;
};

struct MonoTexture : Texture {
  Color pigment;

  explicit MonoTexture (const Color &c) : pigment (c) {}
  MonoTexture (real r_, real g_, real b_) : pigment (r_, g_, b_) {}
  Color get_color (const Point3D &p3d) const;

  static MonoTexture black;
  static MonoTexture white;
  static MonoTexture red;
  static MonoTexture green;
  static MonoTexture lightBlue;
  static MonoTexture blue;
  static MonoTexture yellow;
};

#endif
