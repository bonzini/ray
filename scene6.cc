#include "images.h"
#include "scene.h"

int main (void)
{
  Light l (30, 30, -30);
  Material floor (0, 1, 0.6, 0.6);
  Material plastic (0, 1, 1, 0);

  Plane p (0, 1, 0, 0);
  Sphere s (0, 3, 0, 3);

  CheckerTexture floor_texture (colors::black, colors::blue, 5);

  Scene scene (0.3);
  scene.add_light (l);
  scene.add_object (p, floor, floor_texture);
  scene.add_object (s, plastic, MonoTexture::red);

  Image image;
  image.set_size (320, 240);

  Point3D location (0, 5, -20);
  Vector3D direction (0, 0, 1);
  scene.render (location, direction, image);
  image.write ("scene6.png", OUT_PNG);
}
