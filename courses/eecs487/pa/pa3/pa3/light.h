#ifndef _LIGHT_H
#define _LIGHT_H

#include "xvec.h"
//#include "raytracer.h"

extern float frand();

/// Base interface class for the lights in the scene
class ILight {
public:
  virtual ~ILight() {
  }

  /// samples a position on the light possibly using the hint index
  virtual XVec3f SamplePos() const = 0;
  
  /// returns the color value of the light
  virtual XVec3f Color() const = 0;

  /// sets the hint index for the light
  virtual void HintSample(int k) = 0;

  /// returns the jittered samples array pointer if available
  /// if the jittering cannot be used returns NULL
  virtual std::vector<XVec2f>* Jitters() = 0;
};

/// A simple point light class
class PointLightT: public ILight {
public:
  PointLightT(const XVec3f& _pos, const XVec3f& _color) 
    : m_pos(_pos), m_color(_color)
  {
  }
  virtual ~PointLightT() {}
  virtual XVec3f SamplePos() const { return m_pos; }
  virtual XVec3f Color() const { return m_color; }
  virtual void HintSample(int k) {}
  virtual std::vector<XVec2f>* Jitters() {
    return NULL;
  }
private:
  XVec3f m_pos;
  XVec3f m_color;
};

class AreaLightT: public ILight{
	public:
		AreaLightT(const float width, const XVec3f & color)
			: m_width(width), m_color(color) {}

		virtual ~AreaLightT() {}
		virtual XVec3f SamplePos() const 
		{ 
			return XVec3f(0.0f);
			return XVec3f(-m_width + 2 * frand() * m_width,
										-m_width + 2 * frand() * m_width,
										0.0f);
		}
		virtual XVec3f Color() const { return m_color; }
		virtual void HintSample(int k) {}
		virtual std::vector<XVec2f>* Jitters() {
			return NULL;
		}
	private:
		float m_width;
		XVec3f m_color;
};

#endif
