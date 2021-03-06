// Example ray tracing program
// Written by Paolo Bonzini, 2005

#include "light.h"

AbstractLight::~AbstractLight ()
{
}

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
  return strength / (1 + attenuation * (v * v)) * l.get_color (p);
}

Color BoundedLight::get_color (const Point3D &p) const
{
  if (e.inside (p))
    return l.get_color (p);
  else
    return colors::black;
}

Color DirectedLight::get_color (const Point3D &p) const
{
  if ((p - pos) * dir < 0)
    return colors::black;
  else
    return Light::get_color (p);
}

