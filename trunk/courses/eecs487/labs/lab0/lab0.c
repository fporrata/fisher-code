/*
 * Copyright (c) 2009 University of Michigan, Ann Arbor.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of Michigan, Ann Arbor. The name of the University 
 * may not be used to endorse or promote products derived from this 
 * software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 *
 * This lab consists of the following tasks:
 *   1. Draw two points on the screen
 *   2. Connect the two points into a line
 *   3. Change the line to 3-pixel thick
 *   4. Draw filled squares at both end points:
 *      - perturb coordinates
 *   5. Draw a thin line at the x- and y-axes, i.e., thru (0,0)
 *   6. Register the provided refresh() function that shifts the coordinate 
 *      system by (-1, -1) every 100 ms when the syste is idle
 *   7. Add keyboard command `b' to draw outline squares at both end points
 *   8. Draw a filled square around the Origin
 * 
 * Authors: Manoj Rajagopalan, Sugih Jamin
*/
#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#define NENDS 2           /* number of end "points" to draw */
#define ENDHWIDTH 5       /* half width of end "points," in pixels */
 
GLdouble width, height;   /* window width and height */
int wd;                   /* GLUT window handle */
int ends[NENDS][2];       /* array of 2D points */
int lastx, lasty;         /* where was the mouse last clicked? */
int end;                  /* which of the above points was
                             the last mouse click close to?
                             -1 if neither */
int drift;                /* whether to drift the coordinate system */
double offset=0.0;

/* Program initialization NOT OpenGL/GLUT dependent,
   as we haven't created a GLUT window yet */
void
init(void)
{
  drift = 1;
  width  = 1280.0;                 /* initial window width and height, */
  height = 800.0;                  /* within which we draw. */
  ends[0][0] = (int)(0.25*width);  /* (0,0) is the lower left corner */
  ends[0][1] = (int)(0.75*height);
  ends[1][0] = (int)(0.75*width);
  ends[1][1] = (int)(0.25*height);
}

/* Callback functions for GLUT */

/* Draw the window - this is where all the GL actions are */
void
disp(void)
{
  int i;

  /* clear the screen to white */

     /* set the clearing color (color to clear to) to white (default black) */
  glClearColor(1.0, 1.0, 1.0, 0.0);
     /* actually perform clearing of the color buffer */
  glClear(GL_COLOR_BUFFER_BIT);

  /* draw a black line of width 3 pixels
     between the two points stored in the ends[] global array */

     /* set the drawing color to black (default white) */
  glColor3f(0.0, 0.0, 0.0);
     /* Task 3: set line thickness to 3 pixels (default 1) */
  glLineWidth(3.0);
     /* draw the line */

  glBegin(GL_POINTS); /* Task 1: draw two points */
  for (i = 0; i < NENDS; ++i) {
      glVertex2iv(ends[i]);
  }
  glEnd();
  
  glBegin(GL_LINES); /* Task 2: draw line */
    for (i = 0; i < NENDS; ++i) {
      glVertex2iv(ends[i]);
    }
  glEnd();

  /* Task 4: draw a red square of width 2*ENDHWIDTH pixels
     around each ends[] to highlight the end points. */

     /* set the squares' color to red */
     /* form quadrilaterals around each ends[]
        - each quad takes a group of 4 points */
  glLineWidth(1.0);
  glBegin(GL_QUADS);
  glColor3f(1.0, 0.0, 0.0);
  for (i = 0; i < NENDS; ++i) {
      glVertex2i(ends[i][0] - ENDHWIDTH, ends[i][1] - ENDHWIDTH);
      glVertex2i(ends[i][0] + ENDHWIDTH, ends[i][1] - ENDHWIDTH);
      glVertex2i(ends[i][0] + ENDHWIDTH, ends[i][1] + ENDHWIDTH);
      glVertex2i(ends[i][0] - ENDHWIDTH, ends[i][1] + ENDHWIDTH);
  }
  glEnd();

  /* Task 5: draw a thin (1 pixel) green line on the x- and y-axes */
  glBegin(GL_LINES);
  glColor3f(0.0, 1.0, 0.0);
  glVertex2i(0, offset);
  glVertex2i(0, offset+height);
  glVertex2i(offset, 0);
  glVertex2i(offset+width, 0);
  glEnd();
  /* Task 8: draw a 10x10 blue filled square around the Origin. */
  glPushAttrib(GL_POLYGON_BIT);
  glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  glColor3f(0.0, 0.0, 1.0);
  glBegin(GL_QUADS);
  glVertex2i(-5, -5);
  glVertex2i(5, -5);
  glVertex2i(5, 5);
  glVertex2i(-5, 5);
  glEnd();
  glPopAttrib();
  /* force drawing to start */
  glFlush();
}

