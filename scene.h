// Example ray tracing program
// Written by Paolo Bonzini, 2005

#ifndef PTGEN_SCENE_H
#define PTGEN_SCENE_H

#include "config.h"
#include "geom.h"
#include "light.h"
#include "texture.h"
#include "images.h"

#ifdef HAVE_SLIST
#include <ext/slist>
#else
#include <list>
#endif

struct Material {
  real ambient;
  real diffuse;

  real specular;
  int reflectivity;

  real reflective;
  int max_ref;

  Material (real ambient_ = 0.0, real diffuse_ = 1.0, real specular_ = 0.0,
	    real reflective_ = 0.0, int reflectivity_ = 20, int max_ref_ = 5) :
    ambient (ambient_), diffuse (diffuse_), specular (specular_),
    reflectivity (reflectivity_), reflective (reflective_),
    max_ref (reflective > 0.0 ? max_ref_ : 0) {}
};

struct Object {
  const Entity &e;
  const Material &m;
  const Texture &t;
  bool have_shadows;

  Object (const Entity &e_, const Material &m_, const Texture &t_,
    bool shadows = true) : e (e_), m (m_), t (t_), have_shadows (shadows) {}

  bool intersect (Intersection &i) const { return e.intersect (i, *this); }
};

class Scene {
#ifdef HAVE_SLIST
  template <typename T> struct slist : std::slist<T> { };
#else
  template <typename T> struct slist : std::list<T> { };
#endif

  slist <const AbstractLight *> lights;
  slist <Object> objects;

  typedef slist<const AbstractLight *>::const_iterator light_iterator;
  typedef slist<Object>::const_iterator object_iterator;

  bool compute_intersection (Intersection &i) const;
  bool find_an_intersection (NormRay3D &r) const;
  Color trace (const Ray3D &r, int max_ref, real strength = 1.0) const;

 public:
  real ambient;

  Scene (real ambient_ = 0.0) : lights (), objects (), ambient (ambient_) {}

  void add_light (const AbstractLight &l) {
    lights.push_back (&l);
  }

  void add_object (const Entity &e, const Material &m, const Texture &t) {
    objects.push_back (Object (e, m, t));
  }

  void render (const Point3D &camera, Image &m, int max_ref = 5) const {
    Point3D dest (0, 0, 0);
    Ray3D camera_dir (camera, dest);
    render (camera_dir, m, max_ref);
  }

  void render (const Point3D &camera, const Vector3D &dir, Image &m,
	       int max_ref = 5) const {
    Ray3D camera_dir (camera, dir);
    render (camera_dir, m, max_ref);
  }

  void render (const Ray3D &camera, Image &m, int max_ref = 5) const;
};

#endif
