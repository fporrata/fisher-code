#include "plane.h"

bool Plane::contains(Pt const& p) const {
    return fabs(dot(n.normalized(), (p-q).normalized()))<1.0e-8;
}

// If the ray lies in the plane or interesects it at a unique
// point then they are considered to intersect.
bool Plane::is_intersecting(Ray const& ray) const {
    return contains(ray.p) ?
	true :
	// Is the dot product zero within some numerical tolerance?
	(fabs(dot(n.normalized(), ray.v.normalized())) > 1.0e-8);
}

// The point of intersection with given ray, in terms of 't'
// parameter for ray. If ray is parallel and not in the plane
// then intersection is theoretically at infinity. For this case,
// HUGE_VAL (largest representable double precision number) is
// returned.
double Plane::intersect(Ray const& ray) const {
    
    // ** TODO **
	
	if (is_intersecting(ray)) {
			Vec norm = n.normalized();
			return dot(q - ray.p, norm) / dot(ray.v, norm);
	} else
			return HUGE_VAL;
}

Plane & Plane::operator=(const Plane & p)
{
		n = p.n;
		q = p.q;
		mat = p.mat;

		return *this;
}
