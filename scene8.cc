// Example scene
// Written by Paolo Bonzini, 2005

#include "images.h"
#include "scene.h"

int main (int argc, char **argv)
{
  Light l (10, 10, -30);
  // Material floor (0, 0.8, 0, 0.1);
  Material floor (0, 0.8, 0, 0);
  Material glass (0.05, 0.2, 0.7, 0.2, 0.9, 1.33, 0.8);

  Plane p (0, 1, 0, 0);
  Sphere s (0, 5, -6, 5);

  CheckerTexture floor_texture (colors::red, colors::white, 3);

  Scene scene;
  scene.add_light (l);
  scene.add_object (p, floor, floor_texture);
  scene.add_object (s, glass, MonoTexture::white);

  Point3D location (0, 5, -20);
  Vector3D direction (0, 0, 1);
  scene.render (location, direction, argc, argv, 320, 240);
}
