#include "loadscene.h"
#include "camera.h"

#include "ball.h"
#include "triangle.h"
#include "mesh.h"
#include "cylinder.h"


#include <iostream>
#include <fstream>
#include <string>
#include <stack>

#include "xform.h"
#include "xvec.h"

#ifdef _MSC_VER
const char* dirname(const char* path);
#else
#include "libgen.h"
#endif

using namespace std;

bool EqualStr(const string& line, const char* sz) {
  return line.compare(0, strlen(sz), sz)==0;
}

/// state to be stored in a stack. currently includes only 
/// material color properties.
struct gstate_t {
  MaterialT m_mat;
};

/// Reads the scene file and constructs the scene and fills the parameters
/// of the camera and global rendering options
bool LoadScene(SceneT& scene, CameraT& cam, OptionsT& options, 
               const char* filename) {
  ifstream ist(filename);

  string line;

  if(ist.is_open()) {
    getline(ist, line);
    if(line.compare(0,6,"sce1.0")!=0) {
      cerr << "WARNING: wrong file format" << endl;
      return false;
    }
  } else {
    cerr << "WARNING: could not open file: " << filename << endl;
    return false;
  }

  
  char pathcopy[1024];
  strncpy(pathcopy, filename, 1023);
  const char* dir = dirname(pathcopy);

  char dummy[255];

  stack<gstate_t> states;
  states.push(gstate_t());

  states.top().m_mat.m_ca = XVec3f(0.1f, 0.1f, 0.1f);
  states.top().m_mat.m_cr = XVec3f(1.0f, 1.0f, 1.0f);
  states.top().m_mat.m_cp = XVec3f(0.0f, 0.0f, 0.0f);
  states.top().m_mat.m_p = 1.0f;

  stack<XFormf> xforms;
  xforms.push(XFormf::Identity());

  while(ist.good()) {
    getline(ist, line);
    string::size_type st = line.find_first_not_of(" \t");
    if(st==string::npos)
      continue;
    line = line.substr(st);
    if(line.empty())
      continue;
    if(line[0]=='#')
      continue;
    if(EqualStr(line, "ball")) {
      float radius;
      XVec3f center;
      int nread = sscanf(line.c_str(), "%s %f %f %f %f", dummy, &radius, 
        &center.x(), &center.y(), &center.z());
      if(nread!=5) {
        cerr << "ERROR: " << line << endl;
        continue;
      }
      BallT* ball = new BallT(radius, center, xforms.top());
      ball->m_mat = states.top().m_mat;
      scene.AddGel(ball);
    } else if(EqualStr(line, "cylinder")) {

      // YOUR CODE HERE
      // construct a cylinder and add it to the scene
			CylinderT* cylinder = new CylinderT(xforms.top());
			cylinder->m_mat = states.top().m_mat;
			scene.AddGel(cylinder);
    } else if(EqualStr(line, "object_flat")) {
      char buf[1024];
      int nread = sscanf(line.c_str(), "%s %s", dummy, buf);
      if(nread!=2) {
        cerr << "ERROR: " << line << endl;
        continue;
      }
      string mesh_path = string(dir) + "/" + string(buf);
      cerr << "flat mesh: " << mesh_path << endl;
      MeshT* mesh = new MeshT(xforms.top());
      mesh->SetShading(MeshT::FLAT_SHADE);
      bool success = mesh->Load(mesh_path.c_str());
      if(!success) {
        delete mesh;
        cerr << "ERROR: could not load mesh obj file " << buf << endl;
      }
      mesh->m_mat = states.top().m_mat;
      scene.AddGel(mesh); 
    } else if(EqualStr(line, "object_phong")) {
      char buf[1024];
      int nread = sscanf(line.c_str(), "%s %s", dummy, buf);
      if(nread!=2) {
        cerr << "ERROR: " << line << endl;
        continue;
      }
      string mesh_path = string(dir) + "/"  + string(buf);
      cerr << "phong mesh: " << mesh_path << endl;
      MeshT* mesh = new MeshT(xforms.top());
      mesh->SetShading(MeshT::PHONG_SHADE);
      bool success = mesh->Load(mesh_path.c_str());
      if(!success) {
        delete mesh;
        cerr << "ERROR: could not load mesh obj file " << buf << endl;
      }
      mesh->m_mat = states.top().m_mat;
      scene.AddGel(mesh); 
    } else if(EqualStr(line, "triangle")) {
      XVec3f pts[3];
      int nread = sscanf(line.c_str(), "%s %f %f %f %f %f %f %f %f %f", dummy, 
        &pts[0].x(), &pts[0].y(), &pts[0].z(),
        &pts[1].x(), &pts[1].y(), &pts[1].z(),
        &pts[2].x(), &pts[2].y(), &pts[2].z()
        );
      if(nread!=10) {
        cerr << "ERROR: " << line << endl;
        continue;
      }
      for(int k=0; k<3; ++k)
        pts[k] = xforms.top().apply(pts[k]);

      TriangleT* t = new TriangleT(pts[0], pts[1], pts[2]);
      t->m_mat = states.top().m_mat;
      scene.AddGel(t); 
    } else if(EqualStr(line, "pointlight")) {
      XVec3f center;
      XVec3f color;
      int nread = sscanf(line.c_str(), "%s %f %f %f %f %f %f", dummy, 
        &center.x(), &center.y(), &center.z(),
        &color.x(), &color.y(), &color.z());
      if(nread!=7) {
        cerr << "ERROR: " << line << endl;
        continue;
      }
      center = xforms.top().apply(center);
      PointLightT* light = new PointLightT(center, color);
      scene.AddLight(light);
    } else if(EqualStr(line, "arealight")) {
      float width;
      XVec3f color;
      int nread = sscanf(line.c_str(), "%s %f %f %f %f", dummy, 
        &width,
        &color.x(), &color.y(), &color.z());
      if(nread!=5) {
        cerr << "ERROR: " << line << endl;
        continue;
      }
      XVec3f corner = xforms.top().apply(XVec3f(-width, -width, 0));
      XVec3f veca = xforms.top().applyv(XVec3f(2*width, 0, 0));
      XVec3f vecb = xforms.top().applyv(XVec3f(0, 2*width, 0));

      // YOUR CODE HERE
      // construct the area light and add it to the scene
      // scene.AddLight(light);
			AreaLightT *light = new AreaLightT(corner, veca, vecb, color);
			scene.AddLight(light);
    } else if(EqualStr(line, "shininess")) {
      float c;
      int nread = sscanf(line.c_str(), "%s %f", dummy, &c);
      if(nread!=2) {
        cerr << "ERROR: " << line << endl;
      }
      states.top().m_mat.m_p = c;
    } else if(EqualStr(line, "cr")) {
      XVec3f c;
      int nread = sscanf(line.c_str(), "%s %f %f %f", dummy, &c.x(), &c.y(), &c.z());
      if(nread!=4) {
        cerr << "ERROR: " << line << endl;
      }
      states.top().m_mat.m_cr = c;
    } else if(EqualStr(line, "cp")) {
      XVec3f c;
      int nread = sscanf(line.c_str(), "%s %f %f %f", dummy, &c.x(), &c.y(), &c.z());
      if(nread!=4) {
        cerr << "ERROR: " << line << endl;
      }
      states.top().m_mat.m_cp = c;
    } else if(EqualStr(line, "ca")) {
      XVec3f c;
      int nread = sscanf(line.c_str(), "%s %f %f %f", dummy, &c.x(), &c.y(), &c.z());
      if(nread!=4) {
        cerr << "ERROR: " << line << endl;
      }
      states.top().m_mat.m_ca = c;
		} else if(EqualStr(line, "alpha")) {
			float c;
			int nread = sscanf(line.c_str(), "%s %f", dummy, &c);
			if (nread!=2) {
				cerr << "ERROR: " << line << endl;
			}
			states.top().m_mat.m_alpha = c;
		} else if(EqualStr(line, "ri")) {
			float c;
			int nread = sscanf(line.c_str(), "%s %f", dummy, &c);
			if (nread != 2) {
				cerr << "ERROR: " << line << endl;
			}
			states.top().m_mat.m_ri = c;
    } else if(EqualStr(line, "background")) {
      XVec3f c;
      int nread = sscanf(line.c_str(), "%s %f %f %f", dummy, &c.x(), &c.y(), &c.z());
      if(nread!=4) {
        cerr << "ERROR: " << line << endl;
      }
      scene.BackgroundColor(c);
    } else if(EqualStr(line, "eyepos")) {
      int nread = sscanf(line.c_str(), "%s %f %f %f", dummy, &cam.m_eyepos.x(), &cam.m_eyepos.y(), &cam.m_eyepos.z());
      if(nread!=4) {
        cerr << "ERROR: " << line << endl;
      }
    } else if(EqualStr(line, "eyedir")) {
      int nread = sscanf(line.c_str(), "%s %f %f %f", 
        dummy, &cam.m_eyedir.x(), &cam.m_eyedir.y(), &cam.m_eyedir.z());
      if(nread!=4) {
        cerr << "ERROR: " << line << endl;
      }
    } else if(EqualStr(line, "eyeup")) {
      int nread = sscanf(line.c_str(), "%s %f %f %f", 
        dummy, &cam.m_eyeup.x(), &cam.m_eyeup.y(), &cam.m_eyeup.z());
      if(nread!=4) {
        cerr << "ERROR: " << line << endl;
      }
    } else if(EqualStr(line, "wdist")) {
      int nread = sscanf(line.c_str(), "%s %f", 
        dummy, &cam.m_wdist);
      if(nread!=2) {
        cerr << "ERROR: " << line << endl;
      }
    } else if(EqualStr(line, "fovy_deg")) {
      int nread = sscanf(line.c_str(), "%s %f", 
        dummy, &cam.m_fovy);
      if(nread!=2) {
        cerr << "ERROR: " << line << endl;
      }
      cam.m_fovy *= float(M_PI)/180;
    } else if(EqualStr(line, "nx")) {
      int nread = sscanf(line.c_str(), "%s %d", 
        dummy, &cam.m_nx);
      if(nread!=2) {
        cerr << "ERROR: " << line << endl;
      }
    } else if(EqualStr(line, "ny")) {
      int nread = sscanf(line.c_str(), "%s %d", 
        dummy, &cam.m_ny);
      if(nread!=2) {
        cerr << "ERROR: " << line << endl;
      }
    } else if(EqualStr(line, "max_recursion")) {
      int nread = sscanf(line.c_str(), "%s %d", 
        dummy, &options.m_max_recursion);
      if(nread!=2) {
        cerr << "ERROR: " << line << endl;
      }
    } else if(EqualStr(line, "aasample")) {
      int nread = sscanf(line.c_str(), "%s %d", 
        dummy, &options.m_aasample);
      if(nread!=2) {
        cerr << "ERROR: " << line << endl;
      }
    } else if(EqualStr(line, "{")) {
      gstate_t cstate = states.top();
      states.push(cstate);
      XFormf cxf = xforms.top();
      xforms.push(cxf);
    } else if(EqualStr(line, "}")) {
      if(states.empty())
        cerr << "ERROR: closing bracket unexpected" << endl;
      xforms.pop();
      states.pop();
    } else if(EqualStr(line, "push_matrix")) {
      XFormf cxf = xforms.top();
      xforms.push(cxf);
    } else if(EqualStr(line, "pop_matrix")) {
      if(states.empty())
        cerr << "ERROR: closing bracket unexpected" << endl;
      xforms.pop();
    } else if(EqualStr(line, "rotate")) {
      XVec3f axis;
      float angle;
      int nread = sscanf(line.c_str(), "%s %f %f %f %f", 
        dummy, &angle, &axis.x(), &axis.y(), &axis.z());
      if(nread!=5) {
        cerr << "ERROR: " << line << endl;
      }
      xforms.top() *= XFormf::Rotation(angle, axis);
    } else if(EqualStr(line, "translate")) {
      XVec3f delta;
      int nread = sscanf(line.c_str(), "%s %f %f %f", 
        dummy, &delta.x(), &delta.y(), &delta.z());
      if(nread!=4) {
        cerr << "ERROR: " << line << endl;
      }
      xforms.top() *= XFormf::Translation(delta);
    } else if(EqualStr(line, "scale")) {
      XVec3f delta;
      int nread = sscanf(line.c_str(), "%s %f %f %f", 
        dummy, &delta.x(), &delta.y(), &delta.z());
      if(nread!=4) {
        cerr << "ERROR: " << line << endl;
      }
      xforms.top() *= XFormf::Scaling(delta.x(), delta.y(), delta.z());
    } else if(EqualStr(line, "end")) {
      cout << "scene read." << endl;
      break;
    } else {
      cerr << "WARNING: line not recognized: " << line << endl;
    }
  }

  cam.Init();

  return true;
}

#ifdef _MSC_VER

// define the dirname function for win32.
string buf_dirname;

const char* dirname(const char* path) {
  string path_copy(path);
  string::size_type last_slash = path_copy.find_last_of("/\\");
  if(last_slash==string::npos)
    buf_dirname.assign(".");
  else
    buf_dirname = path_copy.substr(0, last_slash);
  cerr << buf_dirname << endl;
  return buf_dirname.c_str();
}

#endif

