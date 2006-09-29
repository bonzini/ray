// Example scene
// Written by Paolo Bonzini, 2005

#include "images.h"
#include "scene.h"

int main (int argc, char **argv)
{
  Light l1 (0, 200, -200);

  // Do not let the bottom scene interfere with illumination of the top scene.

  Material diffuse (0.2, 1, 0.3, 0, 0);
  Material plastic (0.2, 0.8, 1, 0, 0);
  Material glass (0.1, 0.3, 1, 0.15, 0.65, 1.33, 0.15);

  CheckerTexture floor_texture (colors::black, colors::blue, 5);

  Plane p (0, 1, 0, 4);

  Sphere s1 (-10, 4, 0, 4);
  Sphere s2 (10, 4, 0, 4);

  Scene scene;
  scene.add_light (l1);
  scene.add_object (p, diffuse, floor_texture);
  scene.add_object (s1, plastic, MonoTexture::yellow);
  scene.add_object (s2, glass, MonoTexture::green);

  // Add a light to simulate indirect illumination
  DirectedLight l2 (10, -1, 10, 0, -1, 1, Color (0, 0.7, 0));
  scene.add_light (l2);

  Point3D location (0, 5, -20);
  Vector3D direction (0, 0, 1);
  scene.render (location, direction, argc, argv, 320, 240);
}
