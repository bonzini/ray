#include "images.h"
#include "scene.h"

int main (void)
{
  Light l (Point3D (30, 30, -30));
  Material floor;
  Material plastic (0.0, 1.0, 1.0);

  MonoTexture blue (Color (0.0, 0.0, 0.5));
  MonoTexture red (Color (1.0, 0.0, 0.0));

  Plane p (0.0, 1.0, 0.0, 0.0);
  Sphere s (0.0, 3.0, 0.0, 3.0);

  Scene scene;
  scene.add_light (l);
  scene.add_object (p, floor, blue);
  scene.add_object (s, floor, red);

  Image image;
  image.set_size (320, 240);

  Point3D location (0, 5, -20);
  Vector3D direction (0, 0, 1);
  scene.render (location, direction, image);
  image.write ("scene1.png", OUT_PNG);
}
