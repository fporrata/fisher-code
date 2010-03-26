/*
 * Name: 			Fu Yu
 * UMID: 			33551750
 * uniqname:	yufu
 */

#ifndef _XFORM_H
#define _XFORM_H

#ifndef _NO_IOSTREAMS
#include <iostream>
#endif

#include <math.h>
#include "xvec.h"
#include <cstdlib>
#include <cstring>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433832795
#endif

// 3x4 matrix class
template<class real_t> 
class XFormT {
public:
  XFormT(void) {
  }
  
  XFormT(const XFormT& xf) {
    memcpy((void*)m_a, (void*)xf.m_a, 12*sizeof(real_t));
  }

  XFormT& operator=(const XFormT& xf) {
    memcpy(m_a, xf.m_a, 12*sizeof(real_t));
    return *this;
  }

  ~XFormT() {}

  /// returns an identity transform
  static XFormT Identity() {
    XFormT xf;
    memset(xf.m_a, 0, 12*sizeof(real_t));
    xf.at(0,0) = xf.at(1,1) = xf.at(2,2) = 1;
    return xf;
  }

  /// returns a translation transform
  static XFormT Translation(XVec3f t) {
    XFormT xf;
    memset(xf.m_a, 0, 12*sizeof(real_t));
    xf.at(0,0) = xf.at(1,1) = xf.at(2,2) = 1;
    xf.at(0,3) = t(0);
    xf.at(1,3) = t(1);
    xf.at(2,3) = t(2);
    return xf;
  }

  /// returns a scaling transform
  static XFormT Scaling(real_t sx, real_t sy, real_t sz) {
    XFormT xf;
    memset(xf.m_a, 0, 12*sizeof(real_t));
    xf.at(0,0) = sx;
    xf.at(1,1) = sy;
    xf.at(2,2) = sz;
    return xf;
  }

  /// returns a rotation transform
  static XFormT Rotation(real_t angle_degree, const XVec<3, real_t>& axis) {
    XFormT xf;
    XVec<3, real_t> a = axis;
    a.normalize();
    real_t s = sinf(angle_degree*float(M_PI)/180.0f);
    real_t c = cosf(angle_degree*float(M_PI)/180.0f);

    xf.m_a[0] = a.x()*a.x() + (1.0f-a.x()*a.x())*c;
    xf.m_a[1] = a.x()*a.y()*(1.0f-c) - a.z()*s;
    xf.m_a[2] = a.x()*a.z()*(1.0f-c) + a.y()*s;
    xf.m_a[3] = 0;

    xf.m_a[4] = a.y()*a.x()*(1.0f-c) + a.z()*s;
    xf.m_a[5] = a.y()*a.y() + (1.0f-a.y()*a.y())*c;
    xf.m_a[6] = a.y()*a.z()*(1.0f-c) + a.x()*s;
    xf.m_a[7] = 0;

    xf.m_a[8]  = a.z()*a.x()*(1.0f-c) - a.y()*s;
    xf.m_a[9]  = a.z()*a.y()*(1.0f-c) + a.x()*s;
    xf.m_a[10] = a.z()*a.z() + (1.0f-a.z()*a.z())*c;
    xf.m_a[11] = 0;

    return xf;
  }

  /// access to the (i,j) element
  const real_t& at(int i, int j) const {
    return m_a[ (i<<2)+j ];
  }

  /// access to the (i,j) element
  real_t& at(int i, int j) {
    return m_a[ (i<<2)+j ];
  }

  // apply to a point
  XVec<3, real_t> apply(const XVec<3, real_t>& pt) const {
    XVec<3, real_t> ret;
    ret(0) = m_a[0]*pt(0) + m_a[1]*pt(1) + m_a[2]*pt(2)  + m_a[3];
    ret(1) = m_a[4]*pt(0) + m_a[5]*pt(1) + m_a[6]*pt(2)  + m_a[7];
    ret(2) = m_a[8]*pt(0) + m_a[9]*pt(1) + m_a[10]*pt(2) + m_a[11];
    return ret;
  }

  // apply to a vector -- do not add translation part
  XVec<3, real_t> applyv(const XVec<3, real_t>& vec) const {
    XVec<3, real_t> ret;
    ret(0) = m_a[0]*vec(0) + m_a[1]*vec(1) + m_a[2]*vec(2);
    ret(1) = m_a[4]*vec(0) + m_a[5]*vec(1) + m_a[6]*vec(2);
    ret(2) = m_a[8]*vec(0) + m_a[9]*vec(1) + m_a[10]*vec(2);
    return ret;
  }

