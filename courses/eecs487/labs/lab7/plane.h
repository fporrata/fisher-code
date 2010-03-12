#ifndef __PLANE_H__
#define __PLANE_H__

#include <cmath>
#include "point.h"
#include "material.h"

struct Plane {
	
	Vec n; // normal vector, not necessarily normalized
	Pt q;  // plane passes through  
	Material mat; 
	
	Plane(Vec const& normal, Pt const& q0, Material const& m) :
    n(normal), q(q0), mat(m) {}
	
	bool contains(Pt const& p) const;
	
	// If the ray lies in the plane or interesects it at a unique
	// point then they are considered to intersect.
	bool is_intersecting(Ray const& ray) const;
	
	// The point of intersection with given ray, in terms of 't'
	// parameter for ray. If ray is parallel and not in the plane
	// then intersection is theoretically at infinity. For this case,
	// HUGE_VAL (largest representable double precision number) is
	// returned.
	double intersect(Ray const& ray) const;
	
};

#endif
