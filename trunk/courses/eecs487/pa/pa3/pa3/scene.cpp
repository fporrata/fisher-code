#include "scene.h"
#include "gel.h"

using namespace std;

const float EPSILON = 0.0001f;

/// Intersects a ray [EPSILON, +infinity) with the scene
/// returns true and fills in the hitinfo structure if the ray hits the scene
/// otherwise returns false
bool SceneT::Intersect(const ray_t& ray, hitinfo_t& hit) const {

  GelCt::const_iterator gi;

  hit.m_t = float(1e23);

  hitinfo_t curhit;
  curhit.m_indir = ray.m_dir;

  bool result = false;

  for(gi=m_gels.begin(); gi!=m_gels.end(); ++gi) {
    if((*gi)->Intersect(ray, curhit)) {
      //cerr << "intersected! " << curhit.m_t << endl;
      if(curhit.m_t>EPSILON && curhit.m_t<hit.m_t) {
        hit = curhit;
        result = true;
      }
    }
  }

  return result;
}


