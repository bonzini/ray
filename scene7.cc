// Example scene
// Based on shell.c (C)1994 by C. Giustozzi MCmicrocomputer 139 (April 1994)
// Written by Paolo Bonzini 2005

#include "images.h"
#include "scene.h"

const real k = 8.0;
const real v = 0.10;
const real beta = -2.0;
const real gam = 0.9;
const int N = 250;

Sphere shell (int i)
{
  real theta = M_PI * 12.0 * (i + 1.5 - N) / (N - 1.0);
  real rho = k * exp (v * theta);
  real x = rho * sin (theta);
  real z = rho * cos (theta);
  real y = rho * beta;
  real r = rho / gam;
  return Sphere (x, y, z, r);
}

int main (int argc, char **argv)
{
  Light l (30, 30, -30);
  Material plastic (0, 1, 1, 0);
  MonoTexture coral (0.8, 0.4, 0);
  Scene scene (0.3);

  scene.add_light (l);

  Sphere s[N];
  for (int i = 0; i < N; i++)
    {
      s[i] = shell (i);
      scene.add_object (s[i], plastic, coral);
    }

  Point3D location (0, -7, -20);
  Vector3D direction (0, -0.1, 1);
  scene.render (location, direction, argc, argv, 320, 240);
}
