#ifdef WIN32
#include <windows.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "main.h"

int window_handle;

GLdouble screen_width = 1280;
GLdouble screen_height = 800;

GLfloat points[NUM_POINTS][4]; /* Points forming the curve */

/* flags to show various interpolants/approximants */
int show_linear = 1;
int show_catmull_rom = 1;
int show_bezier = 1;

/* Which point has the user currently grabbed? */
int which_point = -1;


int main(int argc, char *argv[])
{
  init();
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(screen_width, screen_height);
  window_handle = glutCreateWindow("Experiment with splines");
  glutDisplayFunc(disp);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutPassiveMotionFunc(passive_motion);
  glutKeyboardFunc(keyb);
  glutIdleFunc(refresh_display);
  glutMainLoop();
  return 0;
}


void print_help(void)
{
  printf("\n");
  printf("Drag the mouse to move the points around\n");
  printf("'1' toggles linear interpolant\n");
  printf("'2' toggles Catmull-Rom interpolant\n");
  printf("'3' toggles Bezier curve approximant\n");
  printf("'?' prints this help message\n");
  printf("'q' or <ESC> quits program\n");
  printf("\n");
}


void init(void)
{
  int i;
  GLfloat y = screen_height / 2;
  GLfloat x = 0.1 * screen_width;
  GLfloat dx = 0.8*screen_width / (GLfloat)(NUM_POINTS-1);
  points[0][0] = x;
  points[0][1] = y;
  for(i=1; i<NUM_POINTS; ++i) {
    x += dx;
    points[i][0] = x;
    points[i][1] = y;
  }

  print_help();
}

void keyb(unsigned char key, int x, int y)
{
  switch((char)key) {
  case '1':
    show_linear = 1-show_linear;
    if(show_linear) printf("Displaying linear interpolant\n");
    else printf("Hiding linear interpolant\n");
    glutPostRedisplay();
    break;

  case '2':
    show_catmull_rom = 1-show_catmull_rom;
    if(show_catmull_rom) printf("Displaying Catmull-Rom interpolant\n");
    else printf("Hiding Catmull-Rom interpolant\n");
    glutPostRedisplay();
    break;

  case '3':
    show_bezier = 1 - show_bezier;
    if(show_bezier) printf("Displaying bezier approximant\n");
    else printf("Hiding bezier approximant\n");
    glutPostRedisplay();
    break;

  case '?':
    print_help();
    break;

  case 'q':
  case 27:
    glutDestroyWindow(window_handle);
    break;
  }
}


void disp(void)
{
  glClear(GL_COLOR_BUFFER_BIT);
  draw_points();
  if(show_linear) draw_linear();
  if(show_catmull_rom) draw_catmull_rom();
  if(show_bezier) draw_bezier();
  glutSwapBuffers();
}


void reshape(int w, int h)
{
  glClearColor(1.0, 1.0, 1.0, 0.0);
  screen_width = (GLdouble) w;
  screen_height = (GLdouble) h;
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, screen_width, 0, screen_height);
}


int find_closest_point(int x, int y)
{
  int i;
  for(i=0; i<NUM_POINTS; ++i) {
    if(abs(points[i][0]-x) < 10 && abs(points[i][1]-y) < 10) return i;
  }
  return -1;
}


void passive_motion(int x, int y)
{
  static int cur_cursor = GLUT_CURSOR_LEFT_ARROW;

  /* which_point contains the point that the mouse is closest to */
  /* -1 if not close to any point */
  which_point = find_closest_point(x,y);

  if(which_point < 0 && cur_cursor == GLUT_CURSOR_CROSSHAIR) {
    glutSetCursor(GLUT_CURSOR_LEFT_ARROW);
    cur_cursor = GLUT_CURSOR_LEFT_ARROW;
  }

  else if(which_point >=0 && cur_cursor == GLUT_CURSOR_LEFT_ARROW) {
    glutSetCursor(GLUT_CURSOR_CROSSHAIR);
    cur_cursor = GLUT_CURSOR_CROSSHAIR;
  }
}

int mouse_prev_x;
int mouse_prev_y;


/* Mouse drag callback */
void motion(int x, int y)
{
  if(which_point >= 0) {
    points[which_point][0] += (x - mouse_prev_x);
    points[which_point][1] += (y - mouse_prev_y);
    mouse_prev_x = x;
    mouse_prev_y = y;
  }
}


/* Mouse click callback */
void mouse(int button, int state, int x, int y)
{
  /* left-click */
  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && which_point>=0) {
    mouse_prev_x = x;
    mouse_prev_y = y;
  }
}


void refresh_display(void)
{
  static struct timeval last_time;
  struct timeval present_time;
  struct timezone tz;
  double last_time_d;
  double present_time_d;

  gettimeofday(&present_time, &tz);
  last_time_d = (double)last_time.tv_sec + 1.0e-6*(double)last_time.tv_usec;
  present_time_d = (double)present_time.tv_sec + 1.0e-6*(double)present_time.tv_usec;
  if( (present_time_d - last_time_d) > 0.1) {
    last_time.tv_sec = present_time.tv_sec;
    last_time.tv_usec = present_time.tv_usec;
    glutPostRedisplay();
  }

}
