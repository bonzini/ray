// Example ray tracing program
// Written by Paolo Bonzini, 2005

#ifndef PTGEN_MATERIAL_H
#define PTGEN_MATERIAL_H

#include "config.h"
#include "v3d.h"

struct Texture {
  virtual Color get_color (const Point3D &p3d) const = 0;
};

struct MonoTexture : Texture {
  Color pigment;

  explicit MonoTexture (const Color &c) : pigment (c) {}
  Color get_color (const Point3D &p3d) const;
};

#endif
