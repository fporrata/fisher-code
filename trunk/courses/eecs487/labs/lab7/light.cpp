
#include "light.h"

static inline double max(double const& a, double const& b) {
  return (a>b) ? a : b;
}


Color Light::pt(Pt const& p, Vec const& n, Material const& mat) const
{
  double const d = p.dist(o);
  double const att = 1.0/(T0+T1*d+T2*d*d);
  Vec const L = o-p;
  return (att*max(dot(L,n),0)) * (diff * mat.diff);
}

