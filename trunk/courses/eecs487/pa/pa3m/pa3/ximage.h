#ifndef _XIMAGE_H
#define _XIMAGE_H

#include <assert.h>

#include "xvec.h"

// Image class
template<class IPixelT>
class XImageT 
{
public:
  /// construct an image, possibly using a provided memory buffer
  XImageT(size_t _nx=0, size_t _ny=0, IPixelT* _p=0) 
    : m_nx(_nx), m_ny(_ny), m_p(_p), m_allocated(false)
  {}

  /// allocate an image buffer and setup the dimensions
  bool Allocate(size_t _nx, size_t _ny) {
    if(m_p)
      return false;
    m_nx = _nx;
    m_ny = _ny;
    m_p = new IPixelT[m_nx*m_ny];
    assert(m_p);
    m_allocated = true;
    return true;
  }

  bool Free() {
    if(m_p==0)
      return false;
    if(m_allocated) {
      delete[] m_p;
      m_allocated = false;
    }
    m_p = 0;
    return true;
  }

  void Set(size_t _nx, size_t _ny, IPixelT* _p) {
    m_nx = _nx;
    m_ny = _ny;
    m_p = _p;
  }

  ~XImageT() {
    if(m_allocated)
      delete[] m_p;
  }

  XImageT(const XImageT& yim) {
    m_nx = yim.m_nx;
    m_ny = yim.m_ny;
    m_p = yim.m_p;
    m_allocated = false;
  }

  XImageT& operator=(const XImageT& yim) {
    m_nx = yim.m_nx;
    m_ny = yim.m_ny;
    m_p = yim.m_p;
    m_allocated = false;
    return *this;
  }

  bool IsValid() const {
    return m_p!=0;
  }

  const IPixelT& at(int i, int j) const {
    return m_p[i + j*m_nx];
  }

  IPixelT& at(int i, int j) {
    return m_p[i + j*m_nx];
  }

  size_t width() const {
    return m_nx;
  }

  size_t height() const {
    return m_ny;
  }
  
  IPixelT* row_pointer(int y) const {
    return m_p + (m_ny - y - 1) * m_nx;
  }
private:
  size_t m_nx, m_ny;
  IPixelT* m_p;
  bool m_allocated;
};

#endif
