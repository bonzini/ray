// Example ray tracing program
// Vector math classes
// Written by Paolo Bonzini, 2005

#ifndef RAY_V3D_H
#define RAY_V3D_H

#include "config.h"
#include <cmath>

template <typename T> class P3D;
template <typename T> class V3D;
template <typename T> class NV3D;
template <typename T> class R3D;
template <typename T> class NR3D;

// A 3D point.  Possible operations:
//    Point = Point + Vector (and reverse, see V3D)
//    Vector = Point - Point
template <typename T>
struct P3D {
  T x, y, z;
  P3D<T> () : x (0), y (0), z (0) {}
  P3D<T> (const V3D<T> &v) : x (v.x), y (v.y), z (v.z) {}
  P3D<T> (T x_, T y_, T z_) : x (x_), y (y_), z (z_) {}

  P3D<T> operator + (const V3D<T> &p) const { return P3D<T> (*this) += p; }
  V3D<T> operator - (const P3D<T> &p) const { return V3D<T> (*this, p); }
  T operator * (const V3D<T> &v) const { return x * v.x + y * v.y + z * v.z; }

  P3D<T> &operator += (const V3D<T> &v) {
    x += v.x, y += v.y, z += v.z;
    return *this;
  }
};

// A 3D vector.  Possible operations:
//    Point = Vector + Point (and reverse, see P3D)
//    Vector = Vector + Vector
//    Vector = Vector - Vector
//    Vector = Vector * double
//    Vector = Vector / double
//    double = Vector * Vector (scalar product)
//    Vector = Vector ^ Vector (vector product)
//    double = Vector.length ()
//    Vector = Vector.normalize ()
//
// Normalized vectors are actually represented by a separate class,
// as a small optimization.  Conversion operators ensure that NV3D
// and V3D interoperate.
//
template <typename T>
struct V3D {
  T x, y, z;
  V3D<T> () :
    x (0), y (0), z (0) {}
  V3D<T> (T x_, T y_, T z_) :
    x (x_), y (y_), z (z_) {}
  V3D<T> (const P3D<T> &a, const P3D<T> &b) :
    x (a.x - b.x), y (a.y - b.y), z (a.z - b.z) {}
  V3D<T> (const NV3D<T> &a) :
    x (a.x), y (a.y), z (a.z) {}

  explicit V3D<T> (const R3D<T> &a) :
    x (a.dir.x), y (a.dir.y), z (a.dir.z) {}
  explicit V3D<T> (const P3D<T> &a) :
    x (a.x), y (a.y), z (a.z) {}

  P3D<T> operator + (const P3D<T> &v) const { return P3D<T> (v) += *this; }
  V3D<T> operator + (const V3D<T> &v) const { return V3D<T> (*this) += v; }
  V3D<T> operator - (const V3D<T> &v) const { return V3D<T> (*this) -= v; }
  V3D<T> operator ^ (const V3D<T> &v) const { return V3D<T> (*this) ^= v; }
  V3D<T> operator * (T f) const { return V3D<T> (*this) *= f; }
  V3D<T> operator / (T f) const { return V3D<T> (*this) /= f; }
  V3D<T> operator - () const { return V3D<T> (-x, -y, -z); }

  V3D<T> &operator += (const V3D<T> &v) {
    x += v.x, y += v.y, z += v.z;
    return *this;
  }
  V3D<T> &operator -= (const V3D<T> &v) {
    x -= v.x, y -= v.y, z -= v.z;
    return *this;
  }
  V3D<T> &operator *= (T f) {
    x *= f, y *= f, z *= f;
    return *this;
  }
  V3D<T> &operator /= (T f) {
    x /= f, y /= f, z /= f;
    return *this;
  }

  V3D<T> &operator ^= (const V3D<T> &v) {
    T x1 = x, x2 = v.x;
    T y1 = y, y2 = v.y;
    T z1 = z, z2 = v.z;
    x = y1 * z2 - y2 * z1;
    y = x2 * z1 - x1 * z2;
    z = x1 * y2 - x2 * y1;
    return *this;
  }

  T length () const {
    return sqrt (x * x + y * y + z * z);
  }

  NV3D<T> normalize () const {
    T len = length ();
    return NV3D<T> (x / len, y / len, z / len);
  }

  T operator * (const V3D<T> &v) const { return x * v.x + y * v.y + z * v.z; }
  T operator * (const P3D<T> &v) const { return x * v.x + y * v.y + z * v.z; }

  T cosine (const V3D<T> &v) const {
    return (*this * v) / (this->length () * v.length ());
  }
};


// A normalized 3D vector.  This is only a small optimization, conversion
// operators ensure that NV3D and V3D interoperate.
//
template <typename T>
struct NV3D {
  T x, y, z;
  NV3D<T> () :
    x (0), y (0), z (0) {}
  NV3D<T> (T x_, T y_, T z_) :
    x (x_), y (y_), z (z_) {}

  explicit NV3D<T> (const NR3D<T> &a) :
    x (a.dir.x), y (a.dir.y), z (a.dir.z) {}
  explicit NV3D<T> (const V3D<T> &a) :
    x (a.x), y (a.y), z (a.z) {}

  V3D<T> operator + (const V3D<T> &v) const { return V3D<T> (*this) += v; }
  V3D<T> operator - (const V3D<T> &v) const { return V3D<T> (*this) -= v; }
  V3D<T> operator ^ (const V3D<T> &v) const { return V3D<T> (*this) ^= v; }
  V3D<T> operator * (T f) const { return V3D<T> (*this) *= f; }
  V3D<T> operator / (T f) const { return V3D<T> (*this) /= f; }

  NV3D<T> operator - () const { return NV3D<T> (-x, -y, -z); }

