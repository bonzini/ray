#include "images.h"
#include "scene.h"

int main (void)
{
  Point3D location (0, 5, -20);
  Vector3D direction (0, 0, 1);
  Light l (location, colors::white, false);

  Material diffuse (0, 1, 0, 0);
  Material shiny (0, 1, 1, 0.5);

  Plane p (0, 1, 0, 0);

  Vector3D v1 (0, 0, sqrt (1/3.0));
  Vector3D v2 (0.5, 0, -sqrt (1/27.0));
  Vector3D v3 (-0.5, 0, -sqrt (1/27.0));
  Vector3D v4 (0, sqrt (2/3.0), 0);

  const real side = 10;
  Point3D higher (0, side / 2, 0);

  Sphere s1 (higher + v1 * side, side / 2);
  Sphere s2 (higher + v2 * side, side / 2);
  Sphere s3 (higher + v3 * side, side / 2);
  Sphere s4 (higher + v4 * side, side / 2);

  Scene scene;
  scene.add_light (l);
  scene.add_object (p, diffuse, MonoTexture::red);
  scene.add_object (s1, shiny, MonoTexture::green);
  scene.add_object (s2, shiny, MonoTexture::green);
  scene.add_object (s3, shiny, MonoTexture::green);
  scene.add_object (s4, shiny, MonoTexture::yellow);

  Image image;
  image.set_size (320, 240);

  scene.render (location, direction, image);
  image.write ("scene2.png", OUT_PNG);
}
