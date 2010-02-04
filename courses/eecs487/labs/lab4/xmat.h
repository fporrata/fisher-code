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

#ifndef  __XMAT_H__
#define  __XMAT_H__

#ifndef _NO_IOSTREAMS
#include <iostream>
#endif

#include <assert.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.141592653589793238462643383279
#endif /* M_PI */

#include "xvec.h"

// Simple matrix class.
template<int dim, class real_type> 
class XMat
{
public:
	
	XMat() { toIdentity(); }
	
	explicit XMat(real_type f)
	{
		for(int i=0; i<dim*dim; ++i)
			m_v[i] = f;
	}
	
	XMat(const XMat& c)
	{
		for(int i=0; i<dim*dim; ++i)
			m_v[i] = c.m_v[i];
	}
	
	explicit XMat(const real_type* a)
	{
		for(int i=0; i<dim*dim; ++i)
			m_v[i] = a[i];
	}
	
	template<class oreal_type> 
	explicit XMat(const XMat<dim, oreal_type>& c)
	{
		for(int i=0; i<dim*dim; ++i)
			m_v[i] = static_cast<oreal_type>(c(i));
	}
	
	operator real_type*() { return &m_v[0]; }
	
	operator const real_type*() const { return &m_v[0]; }
	
	bool operator==(const XMat& c) const
	{
		for(int i=0; i<dim*dim; ++i)
			if(m_v[i]!=c.m_v[i])
				return false;
		return true;
	}
	
	bool operator!=(const XMat& c) const { return !((*this)==c); }
	
	XMat& operator=(const XMat& c)
	{
		for(int i=0; i<dim*dim; ++i)
			m_v[i] = c.m_v[i];
		return *this; 
	}

	XMat operator+(const XMat& c) const
	{
		XMat pt(*this);
		for(int i=0; i<dim*dim; ++i)
			pt[i] += c.m_v[i];
		return pt;
	}
	
	XMat operator-(const XMat& c) const
	{
		XMat pt(*this);
		for(int i=0; i<dim*dim; ++i)
			pt[i] -= c.m_v[i];
		return pt;
	}
	
	XMat operator*(real_type s) const
	{
		XMat pt(*this);
		for(int i=0; i<dim*dim; ++i)
			pt[i] *= s;
		return pt;
	}
	
	friend XMat operator*(real_type s, const XMat& c)
	{
		XMat pt(c);
		for(int i=0; i<dim*dim; ++i)
			pt[i] *= s;
		return pt;
	}
	
	XVec4f operator*(XVec4f u) const
	{
		XVec4f v(u);
		for(int i=0; i<dim; ++i)
			v(i) = row(i).dot(u);
		return v;
	}
	
	XMat operator/(real_type s) const
	{
		XMat pt(*this);
		for(int i=0; i<dim*dim; ++i)
			pt[i] /= s;
		return pt;
	}
	
	XMat& operator+=(const XMat& c)
	{
		for(int i=0; i<dim*dim; ++i)
			m_v[i] += c.m_v[i];
		return *this; 
	}
	
	XMat& operator-=(const XMat& c)
	{
		for(int i=0; i<dim*dim; ++i)
			m_v[i] -= c.m_v[i];
		return *this; 
	}
	
	XMat& operator*=(real_type s)
	{
		for(int i=0; i<dim*dim; ++i)
			m_v[i] *= s;
		return *this; 
	}
	
	XMat& operator/=(real_type s)
	{
		for(int i=0; i<dim*dim; ++i)
			m_v[i] /= s;
		return *this; 
	}
	
	XMat operator-() const
	{
		XMat pt(*this);
		for(int i=0; i<dim*dim; ++i)
			pt[i] = -pt[i];
		return pt;
	}
	
	XMat operator*(const XMat& c) const
	{
		XMat pt(0.0);
		/* row */
		for(int i=0; i<dim; ++i)
		{
			/* column */
			for(int j=0; j<dim; ++j)
				pt.m_v[dim*j+i] = row(i).dot(c.column(j));
			
		}
		return pt;
	}
	
	XMat& operator*=(const XMat& c)
	{	
		XMat pt(0.0);
		/* row */
		for(int i=0; i<dim; ++i)
		{
			/* column */
			for(int j=0; j<dim; ++j)
				pt.m_v[dim*j+i] = row(i).dot(c.column(j));
			
		}
		*this = pt;
		
		return *this;
	}

	real_type& operator() (const int r, const int c) { return m_v[c*dim+r]; }
	
	real_type operator() (const int r, const int c) const { return m_v[c*dim+r]; }
	
	const real_type& ref() const { return m_v[0]; }
	
	XVec<dim, real_type> row(const int r) const
	{
		assert(r<dim && r>=0);
		
		XVec<dim, real_type> v;
		for(int i=0; i<dim; ++i)
			v[i] = m_v[dim*i+r];
		
		return v;
	}
	
