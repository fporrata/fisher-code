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
  PointLightT(const XVec3f & pos, const XVec3f & color) 
			: m_pos(pos), m_color(color)
  {}
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
		AreaLightT(const XVec3f & corner, 
							 const XVec3f & side1, 
							 const XVec3f & side2, 
							 const XVec3f & color) 
				: m_corner(corner), m_side1(side1), m_side2(side2), m_color(color)
		{}

		virtual ~AreaLightT() {}
		virtual XVec3f SamplePos() const 
		{ 
				return m_corner + frand() * m_side1 + frand() * m_side2;
		}
		virtual XVec3f Color() const { return m_color; }
		virtual void HintSample(int k) {}
		virtual std::vector<XVec2f>* Jitters() {
			return NULL;
		}
		std::vector<XVec3f> jitterPos(int nsample) {
			float sub_side = 1.0 / nsample;
			std::vector<XVec3f> jp;
			for (int i = 0; i < nsample; i++) {
				for (int j = 0; j < nsample; j++) {
					jp.push_back(m_corner + m_side1 * (i + frand()) * sub_side +
											 m_side2 * (j + frand()) * sub_side);
				}
			}

			return jp;
		}

	private:
		XVec3f m_corner;
		XVec3f m_side1;
		XVec3f m_side2;
		XVec3f m_color;
};

#endif
