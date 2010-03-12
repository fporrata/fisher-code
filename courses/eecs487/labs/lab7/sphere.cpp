#include <cmath>
#include <cassert>

#include "sphere.h"

bool Sphere::contains(Pt const& p) const
{
  return (p-o).length() <= r;
}


bool Sphere::is_intersecting(Ray const& ray) const
{
  Vec dp(ray.p - o);
  if(dot(dp, ray.v) > 0) return false;

  double const C = dot(dp,dp)-r*r;
  double const B = 2.0 * dot(ray.v, dp);
  double const A = dot(ray.v, ray.v);

  // solve quadratic
  double const DISC = B*B - 4*A*C;
  // no solution: return center because that can't be a point of intersection.
  if(DISC < 0) 
    return false;

  // return the root with the smaller t
  else {
    double const t1 = (-B-sqrt(DISC))/(2.0*A);
    double const t2 = (-B+sqrt(DISC))/(2.0*A);
    return (t1<0 && t2<0) ? false : true;
  }
}


double Sphere::intersect(Ray const& ray) const
{
    // ** TODO **

    // If the ray intersects the sphere, return the ray parameter
    // 't' for which this occurs. If it doesn't intersect, return
    // NAN, which is #defined in a file included by cmath (math.h)

    return NAN; 
}


