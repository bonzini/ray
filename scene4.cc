// Example scene
// Written by Paolo Bonzini, 2005

#include "images.h"
#include "scene.h"

int main (void)
{
  Point3D location (0, 5, -20);
  Vector3D direction (0, 0, 1);
  Light l (30, 30, -30);

  Material diffuse (0, 1, 0, 0, 0);
  Material shiny (0.2, 0.5, 1, 0);

  Plane p (0, 1, 0, 0);

  Sphere s1 (0, 5.5, -5, 5);
  Plane bottom (0, -1, 0, 6);
  Difference ufo (s1, bottom);

  Scene scene;
  scene.add_light (l);
  scene.add_object (p, diffuse, MonoTexture::lightBlue);
  scene.add_object (ufo, shiny, MonoTexture::white);

  Image image;
  image.set_size (320, 240);

  scene.render (location, direction, image);
  image.write ("scene4.png", OUT_PNG);
}
