// Example ray tracing program
// Written by Paolo Bonzini, 2005

#include "scene.h"

void Scene::render (const Ray3D &camera, Image &m, int max_ref) const
{
  // Rotate by 90 degrees around the Y axis
  Vector3D x_vec_unit (camera.dir.z, camera.dir.y, -camera.dir.x);

  // Rotate by -90 degrees around the X axis (to compensate for
  // the wrong orientation of the Y axis on the screen)
  Vector3D y_vec_unit (-camera.dir.x, -camera.dir.z, camera.dir.y);

  int w = m.get_width ();
  int h = m.get_height ();

  real vec_step;
  Vector3D leftmost_dir;

  if (w < h)
    {
      vec_step = 2.0 / (w - 1);
      leftmost_dir = camera.dir - x_vec_unit - y_vec_unit * h / w;
    }
  else
    {
      vec_step = 2.0 / (h - 1);
      leftmost_dir = camera.dir - x_vec_unit * w / h - y_vec_unit;
    }

  for (int i = 0; i < h; i++, leftmost_dir += y_vec_unit * vec_step)
    {
      Vector3D dir = leftmost_dir;
      for (int j = 0; j < w; j++, dir += x_vec_unit * vec_step)
	m.set_pixel (j, i, trace (Ray3D (camera.source, dir), max_ref, 1.0));
      std::cerr << '.';
    }
  std::cerr << std::endl;
}

bool Scene::compute_intersection (Intersection &i) const
{
  bool had_intersection = false;
  for (object_iterator oi = objects.begin (); oi != objects.end (); oi++)
    {
      const Object &o = *oi;
      had_intersection |= o.intersect (i);
    }

  return had_intersection;
}

bool Scene::find_an_intersection (NormRay3D &r) const
{
  Intersection i (r);
  for (object_iterator oi = objects.begin (); oi != objects.end (); oi++)
    {
      const Object &o = *oi;
      if (o.intersect (i))
	return true;
    }

  return false;
}

Color Scene::trace (const Ray3D &ray, int max_ref, real ior, Color strength,
		    real absorbance) const
{
  Intersection i (ray);
  Color c (0.0, 0.0, 0.0);
  if (!compute_intersection (i))
    return c;

  Point3D p = i.r (i.t);

  if (absorbance != 0.0)
    strength *= exp (-i.t * absorbance);

  bool have_shadows = i.object->have_shadows;
  const Entity &e = *(i.entity);
  const Material &m = i.object->m;
  const Texture &t = i.object->t;

  UnitVector3D normal = i.from_inside ? -e.get_normal (i) : e.get_normal (i);
  Color obj_color = t.get_color (p, e) * strength;

  c += (ambient + m.ambient) * obj_color;

  for (light_iterator li = lights.begin (); li != lights.end (); li++)
    {
      const AbstractLight &l = **li;
      UnitVector3D to_light = (l.get_pos () - p).normalize ();

      if (have_shadows && l.cast_shadows)
	{
	  NormRay3D shadow_ray (p, to_light, 0.0001);
	  if (find_an_intersection (shadow_ray))
	    continue;
	}

      // Diffuse light
      if (m.diffuse > 0)
	{
	  real cosine = to_light * normal;
	  if (cosine > 0.0)
	    c += (m.diffuse - ambient) * cosine * obj_color;
	}
      
      // Specular highlights
      if (m.specular > 0)
	{
	  Vector3D specular = 2 * (to_light * normal) * normal - to_light;
	  real cosine = -ray.dir * specular.normalize ();
          if (cosine > 0.0)
	    {
	      cosine = std::pow (cosine, m.reflectivity);
              Color light_color = strength * l.get_color (p);
	      c += m.specular * cosine * light_color;
	    }
	}
    }

  // reflections...
  if (m.max_ref < max_ref)
    max_ref = m.max_ref;

  if (m.reflective > 0 && max_ref > 0)
    {
      Vector3D reflected = ray.dir - 2 * (ray.dir * normal) * normal;
      Ray3D reflected_ray = Ray3D (p, reflected, 0.0001);
      c += trace (reflected_ray, max_ref - 1, ior, strength * m.reflective);
    }

  // refractions...
  if (m.refractive > 0 && max_ref > 0)
    {
      // Relative index of refraction
      real n = ior / m.ior;
      real cosI = -normal * ray.dir;
      real sinI2 = 1.0 - cosI * cosI;
      real sinT2 = (n * n) * sinI2;
      real cosT2 = 1.0 - sinT2;
      if (cosT2 > 0.0)
	{
	  real cosT = sqrt (cosT2);
	  Vector3D refracted = n * ray.dir + (n * cosI - cosT) * normal;
          Ray3D refracted_ray = Ray3D (p, refracted, 0.01);
          c += trace (refracted_ray, max_ref - 1, m.ior,
		      strength * m.refractive,
		      i.from_inside ? absorbance + m.absorbance
				    : absorbance - m.absorbance);
	}
    }

  return c;
}
