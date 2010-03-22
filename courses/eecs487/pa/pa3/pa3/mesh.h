#ifndef _MESH_H
#define _MESH_H

#include "gel.h"

#include "xform.h"
#include "xvec.h"

#include <fstream>
#include <iostream>
#include <string>

/// Mesh class 
class MeshT: public IGel, public MaterialT {
public:
  
  /// Shading options
  enum { FLAT_SHADE = 0, PHONG_SHADE = 1 };

  /// Mesh vertex structure
  struct vertex_t {
    vertex_t(const XVec3f& _pos) 
      : m_pos(_pos) 
    {}
    XVec3f m_pos;
    XVec3f m_normal;
  };

  /// Mesh triangle structure
  struct triangle_t {
    triangle_t(const XVec3i& _inds) 
      : m_inds(_inds) 
    {}
    XVec3i m_inds;
    XVec3f m_normal;
  };
public:

  /// Mesh constructor
  MeshT(const XFormf& _xf = XFormf::Identity())
    : m_xf(_xf), m_shade(FLAT_SHADE)
  {
    m_xfi = m_xf.Inverse();
  }

  virtual ~MeshT() {}
  
  /// intersection routine for the mesh object
  virtual bool Intersect(const ray_t& wray, hitinfo_t& hit) const {

    // take care of non-trivial transform here
    // transforming the ray into the coordinate system of the mesh
    ray_t ray(m_xfi.apply(wray.m_origin), m_xfi.applyv(wray.m_dir));

    // YOUR CODE HERE
    // add the bounding volume testing here


    std::vector<triangle_t>::const_iterator ti, timin = m_fs.end();

    const float BIG_FLOAT = float(1e34);
    float tmin = BIG_FLOAT;
    float umin = 0.0f, vmin = 0.0f;
    for(ti=m_fs.begin(); ti!=m_fs.end(); ++ti) {
      float t, u, v;
      if(TriangleT::_intersect(m_vs[(*ti).m_inds[0]].m_pos, 
                               m_vs[(*ti).m_inds[1]].m_pos, 
                               m_vs[(*ti).m_inds[2]].m_pos, 
                               ray, t, u, v)) 
      {
        if(t>0.000001 && t<tmin) {
          tmin = t;
          timin = ti;
          umin = u;
          vmin = v;
        }
      }
    }

    if(tmin==BIG_FLOAT)
      return false;

    hit.m_t = tmin;
    hit.m_pos = 
        (1.0f-umin-vmin)*m_vs[(*timin).m_inds[0]].m_pos 
      + umin*m_vs[(*timin).m_inds[1]].m_pos
      + vmin*m_vs[(*timin).m_inds[2]].m_pos;

    switch(m_shade) {
    case FLAT_SHADE:
      hit.m_normal = (*timin).m_normal;
      break;
    case PHONG_SHADE:
      // YOUR CODE HERE
      // set the phong normal using umin, vmin, and timin
			//*hit.m_normal = (*timin).m_normal;
			hit.m_normal = (1.0f-umin-vmin)*m_vs[(*timin).m_inds[0]].m_normal
      + umin*m_vs[(*timin).m_inds[1]].m_normal
      + vmin*m_vs[(*timin).m_inds[2]].m_normal;
      break;
    }

    hit.m_mat = m_mat;

    hit.m_pos = m_xf.apply(hit.m_pos);
    hit.m_normal = m_xfi.applytv(hit.m_normal);
    hit.m_normal.normalize();

    return true;
  }

  /// Loads the mesh from a simplified OBJ file
  bool Load(const char* filename) {
    using namespace std;
    ifstream ist(filename);    
    string line;
    
    size_t nn = 0;

    while(ist.good()) {
      getline(ist, line);
      if(line.length()<2)
        continue;
      if(line[0]=='v') {
        if(line[1]=='n') {
          // read normal here
          XVec3f normal;
          int nr = sscanf(line.c_str(), "vn %f %f %f", 
            &normal.x(), &normal.y(), &normal.z());
          if(nr!=3) {
            std::cerr << "cannot read normal: " << line << std::endl;
            continue;
          }
          if(nn>=m_vs.size()) {
            std::cerr << "cannot read normal before vertex position" << endl;
            return false;
          }
          normal.normalize();
          m_vs[nn].m_normal = normal;
          ++nn;
        } else {
          // read position here
          XVec3f pos;
          int nr = sscanf(line.c_str(), "v %f %f %f", &pos.x(), &pos.y(), &pos.z());
          if(nr!=3) {
            std::cerr << "cannot read position: " << line << std::endl;
            continue;
          }
          //++nv;
          AddVertex(pos);
        }
      } else if(line[0]=='f') {
        // read face here. only triangular
        XVec3i tri;
        int nr = sscanf(line.c_str(), "f %d %d %d", &tri.x(), &tri.y(), &tri.z());
        if(nr!=3) {
          std::cerr << "cannot read face indices: " << line << std::endl;
          continue;
        }
        tri -= XVec3i(1);
        AddTriangle(tri);
      }
    }
    vector<triangle_t>::iterator ti;
    for(ti=m_fs.begin(); ti!=m_fs.end(); ++ti)
      (*ti).m_normal = TriangleT::_compute_normal(m_vs[(*ti).m_inds[0]].m_pos,
                                                  m_vs[(*ti).m_inds[1]].m_pos,
                                                  m_vs[(*ti).m_inds[2]].m_pos);
    ComputeBV();
    cerr << "read " << (unsigned)m_vs.size() << " verts and " 
      << (unsigned)m_fs.size() << " triangles." << endl;
    return true;
  }

  /// Adds a vertex to the vertex set
  void AddVertex(const XVec3f& pos) {
    m_vs.push_back(vertex_t(pos));
  }
  /// Adds a triangle to the triangle set
  void AddTriangle(const XVec3i& inds) {
    m_fs.push_back(triangle_t(inds));
  }

  /// Initializes bounding volume for the mesh
  void ComputeBV() {
    // YOUR CODE HERE 
    // initialize the bounding sphere here

  }

  /// Set a particular shading mode: flat or phong
  void SetShading(int _shade) {
    m_shade = _shade;
  }

private:
  /// a container of mesh vertices
  std::vector<vertex_t> m_vs;
  /// a container of mesh triangles
  std::vector<triangle_t> m_fs;
public:
  float m_shine;
  XFormf m_xf, m_xfi;

  MaterialT m_mat;
  int m_shade;
};

#endif
