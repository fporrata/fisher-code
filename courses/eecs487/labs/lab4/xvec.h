/*
 * Copyright (c) 2010 University of Michigan, Ann Arbor.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of Michigan, Ann Arbor. The name of the University 
 * may not be used to endorse or promote products derived from this 
 * software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * Author:
	Ari Grant (grantaa@umich.edu)
	Sugih Jamin (sugih@eecs.umich.edu)
 */

#ifndef  __XVEC_H__
#define  __XVEC_H__

#ifndef _NO_IOSTREAMS
#include <iostream>
#endif

#include <assert.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279
#endif /* M_PI */

// Column-type vector class.
template<int dim, class real_type> 
class XVec
{
public:
	XVec() {}
	
	explicit XVec(real_type f)
	{ 
		for(int i=0; i<dim; ++i)
			m_v[i] = f;
	}
	
	XVec(real_type f0, real_type f1 )
	{ 
		assert(dim>1);
		m_v[0] = f0;
		m_v[1] = f1;
	}
	
	XVec(real_type f0, real_type f1, real_type f2)
	{ 
		assert(dim>2);
		m_v[0] = f0;
		m_v[1] = f1;
		m_v[2] = f2;
	}
	
	XVec(real_type f0, real_type f1, real_type f2, real_type f3)
	{ 
		assert(dim>3);
		m_v[0] = f0;
		m_v[1] = f1;
		m_v[2] = f2;
		m_v[3] = f3;
	}
	
	XVec(const XVec& c)
	{ 
		for(int i=0; i<dim; ++i)
			m_v[i] = c.m_v[i];
	}
	
	explicit XVec(const real_type* a)
	{ 
		for(int i=0; i<dim; ++i)
			m_v[i] = a[i];
	}
	
	template<class oreal_type> 
	explicit XVec(const XVec<dim, oreal_type>& c)
	{ 
		for(int i=0; i<dim; ++i)
			m_v[i] = static_cast<oreal_type>(c(i));
	}

	operator real_type*() {  return &m_v[0]; }
	
	operator const real_type*() const { return &m_v[0]; }


	bool operator==(const XVec& c) const
	{ 
		for(int i=0; i<dim; ++i)
			if(m_v[i]!=c.m_v[i])
				return false;
		return true;
	}
	
	bool operator!=(const XVec& c) const {  return !((*this)==c); }
	
	template<int cDim, class cReal_type>
	XVec& operator=(const XVec<cDim, cReal_type>& c)
	{ 
		for(int i=0; i<dim && i<cDim; ++i)
			m_v[i] = c(i);
		return *this;
	}

	XVec operator+(const XVec& c) const
	{ 
		XVec pt(*this);
		for(int i=0; i<dim; ++i)
			pt[i] += c.m_v[i];
		return pt;
	}
	
	XVec operator-(const XVec& c) const
	{ 
		XVec pt(*this);
		for(int i=0; i<dim; ++i)
			pt[i] -= c.m_v[i];
		return pt;
	}
	
	XVec operator*(real_type s) const
	{ 
		XVec pt(*this);
		for(int i=0; i<dim; ++i)
			pt[i] *= s;
		return pt;
	}
	
	friend XVec operator*(real_type s, const XVec& c)
	{ 
		XVec pt(c);
		for(int i=0; i<dim; ++i)
			pt[i] *= s;
		return pt;
	}
	
	XVec operator/(real_type s) const
	{ 
		XVec pt(*this);
		for(int i=0; i<dim; ++i)
			pt[i] /= s;
		return pt;
	}
	
	XVec& operator+=(const XVec& c)
	{ 
		for(int i=0; i<dim; ++i)
			m_v[i] += c.m_v[i];
		return *this; 
	}
	
	XVec& operator-=(const XVec& c)
	{ 
		for(int i=0; i<dim; ++i)
			m_v[i] -= c.m_v[i];
		return *this; 
	}
	
	XVec& operator*=(real_type s)
	{ 
		for(int i=0; i<dim; ++i)
			m_v[i] *= s;
		return *this; 
	}
	
	XVec& operator/=(real_type s)
	{ 
		for(int i=0; i<dim; ++i)
			m_v[i] /= s;
		return *this; 
	}
	
	XVec operator-() const
	{ 
		XVec pt(*this);
		for(int i=0; i<dim; ++i)
			pt[i] = -pt[i];
		return pt;
	}

	XVec operator*(const XVec& c) const
	{
		XVec pt(*this);
		for(int i=0; i<dim; ++i)
			pt[i] *= c.m_v[i];
		return pt;
	}

	XVec operator/(const XVec& c) const
	{
		XVec pt(*this);
		for(int i=0; i<dim; ++i)
			pt[i] /= c.m_v[i];
		return pt;
	}
	
	real_type& operator() (const int i) { return m_v[i]; }
	
	real_type operator() (const int i) const { return m_v[i]; }
	
	const real_type& ref() const { return m_v[0]; }
	
	real_type& x() { return m_v[0]; }
	
