// Example ray tracing program
// Written by Paolo Bonzini, 2005

#include "light.h"

Color Light::white = Color (1.0, 1.0, 1.0);
Color BoundedLight::black = Color ();

Color Light::get_color (const Point3D &p) const
{
  return color;
}

const Point3D &Light::get_pos () const
{
  return pos;
}

const Point3D &LightWrapper::get_pos () const
{
  return l.get_pos ();
}

Color AttenuatedLight::get_color (const Point3D &p) const
{
  Vector3D v = p - l.get_pos ();
  return l.get_color (p) / (1 + attenuation * (v * v));
}

Color BoundedLight::get_color (const Point3D &p) const
{
  if (e.inside (p))
    return black;
  else
    return l.get_color (p);
}

