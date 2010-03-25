#ifndef _TRACE_H
#define _TRACE_H

#include "camera.h"
#include "scene.h"
#include "ximage.h"
#include "xvec.h"

#include <stack>

using namespace std;

// Raytracer's options read from the scene file
struct OptionsT {
  OptionsT() : m_max_recursion(6), m_aasample(0) {}
  int m_max_recursion;
  int m_aasample;
};

/// The main raytracer class
class RayTracerT {
public:
  RayTracerT(const SceneT& _scene, const OptionsT& _opts) 
    : m_scene(_scene), m_opts(_opts), m_current_sample(0)
  {
  }
  void TraceAll(const CameraT& cam, XImageT<XVec3b>& image);
private:
  XVec3f Trace(const ray_t& ray, int level = 0, stack<float> ris = stack<float>());
  XVec3f Shade(const hitinfo_t& hit, int level, stack<float> ris = stack<float>());
private:
  const SceneT& m_scene; // scene reference
  OptionsT m_opts;
  int m_current_sample; // useful for distribution raytracing
};

#endif
