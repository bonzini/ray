// Example scene
// Written by Paolo Bonzini, 2005

#include "images.h"
#include "scene.h"

int main (void)
{
  Light l (0, 30, -30);

  Material ambient (0.8, 0.2, 0, 0, 0);
  Material diffuse (0, 0.8, 0, 0, 0);
  Material glass (0, 0.3, 1, 0.15, 0.65, 1.33, 0.15);

  Plane backdrop (0, 0, 1, -100);
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

  MonoTexture sky (0.2, 0.4, 0.7);

  Scene scene;
  scene.add_light (l);
  scene.add_object (backdrop, ambient, sky);
  scene.add_object (p, diffuse, MonoTexture::red);
  scene.add_object (s1, diffuse, MonoTexture::white);
  scene.add_object (s2, glass, MonoTexture::red);
  scene.add_object (s3, glass, MonoTexture::green);
  scene.add_object (s4, glass, MonoTexture::yellow);

  Image image;
  image.set_size (320, 240);

  Point3D location (0, 5, -20);
  Vector3D direction (0, 0, 1);
  scene.render (location, direction, image);
  image.write ("scene5.png", OUT_PNG);
}
