// Example ray tracing program
// Written by Paolo Bonzini, 2005

#ifndef PTGEN_GEOM_H
#define PTGEN_GEOM_H

#include "config.h"
#include "v3d.h"

class Entity;
class Object;

struct Intersection {
 private:
  static real inf;

 public:
  NormRay3D r;
  const Entity *entity;
  const Object *object;
  real t;
  bool from_inside;

  explicit Intersection (const Ray3D &r_) :
    r(r_.normalize ()), entity (NULL), object (NULL), t (inf),
    from_inside (false) {}
  explicit Intersection (const NormRay3D &r_) :
    r(r_), entity (NULL), object (NULL), t (inf), from_inside (false) {}
  Intersection (const Point3D &r_, const Vector3D &v_) :
    r(r_, v_), entity (NULL), object (NULL), t (inf), from_inside (false) {}
  Intersection (const Point3D &r_, const UnitVector3D &v_) :
    r(r_, v_), entity (NULL), object (NULL), t (inf), from_inside (false) {}
  Intersection (const Ray3D &r_, real t_) :
    r(r_.source, r_.dir, t_), entity (NULL), object (NULL),
    t (inf), from_inside (false) {}
  Intersection (const NormRay3D &r_, real t_) :
    r(r_.source, r_.dir, t_), entity (NULL), object (NULL),
    t (inf), from_inside (false) {}
  Intersection (const Intersection &i, Vector3D &dir, real t_ = 0.0) :
    r(i.r (i.t), dir, t_), entity (i.entity), object (i.object),
    t (t_), from_inside (false) {}
  Intersection (const Intersection &i, UnitVector3D &dir, real t_ = 0.0) :
    r(i.r (i.t), dir, t_), entity (i.entity), object (i.object),
    t (t_), from_inside (false) {}
};

class Entity {
 public:
  virtual bool inside (const Point3D &p) const = 0;
  virtual bool intersect (Intersection &i, const Object &o, real tlim = 0.0)
    const = 0;
  virtual real texture_u (const Point3D &p) const = 0;
  virtual real texture_v (const Point3D &p) const = 0;
  virtual UnitVector3D get_normal (const Intersection &i) const = 0;
  virtual UnitVector3D get_normal (const Point3D &p) const = 0;
};

class Plane : public Entity {
  UnitVector3D normal;
  real d;

 public:
  Plane () :
    normal (0, 1, 0), d (0) {}
  Plane (Vector3D normal_, real d_) :
    normal (normal_.normalize ()), d (d_) {}
  Plane (real a_, real b_, real c_, real d_) :
    normal (Vector3D (a_, b_, c_).normalize ()), d (d_) {}
  Plane (Point3D a_, Point3D b_, Point3D c_) :
    normal (((b_ - a_) ^ (c_ - a_)).normalize ()),
    d (-Vector3D (a_) * normal) {}
  Plane (Point3D a_, Vector3D b_, Vector3D c_) :
    normal ((b_ ^ c_).normalize ()), d (-Vector3D (a_) * normal) {}

  bool inside (const Point3D &p) const;
  bool intersect (Intersection &i, const Object &o, real tlim = 0.0) const;
  real texture_u (const Point3D &p) const;
  real texture_v (const Point3D &p) const;
  UnitVector3D get_normal (const Intersection &i) const;
  UnitVector3D get_normal (const Point3D &p) const;
};

class Sphere : public Entity {
 protected:
  Point3D center;
  real r, r2;

 public:
  Sphere () :
    center (0, 0, 1), r (1), r2 (1) {}
  Sphere (Point3D center_, real r_) :
    center (center_), r (r_), r2 (r_ * r_) {}
  Sphere (real x_, real y_, real z_, real r_) :
    center (x_, y_, z_), r (r_), r2 (r_ * r_) {}

  bool inside (const Point3D &p) const;
  bool intersect (Intersection &i, const Object &o, real tlim = 0.0) const;
  real texture_u (const Point3D &p) const;
  real texture_v (const Point3D &p) const;
  UnitVector3D get_normal (const Intersection &i) const;
  UnitVector3D get_normal (const Point3D &p) const;
};

class ReverseSphere : public Sphere {
 public:
  ReverseSphere (Point3D center, real r_) :
    Sphere (center, r_) {}
  ReverseSphere (real x_, real y_, real z_, real r_) :
    Sphere (x_, y_, z_, r_) {}

  bool inside (const Point3D &p) const;
  UnitVector3D get_normal (const Intersection &i) const;
  UnitVector3D get_normal (const Point3D &p) const;
};

class CSGEntity : public Entity {
  UnitVector3D get_normal (const Intersection &i) const;
  UnitVector3D get_normal (const Point3D &p) const;
  real texture_u (const Point3D &p) const;
  real texture_v (const Point3D &p) const;
};

class BoundingBox : public CSGEntity {
  const Entity &obj, &bbox;

 public:
  BoundingBox (const Entity &obj_, const Entity &bbox_) :
    obj (obj_), bbox (bbox_) {};

  bool inside (const Point3D &p) const;
  bool intersect (Intersection &i, const Object &o, real tlim = 0.0) const;
};

class Difference : public CSGEntity {
  const Entity &obj, &bite;

 public:
  Difference (const Entity &obj_, const Entity &bite_) :
    obj (obj_), bite (bite_) {};

  bool inside (const Point3D &p) const;
  bool intersect (Intersection &i, const Object &o, real tlim = 0.0) const;
};

class Union : public CSGEntity {
  const Entity &obj, &next;
  bool next_is_union;

 public:
  Union (const Entity &obj_, const Entity &next_) :
    obj (obj_), next (next_), next_is_union (false) {};
  Union (const Entity &obj_, const Union &next_) :
    obj (obj_), next (next_), next_is_union (true) {};

  bool inside (const Point3D &p) const;
  bool intersect (Intersection &i, const Object &o, real tlim = 0.0) const;
};

#endif
