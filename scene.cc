// Example ray tracing program
// Written by Paolo Bonzini, 2005

#include "scene.h"
#include "rand.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <getopt.h>

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

// Routine per interpretare il parametro -S (--seed).
int
parse_num (const char *c)
{
  std::istringstream iss(c);
  int num;
  iss >> num;

  return (bool) iss ? num : -1;
}

// Stampa l'help.
void
usage(char *progname, int exit_status)
{
  std::cerr << "Usage: " << progname << " [OPTIONS...]\n"
"\n"
" -o, --output-file=NAME   set output file name (- = stdout)\n"
" -f, --file-format=FORMAT set output file format (ppm, png)\n"
" -w, --width=SIZE         set output width (must be power of two)\n"
" -h, --height=SIZE        set output height (must be power of two)\n"
" -S, --seed=NUMBER        set random number seed\n\n";

  exit (exit_status);
}

void
Scene::render (const Ray3D &camera_dir, int argc, char **argv,
	       int default_width, int default_height,
	       char *default_output_file, enum image_file_format output_format,
	       int max_ref) const
{
  char *output_filename = NULL;
  int width = -1;
  int height = -1;
  int seed = std::time(0);

  while (1)
    {
      static struct option long_options[] =
        {
          /* These options set a flag. */
          {"help",    no_argument, 	      0, 1},
          {"file-format", required_argument,  0, 'f'},
          {"output-file",  required_argument, 0, 'o'},
          {"width",   required_argument,      0, 'w'},
          {"height",   required_argument,     0, 'h'},
          {"seed",    required_argument,      0, 'S'},
          {0, 0, 0, 0}
        };
      /* `getopt_long' stores the option index here. */
      int option_index = 0;
      int c;

      c = getopt_long (argc, argv, "f:o:h:s:S:w:",
                       long_options, &option_index);

      /* Detect the end of the options. */
      if (c == -1)
        break;

      switch (c)
        {
	case 0:
	  break;

        case 'o':
          output_filename = optarg;
          break;

        case 'f':
          if (strcmp (optarg, "ppm") == 0)
	    {
	      output_format = OUT_PPM;
  	      if (default_output_file == NULL)
		default_output_file = "ptgen.ppm";
	    }
#ifdef HAVE_LIBPNG
	  else if (strcmp (optarg, "png") == 0)
	    {
	      output_format = OUT_PNG;
  	      if (default_output_file == NULL)
		default_output_file = "ptgen.png";
	    }
#endif
	  else
	    {
	      std::cerr << "Invalid file format" << std::endl;
	      usage (argv[0], 1);
	    }
          break;

        case 'S':
          if ((seed = parse_num (optarg)) == -1)
	    {
	      std::cerr << "Wrong syntax for --seed option" << std::endl;
	      usage (argv[0], 1);
	    }

	  break;

        case 'w':
          if ((width = parse_num (optarg)) == -1)
	    {
	      std::cerr << "Wrong syntax for --width option" << std::endl;
	      usage (argv[0], 1);
	    }

          break;

        case 'h':
          if ((height = parse_num (optarg)) == -1)
	    {
	      std::cerr << "Wrong syntax for --height option" << std::endl;
	      usage (argv[0], 1);
	    }

          break;

        case 1:
	  usage (argv[0], 0);
	  break;

        case '?':
          /* `getopt_long' already printed an error message. */
	  usage (argv[0], 1);

        default:
          abort ();
        }
    }

  if (width == -1)
    {
      if (height == -1)
        height = default_height;
      width = (int) (height * (real) default_width / default_height);
    }
  if (height == -1)
    height = (int) (width * (real) default_height / default_width);

  Image i;
  i.set_size (width, height);
  randf.set_seed (28111979L, seed);

  if (!output_filename)
    output_filename = default_output_file;

  render (camera_dir, i, max_ref);
  if (strcmp(output_filename, "-") == 0)
    i.write (std::cout, output_format);
  else
    i.write (output_filename, output_format);
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
