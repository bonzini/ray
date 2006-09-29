// Example ray tracing program
// Written by Paolo Bonzini, 2005

#ifndef PTGEN_LIGHT_H
#define PTGEN_LIGHT_H

#include "config.h"
#include "geom.h"
#include "v3d.h"
#include "texture.h"

struct AbstractLight {
  bool cast_shadows;

  AbstractLight (bool shadows = true) : cast_shadows (shadows) {}

  virtual ~AbstractLight ();
  virtual Color get_color (const Point3D &p) const = 0;
  virtual const Point3D &get_pos () const = 0;
};

struct Light : AbstractLight {
  Point3D pos;
  Color color;

  Light (const Point3D &pos_, const Color &color_ = colors::white,
    bool shadows = true) : AbstractLight (shadows), pos (pos_),
    color (color_) {}

  Light (real x_, real y_, real z_, const Color &color_ = colors::white,
    bool shadows = true) : AbstractLight (shadows), pos (x_, y_, z_),
    color (color_) {}

  Color get_color (const Point3D &p) const;
  const Point3D &get_pos () const;
};

struct LightWrapper : AbstractLight {
  const AbstractLight &l;

 protected:
  LightWrapper (const AbstractLight &base, bool shadows = true) :
    AbstractLight (shadows), l (base) {}
  const Point3D &get_pos () const;
};

struct AttenuatedLight : LightWrapper {
  real strength;
  real attenuation;

  AttenuatedLight (const AbstractLight &base, real att = 0.0, real strength_ = 1.0,
    bool shadows = true) : LightWrapper (base, shadows), strength (strength_),
    attenuation (att) {}

  Color get_color (const Point3D &p) const;
};

struct BoundedLight : LightWrapper {
  const Entity &e;

  BoundedLight (const AbstractLight &base, const Entity &e_, bool shadows = true) :
    LightWrapper (base, shadows), e (e_) {}

  Color get_color (const Point3D &p) const;
};

struct DirectedLight : Light {
  Vector3D dir;

  DirectedLight (const Point3D &p_, const Vector3D &v_, const Color &c_,
    bool shadows = true) :
    Light (p_, c_, shadows), dir (v_) {}
  DirectedLight (real x, real y, real z, real dx, real dy, real dz,
    const Color &c_, bool shadows = true) :
    Light (x, y, z, c_, shadows), dir (dx, dy, dz) {}

  Color get_color (const Point3D &p) const;
};

#endif