	real_type& y()
	{
		assert(dim>1);
		return m_v[1];
	}
	
	real_type& z()
	{
		assert(dim>2);
		return m_v[2];
	}
	
	real_type& w()
	{
		assert(dim>3);
		return m_v[3];
	}
	
	real_type& red() { return x(); }
	
	real_type& green() { return y(); }
	
	real_type& blue() { return z(); }
	
	real_type& alpha() { return w(); }
	
	inline real_type& width() { return z(); }
	inline real_type& height() { return w(); }
	
	// Updates bounding box corners to include itself.
	void bbox(XVec& cmin, XVec& cmax) const
	{
		for(int i=0; i<dim; ++i) {
			if( m_v[i] < cmin.m_v[i] ) 
				cmin.m_v[i] = m_v[i];
			if( m_v[i] > cmax.m_v[i] ) 
				cmax.m_v[i] = m_v[i];
		}
	}

	// Dot product.
	real_type dot(const XVec& c ) const
	{
		real_type d = 0;
		for(int i=0; i<dim; ++i) 
			d += m_v[i] * c.m_v[i];
		return d;
	}
	
	XVec cross(const XVec& c ) const
	{ 
		// Cross-product is only defined for 3D vectors 
		// and it is specialized below.
		assert(false);
	}
	
	// Dot product with itself -- vector norm squared.
	real_type dot() const
	{ 
		real_type d = 0;
		for(int i=0; i<dim; ++i) 
			d += m_v[i] * m_v[i];
		return d;
	}
	
	// norm of a vector.
	real_type norm() const { return sqrtf(dot()); }
	
	// Euclidean distance between two points.
	real_type dist(const XVec& c) const { return (*this - c).norm(); }
	
	float angle(const XVec& c) const
	{
		if( fabsf(c.dot()) < 1e-25 || fabsf(dot()) < 1e-25 )
			return 0.0;
		
		return acosf(dot(c)/(norm()*c.norm()));
	}
	
	int normalize()
	{ 
		real_type mag = norm(); 
		return (fabsf( mag )<1e-25) ? 0 : ( *this *= 1 / mag, 1 ); 
	}
	
	float polarAngle()
	{
		assert(dim>1);
		return atan2f(m_v[1], m_v[0]);
	}

	float azimuthalAngle()
	{
		assert(dim>2);
		return atan2f(m_v[2], sqrtf(m_v[0]*m_v[0]+m_v[1]*m_v[1]));
	}
	
	// Projection of this onto c
	XVec project(const XVec& c)
	{
		if( fabsf(dot()) < 1e-25 )
		   return XVec(0.0);
		return c*dot(c)/c.dot();
	}
	
	XVec rotate(const XVec& c, float alpha)
	{
		XVec<3, real_type> a(c);
		a.normalize();
		XVec<3, real_type> b(*this);
		XVec v = (1-cosf(alpha))*a.dot(b)*a + b*cosf(alpha)+a.cross(b)*sinf(alpha);
		return v;
	}
	
protected:
	real_type m_v[dim];
};

// This can be done shorter with partial template specialization but some
// compilers do not support it as of now.
template<>
inline XVec<3, float> XVec<3, float>::cross( const XVec<3, float>& c ) const
{ 
	return XVec<3, float>( m_v[1] * c.m_v[2] - m_v[2] * c.m_v[1],
						  m_v[2] * c.m_v[0] - m_v[0] * c.m_v[2],
						  m_v[0] * c.m_v[1] - m_v[1] * c.m_v[0] ); 
}

template<>
inline XVec<3, double> XVec<3, double>::cross( const XVec<3, double>& c ) const
{ 
	return XVec<3, double>( m_v[1] * c.m_v[2] - m_v[2] * c.m_v[1],
						   m_v[2] * c.m_v[0] - m_v[0] * c.m_v[2],
						   m_v[0] * c.m_v[1] - m_v[1] * c.m_v[0] ); 
}

#ifndef _NO_IOSTREAMS
template<int dim, class real_type> 
std::ostream& operator<<( std::ostream& os, const XVec<dim, real_type>& c )
{
	for(int i=0; i<dim-1; ++i) 
		os << c(i) << " ";
	os << c(dim-1);
	return os;
}

template<int dim, class real_type> 
std::istream&
operator>>( std::istream& is, XVec<dim, real_type>& f )
{
	return is >> f.x() >> f.y() >> f.z();
}
#endif

typedef XVec<2, double> XVec2d;
typedef XVec<2, float> XVec2f;
typedef XVec<2, int> XVec2i;

typedef XVec<3, double> XVec3d;
typedef XVec<3, float> XVec3f;
typedef XVec<3, int> XVec3i;

typedef XVec<4, double> XVec4d;
typedef XVec<4, float> XVec4f;
typedef XVec<4, int> XVec4i;

typedef XVec<3, unsigned int> XVec3ui;
typedef XVec<4, unsigned int> XVec4ui;

#endif  // __XVEC_H__