  // apply the transpose to a vector -- do not add translation part
  XVec<3, real_t> applytv(const XVec<3, real_t>& vec) const {
    XVec<3, real_t> ret;
    ret(0) = m_a[0]*vec(0) + m_a[4]*vec(1) + m_a[8]*vec(2);
    ret(1) = m_a[1]*vec(0) + m_a[5]*vec(1) + m_a[9]*vec(2);
    ret(2) = m_a[2]*vec(0) + m_a[6]*vec(1) + m_a[10]*vec(2);
    return ret;
  }

  // computes the inverse transformation 
  XFormT<real_t> Inverse() const {
    XFormT<real_t> xf;
    //xf.m_a[3] = xf.m_a[7] = xf.m_a[11] = 0;
    xf.m_a[0] = m_a[5]*m_a[10] - m_a[6]*m_a[9];
    xf.m_a[4] = m_a[6]*m_a[8] - m_a[4]*m_a[10];
    xf.m_a[8] = m_a[4]*m_a[9] - m_a[5]*m_a[8];
    real_t det = m_a[0]*xf.m_a[0] + m_a[1]*xf.m_a[4] + m_a[2]*xf.m_a[8];
    real_t idet = real_t(1) / det;
    xf.m_a[1] = m_a[9]*m_a[2] - m_a[10]*m_a[1];
    xf.m_a[5] = m_a[0]*m_a[10] - m_a[2]*m_a[8];
    xf.m_a[9] = m_a[8]*m_a[1] - m_a[0]*m_a[9];
    
    xf.m_a[2]  = m_a[1]*m_a[6] - m_a[2]*m_a[5];
    xf.m_a[6]  = m_a[2]*m_a[4] - m_a[0]*m_a[6];
    xf.m_a[10] = m_a[0]*m_a[5] - m_a[1]*m_a[4];

    xf.m_a[0] *= idet;
    xf.m_a[1] *= idet;
    xf.m_a[2] *= idet;

    xf.m_a[4] *= idet;
    xf.m_a[5] *= idet;
    xf.m_a[6] *= idet;
    
    xf.m_a[8] *= idet;
    xf.m_a[9] *= idet;
    xf.m_a[10] *= idet;

    xf.m_a[3]  = - xf.m_a[0]*m_a[3] - xf.m_a[1]*m_a[7] - xf.m_a[2]*m_a[11];
    xf.m_a[7]  = - xf.m_a[4]*m_a[3] - xf.m_a[5]*m_a[7] - xf.m_a[6]*m_a[11];
    xf.m_a[11] = - xf.m_a[8]*m_a[3] - xf.m_a[9]*m_a[7] - xf.m_a[10]*m_a[11];

    return xf;
  }

  // multiplies the matrix on the right
  XFormT<real_t>& operator*=(const XFormT<real_t>& xf) {
    // cannot really do this in-place can we?
    for(int k=0; k<3; ++k) {
      XVec<4, real_t> row(&m_a[4*k]);
      m_a[4*k+0] = row(0)*xf.m_a[0] + row(1)*xf.m_a[4] + row(2)*xf.m_a[8];
      m_a[4*k+1] = row(0)*xf.m_a[1] + row(1)*xf.m_a[5] + row(2)*xf.m_a[9];
      m_a[4*k+2] = row(0)*xf.m_a[2] + row(1)*xf.m_a[6] + row(2)*xf.m_a[10];
      m_a[4*k+3] = row(0)*xf.m_a[3] + row(1)*xf.m_a[7] + row(2)*xf.m_a[11] + row(3);
    }
    return *this;
  }
  //XVecT<real_t> o

protected:
  real_t  m_a[12];
};

#ifndef _NO_IOSTREAMS
template<class real_t> 
std::ostream& operator<<( std::ostream& os, const XFormT<real_t>& xf )
{
  os << xf.at(0,0) << " " << xf.at(0,1) << " " << xf.at(0,2) << " " 
    << xf.at(0,3) << std::endl;
  os << xf.at(1,0) << " " << xf.at(1,1) << " " << xf.at(1,2) << " " 
    << xf.at(1,3) << std::endl;
  os << xf.at(2,0) << " " << xf.at(2,1) << " " << xf.at(2,2) << " " 
    << xf.at(2,3) << std::endl;
  return os;
}

#endif

typedef XFormT<double> XFormd;
typedef XFormT<float> XFormf;

#endif
