
#include <iostream>
#include <cmath>

extern "C" {
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
	
}

using namespace std;


bool init(const char *const texture_image_filename);
void disp(void);
void reshape(int w, int h);
void keyb(unsigned char key, int x, int y);
void print_help(void);


int window_handle;
GLdouble screen_width=800, screen_height=600;
bool in_texture_mode = true;


int main(int argc, char *argv[])
{
  if(argc != 2) {
    cerr << "Usage: globe <texture_image_filename>.tga" << endl;
    return -1;
  }
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize((int)screen_width, (int)screen_height);
  window_handle = glutCreateWindow("Lab6: The World Ain't Flat! [Textures]");
  if(!init(argv[1])) return -1;
  glutDisplayFunc(disp);
  glutReshapeFunc(reshape);
  glutKeyboardFunc(keyb);
  glutMainLoop();

  return 0;
}



GLfloat position[] = { 0.0, 0.0, 2.0, 1.0 };
GLfloat dir[] = {0.0, 0.0, -1.0};
GLfloat diffuse[] = { 0.8, 0.5, 0.6, 1.0 };
GLfloat specular[] = {0.0, 0.8, 0.0, 1.0 };
GLfloat mat_specular[] = { .7, 1.0, 4.0, 1.0 };
GLfloat mat_shininess[] = { 50.0 };


bool init_lights(void)
{
  cerr << "Initializing lights." << endl;

  /* Lighting and material properties */

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();


  glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

  glLightfv(GL_LIGHT0, GL_POSITION, position);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
  
  glEnable(GL_LIGHT0);
  glEnable(GL_DEPTH_TEST);
  glDisable(GL_LIGHTING);

  /* Initial camera location and orientation */
  gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);

  return true;
}






bool init(const char *const texture_image_filename)
{
  // TODO: initialize texture and texture parameters.
  // TODO: Create sphere-drawing display list. Map texture coordinates as you create the sphere.
  return init_lights();
}



void disp(void)
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // TODO: call sphere-drawing display list.

  glutSwapBuffers();
}




void reshape(int w, int h)
{
  /* Save new screen dimensions */
  screen_width = (GLdouble) w;
  screen_height = (GLdouble) h;

  /* Instruct Open GL to use the whole window for drawing */
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);

  /* Set the perspective - later in this course */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(55, screen_width/screen_height, 0.1, 25);
}


void keyb(unsigned char key, int x, int y)
{
  /* set the matrix mode to model-view */
  glMatrixMode(GL_MODELVIEW);

  /* Get info about Alt, Ctrl, Shift modifier keys */
  int mod;
  mod = glutGetModifiers();

  /* Removing the glLight functions after the modeling/viewing transforms
   * will keep the light position and direction fixed in eye coordinates.
   * Preserving them will make the light change position and direction
   * with the transforms.
   */


  switch((char)key) {

  case 'h':
  case '?':
    print_help();
    break;

  case 'x':
    if(mod & GLUT_ACTIVE_ALT) glRotatef(5,1,0,0);
    else glTranslatef(0.1,0, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, position); 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glutPostRedisplay();
    break;

  case 'X':
    if(mod & GLUT_ACTIVE_ALT) glRotatef(-5,1,0,0);
    else glTranslatef(-0.1,0,0);
    glLightfv(GL_LIGHT0, GL_POSITION, position); 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glutPostRedisplay();
    break;

  case 'y':
    if(mod & GLUT_ACTIVE_ALT) glRotatef(5,0,1,0); 
    else glTranslatef(0,0.1, 0);
    glLightfv(GL_LIGHT0, GL_POSITION, position); 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glutPostRedisplay();
    break;

  case 'Y':
    if(mod & GLUT_ACTIVE_ALT) glRotatef(-5,0,1,0); 
    else glTranslatef(0,-0.1,0);
    glLightfv(GL_LIGHT0, GL_POSITION, position); 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glutPostRedisplay();
    break;

  case 'z':
    if(mod & GLUT_ACTIVE_ALT) glRotatef(5,0,0,1);
    else glTranslatef(0,0,0.1);
    glLightfv(GL_LIGHT0, GL_POSITION, position); 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glutPostRedisplay();
    break;

  case 'Z':
    if(mod & GLUT_ACTIVE_ALT) glRotatef(-5,0,0,1);
    else glTranslatef(0,0,-0.1);
    glLightfv(GL_LIGHT0, GL_POSITION, position); 
    glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
    glutPostRedisplay();
    break;

  case '.':
    in_texture_mode = !in_texture_mode;
    if(in_texture_mode) {
      glDisable(GL_LIGHTING);
      glEnable(GL_TEXTURE_2D); 
    }
    else {
      glEnable(GL_LIGHTING);
      glDisable(GL_TEXTURE_2D);
    }
    glutPostRedisplay();
    break;
    

  case 'q':
  case 'Q':
  case 27:
    glutDestroyWindow(window_handle);
    break;
  }

}


void print_help(void)
{
  cerr << "\nEECS 487 F08 Lab 70: OpenGL Lighting\n";
  cerr << "----------------------------------------\n\n";
  cerr << "Navigate by translating and rotating along the axes" << endl;

  cerr << "x    : Move forward along the x-axis by a distance of 0.1\n";
  cerr << "X    : Move backward along the x-axis by a distance of 0.1\n";
  cerr << "ALT-x: Rotate about x-axis by 5 degrees\n";
  cerr << "ALT-X: Rotate about x-axis by -5 degrees\n";
  
  cerr << "y    : Move forward along the y-axis by a distance of 0.1\n";
  cerr << "Y    : Move backward along the y-axis by a distance of 0.1\n";
  cerr << "ALT-y: Rotate about y-axis by 5 degrees\n";
  cerr << "ALT-Y: Rotate about y-axis by -5 degrees\n";
  
  cerr << "z    : Move forward along the z-axis by a distance of 0.1\n";
  cerr << "Z    : Move backward along the z-axis by a distance of 0.1\n";
  cerr << "ALT-z: Rotate about z-axis by 5 degrees\n";
  cerr << "ALT-Z: Rotate about z-axis by -5 degrees\n\n";
  
  cerr << "  .  : Toggle between lighting and texture modes" << endl;
}