	XVec<dim, real_type> column(const int c) const
	{
		assert(c<dim && c>=0);
		
		XVec<dim, real_type> v;
		for(int i=0; i<dim; ++i)
			v[i] = m_v[dim*c+i];
		
		return v;
	}
	
	template<int cDim, class cRealType>
	void setColumn(const int c, XVec<cDim, cRealType> &v)
	{
		assert(c<dim && c>=0);
		for(int i=0; i<dim && i<cDim; ++i)
			m_v[dim*c+i] = v(i);
	}
	
	template<int cDim, class cRealType>
	void setRow(const int r, XVec<cDim, cRealType> &v)
	{
		assert(r<dim && r>=0);

		for(int i=0; i<dim && i<cDim; ++i)
			m_v[dim*i+r] = v(i);
	}
	
	XMat transpose() const
	{
		XMat pt(*this);
		for(int i=0; i<dim; ++i)
		{
			for(int j=0; j<i; ++j)
			{
				real_type temp = pt[dim*i+j];
				pt[dim*i+j] = pt[dim*j+i];
				pt[dim*j+i] = temp;
			}
		}
		return pt;
	}

	void toIdentity()
	{
		for( int i = 0; i < dim*dim; i++ )
		{
			if( i % (dim+1) == 0 )
				m_v[i] = 1;
			else
				m_v[i] = 0;
		}
	}
	
	template<int vDim, class vReal_type>
	void translation(XVec<vDim, vReal_type> vec)
	{
		assert(dim>1);
		toIdentity();
		for( int i=0; i<dim && i<vDim; i++ )
			m_v[dim*(dim-1)+i] = vec(i);
	}
	
	template<int vDim, class vReal_type>
	void scale(XVec<vDim, vReal_type> vec)
	{
		toIdentity();
		for( int i=0; i<dim && i<vDim; i++ )
			m_v[(dim+1)*i] = vec(i);
	}
	
	void rotate(XVec<dim, real_type> vec)
	{
		assert(dim==4);
		toIdentity();
		
		float s = sinf(vec(3));
		float c = cosf(vec(3));

		float x = vec.x();
		float y = vec.y();
		float z = vec.z();
	
		m_v[0] = (1-c)*x*x+c;
		m_v[1] = (1-c)*x*y+s*z;
		m_v[2] = (1-c)*x*z-s*y;
		
		m_v[0+dim] = (1-c)*x*y-s*z;
		m_v[1+dim] = (1-c)*y*y+c;
		m_v[2+dim] = (1-c)*y*z+s*x;
		
		m_v[0+2*dim] = (1-c)*x*z+s*y;
		m_v[1+2*dim] = (1-c)*y*z-s*x;
		m_v[2+2*dim] = (1-c)*z*z+c;
	}
	
	/* return invert of the matrix */
	XMat inverse() const
	{
		XMat inv(0.0);
		inv.toIdentity();

		XMat copy(*this);
		
		float div;
		int i, i1,j;
		
		/* systematically perform elementary row operations on A and A_inv so that
		 * A becomes the identity matrix and A_inv becomes the inverse */
		
		for(i=0; i<dim; ++i)
		{
			/* Divide through by A[i][i] to get 1 on the diagonal */
			div = copy(i, i);
			if( fabsf(div) < 1e-15 )
				return XMat(*this);
			for(j=0; j<dim; ++j)
			{
				copy(i, j) /= div;
				inv(i, j) /= div;
			}
			
			/* Subtract a factor times this row from other rows to get zeros above and below the 1 just created by division */
			for(i1=0; i1<dim; ++i1)
			{
				if(i1 == i) continue; /* skip the present row */
				div = copy(i1, i);
				for(j=0; j<dim; ++j)
				{
					copy(i1, j) -= div*copy(i, j);
					inv(i1, j) -= div*inv(i, j);
				}
			}
		}
		
		return inv;
	}
	
protected:
	real_type  m_v[dim*dim];
};


#ifndef _NO_IOSTREAMS
template<int dim, class real_type> 
std::ostream& operator<<( std::ostream& os, const XMat<dim, real_type>& c )
{	
	for(int i=0; i<dim; ++i) {
		for(int j=0; j<dim; ++j)
			os << c(j,i) << " ";
		os << std::endl;
	}
	return os;
}

template<int dim, class real_type> 
std::istream& operator>>( std::istream& is, XMat<dim, real_type>& f )
{
	for( int i = 0; i < dim; i++ )
		for( int j = 0; j < dim; j++ )
			is >> f(i);
	return is;
}
 
#endif

typedef XMat<2, double> XMat2d;
typedef XMat<2, float> XMat2f;

typedef XMat<3, double> XMat3d;
typedef XMat<3, float> XMat3f;

typedef XMat<4, double> XMat4d;
typedef XMat<4, float> XMat4f;

#endif  // __XVEC_H__
