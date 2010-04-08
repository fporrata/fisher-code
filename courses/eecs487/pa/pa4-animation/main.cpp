#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else 
#include <GL/glut.h>
#endif

#include <iostream>
#include <fstream>
#include <string>

#include "x3reader.h"
#include "x3.h"

#ifndef _MSC_VER
#include "libgen.h"
#endif

using namespace std;

// Global constants (image dimensions).
const int WIDTH = 640;
const int HEIGHT = 480;

X3Scene* scene = NULL;

bool rmb_down = false;
bool lmb_down = false;
float zoff_old = 0.0f;
float phi_old = 0.0f;
float theta_old = 0.0f;
XVec2i down_rmb, down_lmb;

// timing parameters: in milliseconds
bool stop_animation = true;
int start_time = 0; 
int current_time = 0;
int one_step = 50; // 20 fps


// Function prototypes.
void DisplayCallback();
void KeyboardCallback(unsigned char key, int x, int y);
void SpecialCallback(int key, int x, int y);
void ResizeCallback(int w, int h);
void MouseCallback(int button, int state, int x, int y);
void MotionCallback(int x, int y);
void IdleCallback();

//void (*func)(void)
void OnExitCallback(void);

int main(int argc, char** argv)
{
  // Register on-exit callback as soon as possible.
  atexit(OnExitCallback);

  if(argc<2) {
    cerr << "Usage: ./pa4 scene.x3d" << endl;
    return -1;
  }

  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH);
  glutInitWindowSize(WIDTH, HEIGHT); 
  glutInitWindowPosition(100, 100);
  glutCreateWindow(argv[0]);

  ifstream input_stream(argv[1]);
  X3Reader x3reader;
  x3reader.set_dirname(dirname(argv[1]));

  // This is called after glutInit so we can setup OpenGL while parsing 
  // the file.
  scene = x3reader.Read(input_stream);

  glutDisplayFunc(DisplayCallback);
  glutKeyboardFunc(KeyboardCallback);
  glutReshapeFunc(ResizeCallback);
  glutMouseFunc(MouseCallback);
  glutMotionFunc(MotionCallback);
  glutSpecialFunc(SpecialCallback);
  if(!stop_animation) {
    glutIdleFunc(IdleCallback);
  }

  //cerr << endl << "Here is your scene: " << endl;
  //scene->Print(cerr, 0);

  glutMainLoop();

  return 0;
}

void OnExitCallback(void) {
  delete scene;
}

// This function is called when window needs to be rendered.
void DisplayCallback() {
  glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  if(scene) {
    // setup the viewpoint cam

    // Setup lights.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    int count_lights = 0;

    scene->SetupLights(&count_lights);

    // cout << count_lights << " lights in the scene." << endl;

    // If there are no lights in the scene file, setup a default light.
    if(count_lights==0) {
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      XVec4f light_pos(1, 1, 0, 1);
      glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
      glEnable(GL_LIGHT0);
    }

    // Render scene.
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glEnable(GL_LIGHTING);
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_NORMALIZE);

    glShadeModel(GL_SMOOTH);

    glColor3f(1.0f, 0.0f, 0.0f);

    //scene->set_time(0.001f*glutGet(GLUT_ELAPSED_TIME));
    if(!stop_animation) {
      current_time = glutGet(GLUT_ELAPSED_TIME) - start_time;
    } 
    scene->set_time(0.001f*current_time);

    scene->Render();
  }
    
  glutSwapBuffers();
}

// This function is called when user presses a key.
void KeyboardCallback(unsigned char key, int x, int y) {
  switch(key) {
    case 32:
      stop_animation = !stop_animation;
      if(stop_animation) {
        // stopped animation -- remember current_time
        current_time = glutGet(GLUT_ELAPSED_TIME) - start_time;
        glutIdleFunc(NULL);
      } else {
        // restarting...
        start_time = glutGet(GLUT_ELAPSED_TIME) - current_time;
        glutIdleFunc(IdleCallback);
      }
      break;
  case 27: // ESC key
    exit(0);
    break;
  }
  glutPostRedisplay();
}

// This function is called when user presses a special key.
void SpecialCallback(int key, int x, int y) {
  switch(key) {
    case GLUT_KEY_HOME:
      if(stop_animation) {
        current_time = 0;
      }
      break;
    case GLUT_KEY_RIGHT:
      if(stop_animation) {
        current_time += one_step;
      }
      break;
    case GLUT_KEY_LEFT:
      if(stop_animation) {
        current_time -= one_step;
      }
      break;
  }
  glutPostRedisplay();
}

void ResizeCallback(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  if(scene) {
    float aspect = float(w) / h;
    gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
  }
}

void MouseCallback(int button, int state, int x, int y) {
  if(!scene)
    return;
  switch(button) {
    case GLUT_LEFT_BUTTON:
      if(state==GLUT_DOWN) {
        lmb_down = true;
        phi_old = scene->viewpoint()->phi();
        theta_old = scene->viewpoint()->theta();
        down_lmb = XVec2i(x, y);
      } else if(state==GLUT_UP){
        lmb_down = false;
      }
      break;
    case GLUT_RIGHT_BUTTON:
      if(state==GLUT_DOWN) {
        rmb_down = true;
        zoff_old = scene->viewpoint()->zoff();
        down_rmb = XVec2i(x, y);
      } else if(state==GLUT_UP){
        rmb_down = false;
      }
      break;
  }
}

void MotionCallback(int x, int y) {
  if(rmb_down) {
    scene->viewpoint()->set_zoff(zoff_old + 0.1f * (y - down_rmb(1)));
  }
  if(lmb_down) {
    scene->viewpoint()->set_phi(phi_old + 0.1f * (x - down_lmb(0)));
    scene->viewpoint()->set_theta(theta_old + 0.1f * (y - down_lmb(1)));
  }
  glutPostRedisplay();
}

void IdleCallback() {
  glutPostRedisplay();
}


