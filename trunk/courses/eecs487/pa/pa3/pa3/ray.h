#ifndef _RAY_H
#define _RAY_H

#include "xvec.h"
#include "material.h"

/// the ray structure
/// the ray's parametric equation is 
/// r(t) = origin + t*dir, for t>0
struct ray_t {
  ray_t(const XVec3f& _origin, const XVec3f& _dir)
    : m_origin(_origin), m_dir(_dir) 
  {
  }
  XVec3f at(float t) const {
    return m_origin + t*m_dir;
  }
  XVec3f m_origin;
  XVec3f m_dir;
};

/// the hitinformation structure
struct hitinfo_t {
  XVec3f m_indir;   // incoming ray direction, useful for computing reflection rays
  XVec3f m_pos;     // position of the intersection
  XVec3f m_normal;  // surface normal at the intersection point
  float m_t;       // ray parameter value at the intersection point
  MaterialT m_mat; // the material colors at the intersection point
};

#endif
