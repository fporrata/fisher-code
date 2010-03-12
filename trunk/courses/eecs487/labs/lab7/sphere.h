
#ifndef __SPHERE_H__
#define __SPHERE_H__

#include "point.h"


struct Sphere {

  Pt o; // center
  double r; // radius

  Sphere(double const& x0,
	 double const& y0,
	 double const& z0,
	 double const& r0) :
    o(x0, y0, z0), r(r0) {}

  // Does the sphere contain the given point?
  bool contains(Pt const& p) const;
  
  // Does the given ray intersect the sphere?
  bool is_intersecting(Ray const& ray) const;
  
  // point of intersection of ray and sphere
  // in terms of the 't' parameter of the ray.
  // Negative value indicates that point of intersection
  // is in the direction opposite to that of the ray.
  // If they don't intersect, NaN (Not-a-Number)
  // is returned. This will signal floating point errors
  // if used subsequently so use the is_intersecting()
  // method above as a test before calling this.
  double intersect(Ray const& ray) const;
  

  // unit normal to sphere through the given point
  Vec unit_normal(Pt const& p) const {
    return (p-o).normalized();
  }
  
};

#endif

