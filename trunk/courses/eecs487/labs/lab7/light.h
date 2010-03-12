

#ifndef __LIGHT_H__
#define __LIGHT_H__

#include "point.h"
#include "color.h"
#include "material.h"

/*! Represents a positional, adirectional, point light-source */

struct Light {

  Pt o; // position
  Color amb, diff, spec; // light components
  double T0, T1, T2; // attenuation parameters
  
  Light(Pt const& o0,
	Color const& a,
	Color const& d,
	Color const& s,
	double const& t0=1,
	double const& t1=0,
	double const& t2=0) :
    o(o0), amb(a), diff(d), spec(s), T0(t0), T1(t1), T2(t2) {}
  
  // Color at point p due to this light, given normal and material
  // At present, performs diffuse calculations only.
  Color pt(Pt const& p, Vec const& n, Material const& mat) const;
};


#endif

