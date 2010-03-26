#include <iostream>

extern "C" {
#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif
}

#include "sphere.h"
#include "point.h"

// function prototypes

void display(void); // see raytrace.cc
void reshape(int w, int h);
void init(void);
void kb(unsigned char key, int x, int y);

int screen_width=640;
int screen_height=400;


int main(int argc, char *argv[])
{
  // glut functions to create the window and viewport
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);
  glutInitWindowSize(screen_width, screen_height);
  glutCreateWindow("Ray Tracer");
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
	glutKeyboardFunc(kb);
  glutMainLoop();
  return 0;
}


void init(void)
{
  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);
}


void reshape(int w, int h)
{
  glClearColor(0,0,0,0);
  glClear(GL_COLOR_BUFFER_BIT);
  screen_width = w;
  screen_height = h;
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  gluOrtho2D(0,(GLdouble)w, 0, (GLdouble)h);
}
