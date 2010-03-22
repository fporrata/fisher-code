#ifndef _TRIANGLE_H
#define _TRIANGLE_H

#include "gel.h"

#include <iostream>

/// a triangle class
class TriangleT: public IGel {
public:
  TriangleT(const XVec3f& _p0, const XVec3f& _p1, const XVec3f& _p2)
  {
    m_pts[0] = _p0;
    m_pts[1] = _p1;
    m_pts[2] = _p2;
    m_normal = (_p1-_p0).cross(_p2-_p0);
    m_normal.normalize();
  }

  virtual ~TriangleT() {
  }

  static XVec3f _compute_normal(const XVec3f& pt0, const XVec3f& pt1, const XVec3f& pt2) {
    XVec3f normal = (pt1-pt0).cross(pt2-pt0);
    normal.normalize();
    return normal;
  }

  /// Intersection test
  /// Note that the triangle's coordinates are transformed 
  /// when it is read, hence no transform needs to be handled here
  virtual bool Intersect(const ray_t& ray, hitinfo_t& hit) const {
    XVec3f d = ray.m_dir;
    
    float t, u, v;
    if(!_intersect(m_pts[0], m_pts[1], m_pts[2], ray, t, u, v))
      return false;

    hit.m_t = t;
    hit.m_pos = ray.m_origin + t*d;
    hit.m_normal = m_normal;
    hit.m_mat = m_mat;

    return true;
  }

  /// low-level interesection testing. this is also used in the mesh 
  /// intersection testing. Returns barycentric coordinates (u,v) such
  /// that pt0 corresponds to (u=0,v=0), pt1 is at (u=1,v=0), pt2 is at (u=0,v=1)
  /// 
  static bool _intersect(const XVec3f& pt0, const XVec3f& pt1, const XVec3f& pt2, 
                         const ray_t& ray, float& t, float& u, float& v) {
    XVec3f d = ray.m_dir;
    d.normalize();

    XVec3f e1 = pt1 - pt0;
    XVec3f e2 = pt2 - pt0;

    XVec3f p = d.cross(e2);

    float a = e1.dot(p);

    if(fabs(a)<0.00000001)
      return false;

    float f = 1/a;

    XVec3f s = ray.m_origin - pt0;

    u = f*s.dot(p);
    
    //std::cerr << "U:" << u << std::endl;

    if(u<0 || u>1)
      return false;

    XVec3f q = s.cross(e1);
    v = f*d.dot(q);

    //std::cerr << "V:" << v << std::endl;

    if(v<0 || u+v>1)
      return false;

    t = f*e2.dot(q);

    t *= 1.0f / ray.m_dir.l2();

    //std::cerr << "T:" << t << std::endl;

    if(t<=0)
      return false;

    return true;
  }

public:
  XVec3f m_pts[3], m_normal;
  float m_shine;
  MaterialT m_mat;
};

#endif
