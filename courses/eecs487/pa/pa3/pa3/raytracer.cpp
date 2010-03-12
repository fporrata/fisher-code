#include "raytracer.h"

#include <iostream>

using namespace std;

/// An auxilliary function that produces a pseudo-random floating
/// point number between 0 and 1
float frand() {
  return float(rand()) / (RAND_MAX);
}


void RayTracerT::TraceAll(const CameraT& cam, XImageT<XVec3b>& image) {

  // YOUR CODE HERE
  // viewing rays generation setup here

  // YOUR CODE HERE
  // antialiasing and area light setup goes here

  /// The main loop over all the pixels in the output image
  for(int i=0; i<cam.m_nx; ++i) {
    for(int j=0; j<cam.m_ny; ++j) {
      // generate a ray from the eye
      XVec3f color(0.0f);

      if(m_opts.m_aasample<=0) {
        // YOUR CODE HERE
        // viewing ray generation code replacing this default wrong ray
        ray_t vray(XVec3f(0.0f), XVec3f(1.0f, 0.0f, 0.0f));
        
        m_current_sample = 0;
       
        color = Trace(vray);
      } else {
        // YOUR CODE HERE
        // antialiasing and area light setup
      }
      // image pixel is assigned here -- simple clamping performed
      image.at(i,j) = XVec3b((unsigned char)(0xFF*std::min(1.0f, color(0))), 
                            (unsigned char)(0xFF*std::min(1.0f, color(1))), 
                            (unsigned char)(0xFF*std::min(1.0f, color(2))));
    }
  }
}

/// Returns the color from the shading computation using
/// the information in the hitinfo_t structure
/// level is the recursion level
XVec3f RayTracerT::Shade(const hitinfo_t& hit, int level) {
  XVec3f color(0.0f);

  // Ambient light contribution
  color = hit.m_mat.m_ca*hit.m_mat.m_cr;

  // YOUR CODE HERE
  // shading code here 
  // iterate over the lights and collect their contribution
  // make a recursive call to Trace() function to get the reflections


  return color;
}


/// Returns the color value coming along the ray
/// level is the recursion level
XVec3f RayTracerT::Trace(const ray_t& ray, int level) {
  // YOUR CODE HERE
  // limit the recursion level here

  XVec3f color;

  hitinfo_t hit;

  if(!m_scene.Intersect(ray, hit))
    return m_scene.BackgroundColor();

  color = Shade(hit, level);

  return color;
}

