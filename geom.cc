// Example ray tracing program
// Written by Paolo Bonzini, 2005

#include <cmath>
#include <cstdlib>

#include "config.h"
#include "v3d.h"
#include "geom.h"

real Intersection::inf = INFINITY;

bool Plane::inside (const Point3D &p) const
{
  return Vector3D (p) * normal + d >= 0;
}

bool Plane::intersect (Intersection &i, const Object &o, real tlim) const
{
  const NormRay3D &r = i.r;
  real denom = r.dir * normal;
  if (denom == 0.0)
    return false;

  real t = -(Vector3D (r.source) * normal + d) / denom;
  if (t > tlim && t < i.t)
    {
      i.t = t;
      i.entity = this;
      i.object = &o;
      return true;
    }
  else
    return false;
}

UnitVector3D Plane::get_normal (const Point3D &p) const
{
  return normal;
}

UnitVector3D Plane::get_normal (const Intersection &i) const
{
  return normal;
}

bool Sphere::inside (const Point3D &p) const
{
  Vector3D delta = p - center;
  return delta * delta <= r2;
}

bool Sphere::intersect (Intersection &i, const Object &o, real tlim) const
{
  const NormRay3D &r = i.r;
  Vector3D p_to_center = center - r.source;
  real tpp = p_to_center * r.dir;
  real tdc2 = r2 - (p_to_center * p_to_center - tpp * tpp);
  if (tdc2 < 0.0)
    return false;

  // Return the point that is in front of the observer, or the closest one.
  real tdc = sqrt (tdc2);
  bool from_inside = tpp - tdc < tlim;
  real t = from_inside ? tpp + tdc : tpp - tdc;
  if (t > tlim && t < i.t)
    {
      i.from_inside = from_inside;
      i.t = t;
      i.entity = this;
      i.object = &o;
      return true;
    }
  else
    return false;
}

UnitVector3D Sphere::get_normal (const Point3D &p) const
{
  return UnitVector3D ((p - center) / r);
}

UnitVector3D Sphere::get_normal (const Intersection &i) const
{
  return UnitVector3D ((i.r (i.t) - center) / r);
}


bool ReverseSphere::inside (const Point3D &p) const
{
  Vector3D delta = p - center;
  return delta * delta >= r2;
}

UnitVector3D ReverseSphere::get_normal (const Point3D &p) const
{
  return UnitVector3D ((center - p) / r);
}

UnitVector3D ReverseSphere::get_normal (const Intersection &i) const
{
  return UnitVector3D ((center - i.r (i.t)) / r);
}

UnitVector3D CSGEntity::get_normal (const Intersection &i) const
{
  std::abort ();
}

UnitVector3D CSGEntity::get_normal (const Point3D &p) const
{
  std::abort ();
}

bool BoundingBox::inside (const Point3D &p) const
{
  return bbox.inside (p) && obj.inside (p);
}

bool BoundingBox::intersect (Intersection &i, const Object &o, real tlim) const
{
  Intersection i1 (i.r);
  return (bbox.intersect (i1, o, tlim) || bbox.inside (i.r.source))
	 && obj.intersect (i, o, tlim);
}

bool Difference::inside (const Point3D &p) const
{
  return obj.inside (p) && !bite.inside (p);
}

bool Difference::intersect (Intersection &i, const Object &o, real tlim) const
{
  Intersection i1 (i);
  bool had_intersection = obj.intersect (i1, o, tlim);
  if (!had_intersection)
    return false;

  // controllare...
  Point3D p = i1.r (i1.t);
  if (bite.inside (p))
    return bite.intersect (i, o, i1.t);

  i = i1;
  return true;
}

bool Union::inside (const Point3D &p) const
{
  return obj.inside (p) || next.inside (p);
}

bool Union::intersect (Intersection &i, const Object &o, real tlim) const
{
  bool had_intersection = obj.intersect (i, o, tlim);
  const Union *this_union = this;
  while (this_union->next_is_union)
    {
      this_union = static_cast <const Union *> (&next);
      had_intersection |= this_union->obj.intersect (i, o, tlim);
    }

  return had_intersection | this_union->next.intersect (i, o, tlim);
}
