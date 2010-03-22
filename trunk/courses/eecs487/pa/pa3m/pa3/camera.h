#ifndef _CAMERA_H
#define _CAMERA_H

#include "xvec.h"

#include <iostream>

/*****************************************************************
 * CameraT:
 *
 *   Parameters defining the camera, based on Chapter 7 in
 *   "Fundamentals of Computer Graphics, 2nd Ed.", by Shirley et al.
 *
 *   In particular see section 7.5, p. 173, and Figure 7.14, p. 174.
 *
 *****************************************************************/
class CameraT {
   // eyepos is the origin of eye coords
   // eyedir is negative z-axis
   // eyeup is in the yz plane and its dot product with y is positive
   // then 
   // eyew = - eyedir normalized
   // eyeu = eyeup cross eyez normalized
   // eyev = eyez cross eyex normalized
   //
   // (For more about the meaning of the variables, see below.)
 public:
   CameraT() : m_eyepos(0.0f, 0.0f, 0.0f), 
               m_eyedir(0.0f, 0.0f, -1.0f), 
               m_eyeup(0.0f, 1.0f, 0.0f),
               m_wdist(1.0f), m_fovy(0.25f*float(M_PI)),
               m_nx(100), m_ny(100)
      {
      }
  
   // initializes the camera from the read parameters
   void Init() {
      m_eyew = - m_eyedir;
      m_eyew.normalize();
      m_eyeu = m_eyeup.cross(m_eyew);
      m_eyeu.normalize();
      m_eyev = m_eyew.cross(m_eyeu);
      m_eyev.normalize();
   }
  
 public:
   XVec3f m_eyepos;      // camera location in world space
   XVec3f m_eyedir;      // forward direction (world space)
   XVec3f m_eyeup;       // "up" vector (world space)

   // Orthonormal vectors (in world space) that define eye space:
   XVec3f m_eyeu, m_eyev, m_eyew;

   // Distance to near clipping plane:
   float m_wdist;

   // Field of view (vertically). I.e., the angle (in radians) between
   // the bottom and top of the near plane, seen from the camera.
   float m_fovy;

   // Number of pixels in the output image,
   // horizontally (m_nx) and vertically (m_ny):
   int m_nx, m_ny;
};

#endif
