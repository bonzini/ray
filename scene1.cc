// Example scene
// Written by Paolo Bonzini, 2005

#include "images.h"
#include "scene.h"

int main (int argc, char **argv)
{
  Light l (30, 30, -30);
  Material floor (0, 1, 0, 0);
  Material plastic (0, 1, 1, 0);

  Plane p (0, 1, 0, 0);
  Sphere s (0, 3, 0, 3);

  Scene scene (0.3);
  scene.add_light (l);
  scene.add_object (p, floor, MonoTexture::lightBlue);
  scene.add_object (s, plastic, MonoTexture::red);

  Point3D location (0, 5, -20);
  Vector3D direction (0, 0, 1);
  scene.render (location, direction, argc, argv, 320, 240);
}
