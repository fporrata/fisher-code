#ifndef _SCENE_H
#define _SCENE_H

#include "camera.h"
#include "gel.h"

#include <vector>
#include "material.h"
#include "light.h"

#include <iostream>

// Scene class
class SceneT {
  typedef std::vector<IGel*> GelCt; // geometric primitive container type
public:
  typedef std::vector<ILight*> LightCt; // light container type
public:
  SceneT() 
    : m_bkcolor(0.0f, 0.0f, 0.30f)
  {
  }

  ~SceneT() {
    GelCt::iterator gi;
    for(gi=m_gels.begin(); gi!=m_gels.end(); ++gi)
      delete *gi;
    LightCt::iterator li;
    for(li=m_lights.begin(); li!=m_lights.end(); ++li)
      delete *li;
  }

  /// Tests intersection with the ray
  bool Intersect(const ray_t& ray, hitinfo_t& hit) const;

  /// returns the background color
  const XVec3f& BackgroundColor() const {
    return m_bkcolor;
  }
  /// sets the background color
  void BackgroundColor(const XVec3f& color) {
    m_bkcolor = color;
  }

  LightCt::const_iterator BeginLights() const {
    return m_lights.begin();
  }
  LightCt::const_iterator EndLights() const {
    return m_lights.end();
  }

  void AddLight(ILight* light) {
    m_lights.push_back(light);
  }

  void AddGel(IGel* gel) {
    if(gel==0)
      return;
    m_gels.push_back(gel);
  }

   void print_lights() const {
      std::cerr << "num lights: " << m_lights.size() << std::endl;
      for (LightCt::const_iterator i = BeginLights();
           i != EndLights(); ++i) {
         std::cerr << "  " << (*i)->Color() << std::endl;
      }
   }
private:
  GelCt m_gels;     /// geometric primitives
  LightCt m_lights; /// lights
  XVec3f m_bkcolor;  /// background color
};

#endif