  T length () const { return 1.0; }
  NV3D<T> normalize () { return NV3D<T> (*this); }
  const NV3D<T> &normalize () const { return *this; }

  T operator * (const V3D<T> &v) const { return x * v.x + y * v.y + z * v.z; }
  T cosine (const NV3D<T> &v) const { return (*this * v); }
};

// A 3D ray.  Possible operations:
//    Ray = Point + t * Vector
//    Ray = Point1 + t * (Point2 - Point1)
//    Point = Ray (t)
// 
// Again, a normalized ray is a separate class for optimization.
// A ray can be cast (explicitly) to a vector.
template <typename T>
struct R3D {
  P3D<T> source;
  V3D<T> dir;
  R3D<T> () : source (), dir () {}
  R3D<T> (const P3D<T> &a, const P3D<T> &b) : source (a), dir (b - a) {}
  R3D<T> (const P3D<T> &a, const V3D<T> &b, T t = 0.0) : source (a + b * t), dir (b) {}
  R3D<T> (const NR3D<T> &a) : source (a.source), dir (a.dir) {}

  P3D<T> operator () (T time) const { return source + dir * time; }
  R3D<T> operator - () const { return R3D<T> (source, -dir); }

  T length () const {return dir.length (); }
  NR3D<T> normalize () const { return NR3D<T> (source, dir.normalize ()); }
};

// Again, a normalized ray is a separate class for optimization.
// A ray can be cast (explicitly) to a vector.
template <typename T>
struct NR3D {
  P3D<T> source;
  NV3D<T> dir;
  NR3D<T> () : source (), dir () {}
  NR3D<T> (const P3D<T> &a, const P3D<T> &b) :
    source (a), dir ((b - a).normalize ()) {}
  NR3D<T> (const P3D<T> &a, const V3D<T> &b, T t = 0.0) :
    source (a + b * t), dir (b.normalize ()) {}
  NR3D<T> (const P3D<T> &a, const NV3D<T> &b, T t = 0.0) :
    source (a + b * t), dir (b) {}
  NR3D<T> (const NR3D<T> &a, T t = 0.0) :
    source (a.source + a.dir * t), dir (a.dir) {}

  P3D<T> operator () (T time) const { return source + dir * time; }
  NR3D<T> operator - () const { return NR3D<T> (source, -dir); }

  T length () const { return dir.length (); }
  NR3D<T> normalize () { return NR3D<T> (*this); }
  const NR3D<T> &normalize () const { return *this; }
};

template <typename T>
struct Col {
  T r, g, b;

  Col<T> () : r (0), g (0), b (0) {}
  Col<T> (T r_, T g_, T b_) : r (r_), g (g_), b (b_) {}
  Col<T> operator + (const Col<T> &v) const { return Col<T> (*this) += v; }
  Col<T> operator - (const Col<T> &v) const { return Col<T> (*this) -= v; }
  Col<T> operator * (const Col<T> &v) const { return Col<T> (*this) *= v; }
  Col<T> operator * (T f) const { return Col<T> (*this) *= f; }
  Col<T> operator / (T f) const { return Col<T> (*this) /= f; }

  Col<T> &operator += (const Col<T> &v) {
    r = (r + v.r) > 1.0 ? 1.0 : r + v.r;
    g = (g + v.g) > 1.0 ? 1.0 : g + v.g;
    b = (b + v.b) > 1.0 ? 1.0 : b + v.b;
    return *this;
  }
  Col<T> &operator -= (const Col<T> &v) {
    r = (r - v.r) < 0.0 ? 0.0 : r - v.r;
    g = (g - v.g) < 0.0 ? 0.0 : g - v.g;
    b = (b - v.b) < 0.0 ? 0.0 : b - v.b;
    return *this;
  }
  Col<T> &operator *= (const Col<T> &v) {
    r *= v.r;
    g *= v.g;
    b *= v.b;
    return *this;
  }
  Col<T> &operator *= (T f) {
    r *= f; if (r < 0.0) r = 0.0; if (r > 1.0) r = 1.0;
    g *= f; if (g < 0.0) g = 0.0; if (g > 1.0) g = 1.0;
    b *= f; if (b < 0.0) b = 0.0; if (b > 1.0) b = 1.0;
    return *this;
  }
  Col<T> &operator /= (T f) {
    r /= f; if (r < 0.0) r = 0.0; if (r > 1.0) r = 1.0;
    g /= f; if (g < 0.0) g = 0.0; if (g > 1.0) g = 1.0;
    b /= f; if (b < 0.0) b = 0.0; if (b > 1.0) b = 1.0;
    return *this;
  }
};

template <class T>
V3D<T> operator * (T f, const V3D<T> &v) {
  return V3D<T> (v.x * f, v.y * f, v.z * f);
}

template <class T>
V3D<T> operator * (T f, const NV3D<T> &v) {
  return V3D<T> (v.x * f, v.y * f, v.z * f);
}

template <class T>
Col<T> operator * (T f, const Col<T> &c) {
  T r = c.r * f; if (r < 0.0) r = 0.0; if (r > 1.0) r = 1.0;
  T g = c.g * f; if (g < 0.0) g = 0.0; if (g > 1.0) g = 1.0;
  T b = c.b * f; if (b < 0.0) b = 0.0; if (b > 1.0) b = 1.0;
  return Col <T> (r, g, b);
}

#ifdef USE_DOUBLE
typedef double real;
#else
typedef float real;
#endif

typedef P3D<real> Point3D;
typedef V3D<real> Vector3D;
typedef NV3D<real> UnitVector3D;
typedef R3D<real> Ray3D;
typedef NR3D<real> NormRay3D;
typedef Col<real> Color;

#endif
