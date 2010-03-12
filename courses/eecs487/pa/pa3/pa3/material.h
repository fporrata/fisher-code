#ifndef _MATERIAL_H
#define _MATERIAL_H

///
/// Material properties class from the shading model in Chapter 9 of the book:
/// Fundamentals of Computer Graphics, 2nd Edition, by Peter Shirley et al.
/// 
struct MaterialT {
   MaterialT() {}

   MaterialT(XVec3f _ca, XVec3f _cr, XVec3f _cp, float _p) 
      : m_ca(_ca), m_cr(_cr), m_cp(_cp), m_p(_p)
      {}

   MaterialT(const MaterialT& _mat)
      : m_ca(_mat.m_ca), m_cr(_mat.m_cr), m_cp(_mat.m_cp), m_p(_mat.m_p)
      {}

   XVec3f m_ca;  // global ambient light color
   XVec3f m_cr;  // diffuse (and ambient) reflectance
   XVec3f m_cp;  // specular reflectance
   float m_p;   // "shininess"
};

#endif
