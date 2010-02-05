// Shaders support code. Generally, you should not modify this file unless
// you know what you are doing. Change the shader files (*.vs and *.fs) to 
// complete the assignment.

#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else 
#include <GL/glut.h>
#endif

using namespace std;

// Reads the shader source code from the file into a string.
static bool ReadProgram(string& s, const string& name) {
  ifstream ist(name.c_str());

  if(!ist.good())
    return false;

  s.clear();

  while(ist.good()) {
    string line;
    getline(ist, line);
    s += line;
    s += "\n";
  }

  return true;
}


// Compiles the shader, returning false on errors.
bool CompileShader(GLhandleARB hso, string& src) {
  const char* str = src.c_str();
  GLint len = (int)src.length();
  glShaderSourceARB(hso, 1, &str, &len);
  glCompileShaderARB(hso);
  GLint compiled;
  glGetObjectParameterivARB(hso, GL_OBJECT_COMPILE_STATUS_ARB, &compiled);
  if(!compiled) {
    cerr << "could not compile shader:" << endl;
    char log[2048];
    GLint loglen;
    glGetInfoLogARB(hso, 2047, &loglen, log);
    cerr << log << endl;
    return false;
  }
  return true;
}


// Load shaders name.vs and name.fs and link them into the program object.
bool InitShaders(const char* name, GLhandleARB* hpo) {
  if(!hpo)
    return false;

  bool okay = true;
  if(GLEW_ARB_shader_objects && GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader) {
    cerr << "init " << name << endl;

    if(*hpo==0)
      *hpo = glCreateProgramObjectARB();

    GLhandleARB hvso, hfso;
    hvso = glCreateShaderObjectARB(GL_VERTEX_SHADER_ARB);
    hfso = glCreateShaderObjectARB(GL_FRAGMENT_SHADER_ARB);

    string src;
    string vsname = name + string(".vs");
    if(ReadProgram(src, vsname)) {
      CompileShader(hvso, src);
    } else {
      cerr << "could not read vertex shader " << vsname << endl;
    }
    
    string fsname = name + string(".fs");
    if(ReadProgram(src, fsname)) {
      CompileShader(hfso, src);
    } else {
      cerr << "could not read fragment shader " << fsname << endl;
    }

    glAttachObjectARB(*hpo, hvso);
    glAttachObjectARB(*hpo, hfso);

    glLinkProgramARB(*hpo);
    GLint linked;
    glGetObjectParameterivARB(*hpo, GL_OBJECT_LINK_STATUS_ARB, &linked);
    if(!linked) {
      cerr << "could not link " << name << " shaders" << endl;
    }
    
#if 0
    glDetachObjectARB(*hpo, hvso);
    glDetachObjectARB(*hpo, hfso);
    glDeleteObjectARB(hvso);
    glDeleteObjectARB(hfso);
#endif
  } else {
    okay = false;
    cerr << "shader objects not supported in full." << endl;
  }
  return okay;
}

