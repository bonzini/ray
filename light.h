// Example ray tracing program
// Written by Paolo Bonzini, 2005

#ifndef PTGEN_LIGHT_H
#define PTGEN_LIGHT_H

#include "config.h"
#include "geom.h"
#include "v3d.h"

struct AbstractLight {
  bool cast_shadows;

  AbstractLight (bool shadows = true) : cast_shadows (shadows) {}

  virtual Color get_color (const Point3D &p) const = 0;
  virtual const Point3D &get_pos () const = 0;
};

struct Light : AbstractLight {
  Point3D pos;
  Color color;

  static Color white;

  Light (const Point3D &pos_, const Color &color_ = Light::white,
    bool shadows = true) : AbstractLight (shadows), pos (pos_), color (color_) {}

  Color get_color (const Point3D &p) const;
  const Point3D &get_pos () const;
};

struct LightWrapper : AbstractLight {
  const Light &l;

 protected:
  LightWrapper (const Light &base, bool shadows = true) :
    AbstractLight (shadows), l (base) {}
  const Point3D &get_pos () const;
};

struct AttenuatedLight : LightWrapper {
  real attenuation;

  AttenuatedLight (const Light &base, real att = 0.0, bool shadows = true) :
    LightWrapper (base, shadows), attenuation (att) {}

  Color get_color (const Point3D &p) const;
};

struct BoundedLight : LightWrapper {
  const Entity &e;

  BoundedLight (const Light &base, const Entity &e_, bool shadows = true) :
    LightWrapper (base, shadows), e (e_) {}

  Color get_color (const Point3D &p) const;

 private:
  static Color black;
};

#endif
