

#ifndef __MATERIAL_H__
#define __MATERIAL_H__


#include "color.h"


/*! Represents a material, like in OpenGL */

struct Material {

  Color amb, diff, spec;
  double s; // shininess

  Material(Color const& a,
	   Color const& d,
	   Color const& sp,
	   double s0=0) :
    amb(a), diff(d), spec(sp), s(s0) {}
};


#endif