/* Called when window is resized,
   also when window is first created,
   before the first call to disp(). */
void
reshape(int w, int h)
{
  /* save new screen dimensions */
  width = (GLdouble) w;
  height = (GLdouble) h;

  /* tell OpenGL to use the whole window for drawing */
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);

  /* do an orthographic parallel projection with the coordinate
     system set to first quadrant, limited by screen/window size */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0.0, width, 0.0, height);
}

/* Refresh the display, called when system is idle */
void
refresh(void)
{
  static struct timeval prev;
  struct timeval now;
  double tprev;
  double tnow;

#ifdef WIN32
  SYSTEMTIME st;
  
  GetSystemTime(&st);
  now.tv_sec = st.wSecond;
  now.tv_usec = st.wMilliseconds*1000;
#else
  gettimeofday(&now, NULL);
#endif

  tprev = (double)prev.tv_sec + 1.0e-6*(double)prev.tv_usec;
  tnow = (double)now.tv_sec + 1.0e-6*(double)now.tv_usec;
  if ((tnow - tprev) > 0.1) {
    prev.tv_sec = now.tv_sec;
    prev.tv_usec = now.tv_usec;

    if (drift) {
      /* shift the coordinate system by (-1, -1) every 100 ms */
      offset -= 1.0;
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      gluOrtho2D(offset, width+offset, offset, height+offset);
    }     

    glutPostRedisplay();
  }
}

/* Trap and process keyboard events */
void
kbd(unsigned char key, int x, int y)
{
  switch((char)key) {
  case 'q':
  case 27:    /* ESC */
    glutDestroyWindow(wd);
    exit(0);
    break;
  case 's':
    drift = 0;
    break;
  case 'b':   /* Task 7: draw outline squares at both end points */
    /* draw only the boundary/edges of the end squares, 1 pixel wide. */
       /* set edge line width to 1 pixel */
       /* set front facing polygon to draw only line, no fill */
       /* redisplay */
      glPolygonMode(GL_FRONT, GL_LINE);
      glutPostRedisplay();
    break;
  }
}

int
find_closest_end(int x, int y)
{
  int n;
  for (n = 0; n < NENDS; ++n) {
    if (abs(x - ends[n][0]) <= ENDHWIDTH && abs(y - ends[n][1]) <= ENDHWIDTH) {
      return (n);
    }
  }
  
  return (-1);
}

/* Trap and process "mouse moved and button NOT pressed" events */
void
move(int x, int y)
{
  static int cursor = GLUT_CURSOR_LEFT_ARROW;
  int ncursor;

  end = find_closest_end(x+(int)offset, (int)height - y+(int)offset);

  ncursor = (end < 0) ? GLUT_CURSOR_LEFT_ARROW : GLUT_CURSOR_CROSSHAIR;
  if (cursor != ncursor) {
    cursor = ncursor;
    glutSetCursor(cursor);
  }

  return;
}

/* Trap and process mouse click (down and up) events */
void
click(int button, int state, int x, int y)
{
  /* Trap left-clicks */
  if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN && end >= 0) {
    lastx = x;
    lasty = y;
  }

  return;
}

/* Trap and process "mouse moved with button pressed" events */
void
drag(int x, int y)
{
  if (end >= 0) {
    ends[end][0] += (x - lastx);
    ends[end][1] -= (y - lasty);
    lastx = x;
    lasty = y;
    glutPostRedisplay();
  }

  return;
}

int
main(int argc, char *argv[])
{
	/* perform initialization NOT OpenGL/GLUT dependent,
       as we haven't created a GLUT window yet */
  init();
  
	/* initialize GLUT, let it extract command-line 
       GLUT options that you may provide 
       - NOTE THE '&' BEFORE argc */
	glutInit(&argc, argv);

	/* specify the display to be single 
       buffered and color as RGBA values */
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

  /* set the initial window size */
  glutInitWindowSize((int) width, (int) height);

  /* create the window and store the handle to it */
  wd = glutCreateWindow("Experiment with line drawing" /* title */ );

  /* --- register callbacks with GLUT --- */

  /* register function that draws in the window */
  glutDisplayFunc(disp);

  /* register function to handle window resizes */
  glutReshapeFunc(reshape);

  /* Task 6: register the provided refresh() 
     function to call when system is idle */
  glutIdleFunc(refresh);
  /* register keyboard event processing function */
  glutKeyboardFunc(kbd);

  /* register mouse-moved movement event callback */
  glutPassiveMotionFunc(move);

  /* register mouse-click event callback */
  glutMouseFunc(click);
  /* register mouse-drag event callback */  
  glutMotionFunc(drag);

  /* start the GLUT main loop */
  glutMainLoop();

  return 0;
}
