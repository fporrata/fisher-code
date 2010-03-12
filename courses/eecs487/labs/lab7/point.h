

#ifndef __POINT_H__
#define __POINT_H__

#include <cmath>
#include <cassert>
#include <iostream>
#include <fstream>

#define EPSILON 1E-15

using namespace std;

// square the argument
static inline double sqr(double const& x) { return x*x; }

struct Pt; // forward declaration

struct Vec {
  
  double x, y, z; // components

  // canonical axes
  static Vec const X; 
  static Vec const Y;
  static Vec const Z;

  Vec(double const& x0=0,
      double const& y0=0,
      double const& z0=0) :
    x(x0), y(y0), z(z0) {}
    
  Vec(Pt const& p);

  double length(void) const;

  // unit vector along same direction
  Vec normalized(void) const; 


  // increment
  Vec& operator +=(Vec const& v) {
    x+=v.x; y+=v.y; z+=v.z;
    return *this;
  }

  // decrement
  Vec& operator -=(Vec const& v) {
    x-=v.x; y-=v.y; z-=v.z;
    return *this;
  }

  // self-scaling
  Vec& operator *=(double const& c) {
    x*=c; y*=c; z*=c;
    return *this;
  }

  // self-scaling
  Vec& operator /=(double const& c) {
    x/=c; y/=c; z/=c;
    return *this;
  }
  
};


// vector addition
static inline Vec operator +(Vec const& v1, Vec const& v2)
{
  return Vec(v1.x+v2.x, v1.y+v2.y, v1.z+v2.z);
}

// subtraction
inline Vec operator -(Vec const& v1, Vec const& v2)
{
  return Vec(v1.x-v2.x, v1.y-v2.y, v1.z-v2.z);
}

// negation
inline Vec operator -(Vec const& v)
{
  return Vec(-v.x, -v.y, -v.z);
}

// dot product
inline double dot(Vec const& v1, Vec const& v2)
{
  return v1.x*v2.x + v1.y*v2.y + v1.z*v2.z;
}

// cross product
inline Vec cross(Vec const v1, Vec const& v2)
{
  return Vec(v1.y*v2.z-v1.z*v2.y,
	     v1.z*v2.x-v1.x*v2.z,
	     v1.x*v2.y-v1.y*v2.x);
}

// scaling
inline Vec operator *(Vec const& v, double const& c)
{
  return Vec(v.x*c, v.y*c, v.z*c);
}

// scaling, commutativity
inline Vec operator *(double const& c, Vec const& v)
{
  return v*c;
}

// scaling by inverse, not commutative
inline Vec operator /(Vec const& v, double const& c)
{
  return Vec(v.x/c, v.y/c, v.z/c);
}

// equality test
inline bool operator == (Vec const& v1, Vec const& v2)
{
  return (v1.x==v2.x) && (v1.y==v2.y) && (v1.z==v2.z);
}


// length of this vector: Euclidean norm
inline double Vec::length(void) const
{
  return sqrt(x*x+y*y+z*z);
}

// unit vector in the same direction
inline Vec Vec::normalized(void) const
{
  return *this /(length());
}


struct Pt {

  double x, y, z;
  static Pt const ORIGIN;

  Pt(double const& x0=0,
     double const& y0=0,
     double const& z0=0) :
    x(x0), y(y0), z(z0) {}
  
  double dist(Pt const& p) const{
    return sqrt(sqr(x-p.x)+sqr(y-p.y)+sqr(z-p.z));
  }
  
  Pt operator +(Vec const& v) const{
    return Pt(x+v.x, y+v.y, z+v.z);
  }

  Pt operator -(Vec const& v) const{
    return Pt(x-v.x, y-v.y, z-v.z);
  }

  Vec operator -(Pt const& p) const{
    return Vec(x-p.x, y-p.y, z-p.z);
  }

};


// Are the two points the same?
inline bool operator == (Pt const& p1, Pt const& p2)
{
  return (p1.x==p2.x) && (p1.y==p2.y) && (p1.z==p2.z);
}


// Represent point as position vector
inline Vec::Vec(Pt const& p) :
  x(p.x), y(p.y), z(p.z) {}


/*! Represents a ray passing through point p with direction v */
struct Ray {
  Pt p;
  Vec v;

  Ray(Pt const& p0, Vec const& v0) : p(p0), v(v0) {}
  Pt pt(double const& t) const { 
    Vec const tempv = t*v;
    return p+tempv; 
  }  
	
	friend ostream &operator<<(ostream &stream, Ray ray);
};


#endif

