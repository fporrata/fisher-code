#ifndef _GEL_H
#define _GEL_H

#include "ray.h"

/// Geometric primitive interface class
class IGel {
public:
  virtual ~IGel() {
  }
  virtual bool Intersect(const ray_t& ray, hitinfo_t& hit) const = 0;
};

#endif
