// Example scene
// Written by Paolo Bonzini, 2005

#include "images.h"
#include "scene.h"

int main (void)
{
  Point3D location (0, 5, -20);
  Vector3D direction (0, 0, 1);
  Light l (location, colors::white, false);

  Material diffuse (0, 1, 0, 0, 0);
  Material shiny (0, 1, 1, 0.5);
  Material glass (0, 0, 1, 0.1, 0.8, 1.33, 0.8);

  Plane p (0, 1, 0, 0);

  Sphere s1 (0, 5, 0, 5);
  Sphere s2 (-7, 5, 5, 5);
  Sphere s3 (7, 5, 5, 5);
  Sphere s4 (0, 0, 50, 40);

  Scene scene;
  scene.add_light (l);
  scene.add_object (p, diffuse, MonoTexture::red);
  scene.add_object (s1, glass, MonoTexture::white);
  scene.add_object (s2, shiny, MonoTexture::green);
  scene.add_object (s3, shiny, MonoTexture::green);
  scene.add_object (s4, shiny, MonoTexture::yellow);

  Image image;
  image.set_size (320, 240);

  scene.render (location, direction, image);
  image.write ("scene3.png", OUT_PNG);
}
