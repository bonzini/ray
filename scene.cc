// Example ray tracing program
// Written by Paolo Bonzini, 2005

#include "scene.h"

void Scene::render (const Ray3D &camera, Image &m, int max_ref) const
{
  Vector3D x_vec_unit (camera.dir.z, camera.dir.y, -camera.dir.x);
  Vector3D y_vec_unit (camera.dir.x, camera.dir.z, -camera.dir.y);

  int w = m.get_width ();
  int h = m.get_height ();

  Vector3D x_vec_step = x_vec_unit * 2 / w;
  Vector3D y_vec_step = y_vec_unit * 2 / h;

  Point3D x_orig = x_vec_unit * (w / 2 + 0.5) + y_vec_unit * (h / 2 + 0.5);

  for (int i = 0; i < h; i++, x_orig += y_vec_step)
    {
      Point3D dest = x_orig;
      for (int j = 0; j < w; j++, dest += x_vec_step)
	m.set_pixel (j, i,
		     ambient + trace (Ray3D (camera.source, dest), max_ref));
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

Color Scene::trace (const Ray3D &ray, int max_ref, real strength) const
{
  Intersection i (ray);
  Color c (0.0, 0.0, 0.0);
  if (!compute_intersection (i))
    return c;

  Point3D p = i.r (i.t);

  UnitVector3D normal = i.entity->get_normal (p).normalize ();
  bool have_shadows = i.object->cast_shadows;

  const Material &m = i.object->m;
  const Texture &t = i.object->t;
  Color obj_color = t.get_color (p) * strength;
  c += m.ambient * obj_color;

  for (light_iterator li = lights.begin (); li != lights.end (); li++)
    {
      const AbstractLight &l = **li;
      Vector3D to_light = l.get_pos () - p;

      if (have_shadows && l.cast_shadows)
	{
	  Intersection i1 (i, to_light, 0.01);
	  if (compute_intersection (i1))
	    continue;
	}

      to_light = to_light.normalize ();

      // Diffuse light
      {
        real cosine = to_light.normalize () * normal;
        if (cosine < 0.0)
	  cosine = 0.0;

        c += m.diffuse * cosine * obj_color;
      }
      
      // Specular highlights
      if (m.specular > 0)
	{
	  Vector3D specular = 2 * (to_light * normal) * normal - to_light;
	  real cosine = -to_light * specular.normalize ();
          if (cosine < 0.0)
	    cosine = 0.0;
	  else
	    cosine = std::pow (cosine, m.reflectivity);

          Color light_color = l.get_color (p) * strength;
	  c += m.specular * cosine * light_color;
	}
    }

  // reflections...
  if (m.max_ref < max_ref)
    max_ref = m.max_ref;

  if (max_ref > 0)
    {
      Color ref_color;
      Vector3D reflected = 2 * (ray.dir * normal) * normal - ray.dir ;
      Ray3D reflected_ray = Ray3D (p, reflected, 0.01);
      c += trace (reflected_ray, max_ref - 1, strength * m.reflective);
    }

  return c;
}
