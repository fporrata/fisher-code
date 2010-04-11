#include <cstdio>
#include <cassert>
#include <vector>

extern "C" {
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
}

#include "xvec.h"
#include "xmat.h"

/* a simple interpolation template class */
template<class Type>
class Interpolator
{
public:
	/* add a keyframe to the end */
	void appendKeyframeValue(Type k) { _keyframes.push_back(k); }
	
	/* delete the keyframes */
	void clearKeyframes() { _keyframes.clear(); }
	
	/* the total number of keyframes thus far */
	unsigned int keyframeCount() { return _keyframes.size(); }
	
	/* return the interpolated value */
	Type interpolatedValueAtTime(float t)
	{
		/* be sure there is at least one keyframe */
		int numKeyframes = _keyframes.size();
		assert(numKeyframes > 0);
		
		/* if the time is negative, or there is only one
		 keyframe, return the first keyframe's value */
		if( t < 0.0f || numKeyframes == 1)
			return _keyframes[0];
		
		/* if the time is greater than the number of
		 keyframes, return the last one's value */
		else if( t > (float)(numKeyframes - 1) )
			return _keyframes[numKeyframes-1];
		
		/* if there are 2 keyframes, do a linear interpolation */
		else if ( numKeyframes == 2 )
			return (1-t)*_keyframes[0] + t*_keyframes[1];

		/* more than 2 keyframes */
		else {
			XVec<4, Type> m;
			
			/* if time is in the first span, use quadratic interpolation */
			if( t >= 0.0f && t < 1.0f )
				calculateBeginQuadraticSpline(_keyframes[0], _keyframes[1],
											  _keyframes[2], m);
			
			/* if time is in the last span, use quadratic interpolation */
			else if( t > (float)(_keyframes.size() - 2) )
				calculateEndQuadraticSpline(_keyframes[numKeyframes-3],
											  _keyframes[numKeyframes-2],
											  _keyframes[numKeyframes-1], m);

			/* use catmull-rom interpolation */
			else {
				int interval = (int)t;
				
				calculateCatmullRomSpline(_keyframes[interval-1], _keyframes[interval],
										  _keyframes[interval+1], _keyframes[interval+2], m);
			}
		
			/* given the prepared components, m,
			 compute the interpolated value */
			return evaluateSpline(t - (int)t, m);
		}
	}
private:
	/* the keyframe data */
	std::vector<Type> _keyframes;
	
	/* Given 4 points p0...p3, calculate the
	 coefficients a (each row) of the
	 catmull-rom spline interpolating them */
	void calculateCatmullRomSpline(Type p0, Type p1,
								   Type p2, Type p3,
								   XVec<4, Type>& m)
	{
		/* constraint matrix */
		static XMat4f C(1.0f,  1.0f,  1.0f,  1.0f,
					   -1.0f,  0.0f,  1.0f,  2.0f,
						1.0f,  0.0f,  1.0f,  4.0f,
						1.0f,  0.0f,  1.0f,  6.0f);
		
		/* basis matrix */
		XMat4f B = C.inverse();
		
		for( int i = 0; i < 4; ++i )
			m(i) = B(i, 0)*p0 + B(i,1)*p1 + B(i,2)*p2 + B(i,3)*p3;
	}
	
	/* Given 3 points p0, p1, p2, calculate the
	 coefficients a (each row) of the
	 catmull-rom spline interpolating them */
	void calculateBeginQuadraticSpline(Type p0, Type p1,
									   Type p2, XVec<4, Type>& m)
	{
		/* constraint matrix */
		static XMat4f C(1.0f,  1.0f,  1.0f,  0.0f,
						0.0f,  1.0f,  3.0f,  0.0f,
						0.0f,  1.0f,  5.0f,  0.0f,
						0.0f,  0.0f,  0.0f,  1.0f);
		
		/* basis matrix */
		XMat4f B = C.inverse();
		
		for( int i = 0; i < 4; ++i )
			m(i) = B(i, 0)*p0 + B(i,1)*p1 + B(i,2)*p2;
	}
	
	/* Given 3 points p0, p1, p2, calculate the
	 coefficients a (each row) of the
	 catmull-rom spline interpolating them */
	void calculateEndQuadraticSpline(Type p0, Type p1,
									   Type p2, XVec<4, Type>& m)
	{
		/* constraint matrix */
		static XMat4f C(1.0f,  1.0f,  1.0f,  0.0f,
					   -1.0f,  0.0f,  1.0f,  0.0f,
						1.0f,  0.0f,  1.0f,  0.0f,
						0.0f,  0.0f,  0.0f,  1.0f);
		
		/* basis matrix */
		XMat4f B = C.inverse();
		
		for( int i = 0; i < 4; ++i )
			m(i) = B(i, 0)*p0 + B(i,1)*p1 + B(i,2)*p2;
	}
	
	/* Interpolate between 4-vectors:
	 Evaluate the spline represented by the
	 4-vector coefficients a[0]...a[3]
	 at the given value of the parameter t,
	 and store in the result function */
	Type evaluateSpline(double t,			/* 0<= parameter <= 1 */
						 XVec<4, Type>& m	/* coefficients */
						 )
	{
		const double t2 = t*t;
		const double t3 = t2*t;

		return m(0) + m(1)*t + m(2)*t2 + m(3)*t3;
	}
};
