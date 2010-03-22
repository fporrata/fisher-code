#ifndef _BALL_H
#define _BALL_H

#include "gel.h"

#include "xform.h"

#include <iostream>

/// Sphere class 
class BallT: public IGel {
public:
  BallT() : m_radius(-1) {}
  BallT(float _radius, const XVec3f& _center, const XFormf& _xf = XFormf::Identity())
    : m_radius(_radius), m_center(_center), m_xf(_xf) 
  {
    m_xfi = m_xf.Inverse();
  }

  virtual ~BallT() {}
  
  /// Intersection testing, uses the stored transformation
  virtual bool Intersect(const ray_t& wray, hitinfo_t& hit) const {

    // take care of non-trivial transform here
    // transforming the ray into the coordinate system of the ball
    ray_t ray(m_xfi.apply(wray.m_origin), m_xfi.applyv(wray.m_dir));

    float a = ray.m_dir.dot();
    float b = 2*ray.m_dir.dot(ray.m_origin-m_center);
    float c = (ray.m_origin-m_center).dot() - m_radius*m_radius;
    float det = b*b - 4*a*c;
    if(det<=0.0)
      return false;

    float t1 = 0.5f*( -b - sqrt(det) )/a;
    float t2 = 0.5f*( -b + sqrt(det) )/a;

    if(t1>0) {
      hit.m_t = t1;
      hit.m_pos = ray.at(t1);
      hit.m_normal = hit.m_pos - m_center;
      hit.m_mat = m_mat;
    } else if(t2>0) {
      hit.m_t = t2;
      hit.m_pos = ray.at(t2);
      hit.m_normal = hit.m_pos - m_center;
      hit.m_mat = m_mat;
    } else
      return false;

    /// transform it back to the world coordinates
    hit.m_pos = m_xf.apply(hit.m_pos);
    hit.m_normal = m_xfi.applytv(hit.m_normal);
    hit.m_normal.normalize();

    return true;
  }

public:
  float m_radius;
  XVec3f m_center;

  // the transformation and its inverse
  XFormf m_xf, m_xfi;
  MaterialT m_mat;
};

#endif
