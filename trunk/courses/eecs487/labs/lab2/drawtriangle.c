 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Include the GL headers */
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

/* -- GLUT functions and callbacks -- */

/* Paint the window - this is where all the GL action is */
void disp(void);

/* What to do when the window is resized? */
void reshape(int w, int h);

/* Trap and process keyboard events */
void keyb(unsigned char key, int x, int y);

/* Trap and process mouse-click events */
void mouse(int button, int state, int x, int y); /* click events */


/* Function that draws a 'point' on the square grid of 'pixels' */

void drawpoint(GLint x, GLint y, GLclampf r, GLclampf g, GLclampf b);

#define MAX(a, b) ((a) > (b))?(a):(b)
#define MIN(a, b) ((a) > (b))?(b):(a)
#define COLOR_SIZE 4
#define epsilon 1e-25




/* --- Global variables for the program --- */

/* screen width and height - screen refers to
   window within which we draw*/

#define DELTA 25
GLdouble screen_width=51*DELTA, screen_height=32*DELTA;

/* Where was the mouse last clicked? */
int endpoint1[2] = {-1,-1}, endpoint2[2] = {-1,-1}, endpoint3[2] = {-1,-1};

/* Flag to indicate the number of endpoints that the user has set by clicking */
int num_endpoints=0;

/* Handle to the GLUT window */
int window_handle;

/* Anti-aliasing */
int aa = 0;






/* ------- Actual Code --------- */


int main(int argc, char *argv[])
{
  /* Initialize GLUT, let it extract command-line GLUT options that
     you may provide 
     - NOTE THE & BEFORE argc
  */
  glutInit(&argc, argv);

  /* Want single window and color will be specified as RGBA values */
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  //glEnable (GL_BLEND);
  //glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  /* Set initial window size
     screen_width and screen_height have been above
  */
  glutInitWindowSize((int)screen_width, (int)screen_height);

  /* Create the window and store the handle to it */
  window_handle = glutCreateWindow("Experiment with triangle" /* title */ );


  /* --- register callbacks for GUI events --- */
  /* NOTE prototypes for each, declared above */

  /* Register function that paints the window
     - Calling OpenGL functions or otherwise
  */
  glutDisplayFunc(disp);

  /* Register function to handle window reshapes */
  glutReshapeFunc(reshape);

  /* Register keyboard event processing function */
  glutKeyboardFunc(keyb);

  /* Register mouse-click event callback */
  glutMouseFunc(mouse);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  /* Start the GLUT event loop */
  glutMainLoop();

  return 0;
}


void drawpoint(GLint x, GLint y, GLclampf r, GLclampf g, GLclampf b)
{
  glColor3f(r,g,b);
  glRecti(x*DELTA, y*DELTA, (x+1)*DELTA, (y+1)*DELTA);

  /* NOTE: you cannot call glRect within a glBegin()...glEnd() block */
}

void drawpoint_alpha(GLint x, GLint y, const GLfloat * color)
{
    //glColor4fv(color);
    glColor4f(color[0], color[1], color[2], color[3]);
    //glRecti(x*DELTA, y*DELTA, (x+1)*DELTA, (y+1)*DELTA);
    int x1 = x*DELTA;
    int y1 =  y*DELTA;
    int x2 =  (x+1)*DELTA;
    int y2 =  (y+1)*DELTA;
    glBegin(GL_POLYGON);
    glVertex2i(x1, y1);
    glVertex2i(x2, y1);
    glVertex2i(x2, y2);
    glVertex2i(x1, y2);
    glEnd();
}


/*
int min(GLint x, GLint y) { return x < y ? x : y; }
int max(GLint x, GLint y) { return x > y ? x : y; }
*/

float dist_ratio(const float * p, const float * q, const float * r, const float * m)
{
    float dy = p[1] - q[1];
    float dx = p[0] - q[0];
    float c = p[0] * q[1] - q[0] * p[1];
    float fm = dy * m[0] - dx * m[1] + c;
    float fr = dy * r[0] - dx * r[1] + c;
    
    if (fr == 0)
	if (fm) {
	    return -1;
	}else {
	    return 0;
	}
    else
	return fm / fr;
}

void mul_float_arr_c(float * arr, float scaler, float * dest, int length)
{
    int i = 0;
    while (i < length) {
	dest[i] = arr[i] * scaler;
	++i;
    }
}

void add_float_arr_c(float * arr, float c, float * dest, int length)
{
    int i = 0;
    while (i < length) {
	dest[i] = arr[i] + c;
	++i;
    }
}

void add_two_float_arr(float * a1, float * a2, float * dest, int length)
{
    int i = 0;
    while (i < length) {
	dest[i] = a1[i] + a2[i];
	++i;
    }
}


int contains_point(float * v0, float * v1, float * v2,
		     float * c0, float * c1, float * c2,
		     float * p, float * pcolor)
{
    float gamma = dist_ratio(v0, v1, v2, p);
    float beta = dist_ratio(v0, v2, v1, p);
    //float alpha = 1 - gamma - beta;
    float alpha = dist_ratio(v1, v2, v0, p);
    if (gamma >= - epsilon && alpha >= -epsilon && beta >= -epsilon) {
	float tmpc0[COLOR_SIZE];
	float tmpc1[COLOR_SIZE];
	float tmpc2[COLOR_SIZE];
	mul_float_arr_c(c0, alpha, tmpc0, COLOR_SIZE);
	mul_float_arr_c(c1, beta, tmpc1, COLOR_SIZE);
	mul_float_arr_c(c2, gamma, tmpc2, COLOR_SIZE);
	add_two_float_arr(tmpc0, tmpc1, pcolor, COLOR_SIZE);
	add_two_float_arr(tmpc2, pcolor, pcolor, COLOR_SIZE);
	return 1;
    }
    else {
	float tmp[COLOR_SIZE] = {0};
	memcpy(pcolor, tmp, sizeof(float) * COLOR_SIZE);
	return 0;
    }
}

int contains_point_aa(float * v0, float * v1, float * v2,
		      float * c0, float * c1, float * c2,
		      float * p, float * pcolor)
{
    float subp[4][2] = {{p[0]-0.25, p[1]-0.25},
			{p[0]-0.25, p[1]+0.25},
			{p[0]+0.25, p[1]+0.25},
			{p[0]+0.25, p[1]-0.25}};
    int isin = 0;
    int i;

    float tmpcolor[COLOR_SIZE] = {0};
    memcpy(pcolor, tmpcolor, sizeof(float) * COLOR_SIZE);
    for (i = 0; i < 4; i++) {
	if (contains_point(v0, v1, v2, c0, c1, c2, subp[i], tmpcolor)) {
	    isin++;
	    add_two_float_arr(tmpcolor, pcolor, pcolor, COLOR_SIZE);
	}
    }
    if (isin) {
	mul_float_arr_c(pcolor, 1.0 / isin, pcolor, COLOR_SIZE-1);
	pcolor[3] *= 0.25;
	//printf("%f\n", pcolor[3]);
    }
    
    return isin;
}

/* Display callback */

void disp(void)
{
  int i,j;


  /* Set background to white */
  glClearColor(1.0, 1.0, 1.0, 1.0);
  glClear(GL_COLOR_BUFFER_BIT);

  /* Draw a grey grid */
  glColor3f(0.5, 0.5, 0.5);
  glLineWidth(1.0);
  glBegin(GL_LINES);
  /* vertical lines */
  for(i=0; i<screen_width; i+=DELTA) {
    glVertex2i(i,0);
    glVertex2i(i,screen_height);
  }
  /* horizontal lines */
  for(j=0; j<screen_height; j+=DELTA) {
    glVertex2i(0,j);
    glVertex2i(screen_width, j);
  }
  glEnd();

  /* Fill the 'front' face of polygons - rectanges in our case */
  glPolygonMode(GL_FRONT, GL_FILL); 

  /* Draw the first point (square) in red, if value) */
  if(endpoint1[0]>=0 && endpoint1[1]>=0)
    drawpoint(endpoint1[0], endpoint1[1], 1.0, 0.0, 0.0);

  /* Draw the second point (square) in blue, if value) */
  if(endpoint2[0]>=0 && endpoint2[1]>=0)
    drawpoint(endpoint2[0], endpoint2[1], 0.0, 1.0, 0.0);

  /* Draw the third point (square) in blue, if value) */
  if(endpoint3[0]>=0 && endpoint3[1]>=0)
    drawpoint(endpoint3[0], endpoint3[1], 0.0, 0.0, 1.0);

  /* Draw straight line connecting end points if both are valid*/
  if(num_endpoints == 3) {
    glLineWidth(3.0);
    glColor3f(0.0, 0.0, 0.0);
    glBegin(GL_TRIANGLES);
    glVertex2i(endpoint1[0]*DELTA+DELTA/2, endpoint1[1]*DELTA+DELTA/2);
    glVertex2i(endpoint2[0]*DELTA+DELTA/2, endpoint2[1]*DELTA+DELTA/2);
    glVertex2i(endpoint3[0]*DELTA+DELTA/2, endpoint3[1]*DELTA+DELTA/2);
    glEnd();


  /* TODO - implement the triangle rasterization algorithm here.
     int endpoint1[2], endpoint2[2] and endpoint3[2] store integer coordinates of the vertices.
     The support code does the task of trapping mouse clicks and setting these
     NOTE: Interpolate the color between the endpoints to get a gradual transition
  */
    int xmin = MIN(MIN(endpoint1[0], endpoint2[0]), endpoint3[0]);
    int xmax = MAX(MAX(endpoint1[0], endpoint2[0]), endpoint3[0]);
    int ymin = MIN(MIN(endpoint1[1], endpoint2[1]), endpoint3[1]);
    int ymax = MAX(MAX(endpoint1[1], endpoint2[1]), endpoint3[1]);

    float red[] = {1.0, 0.0, 0.0, 1.0};
    float green[] = {0.0, 1.0, 0.0, 1.0};
    float blue[] = {0.0, 0.0, 1.0, 1.0};
    
    float v0[] = {endpoint1[0]+0.5, endpoint1[1]+0.5};
    float v1[] = {endpoint2[0]+0.5, endpoint2[1]+0.5};
    float v2[] = {endpoint3[0]+0.5, endpoint3[1]+0.5};
    int x, y;
    for (y = ymin; y <= ymax; ++y) {
	for (x = xmin; x <= xmax; ++x) {
	    float p [2] = {x+0.5, y+0.5};
	    float pcolor [4];
	    int isdraw;
	    if (aa)
		isdraw = contains_point_aa(v0, v1, v2, red, green, blue, p, pcolor);
	    else
		isdraw = contains_point(v0, v1, v2, red, green, blue, p, pcolor);
	    if (isdraw) {
		//printf("%f %f %f %f\n", pcolor[0], pcolor[1], pcolor[2], pcolor[3]);
		drawpoint_alpha((int)(p[0]), (int)(p[1]), pcolor);
	    }
	}
    }
  }


  /* Force OpenGL to start drawing */
  glutSwapBuffers();
}




/* Mouse click event callback */
void mouse(int button, int state, int x, int y)
{
  /* Trap left-clicks */
  if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
    if(num_endpoints==0) {
      endpoint1[0] = x/25;
      endpoint1[1] = (screen_height-y)/25;      
      ++num_endpoints;
    }
    else if(num_endpoints==1) {
      endpoint2[0] = x/25;
      endpoint2[1] = (screen_height-y)/25;
      ++num_endpoints;
    }
    else if(num_endpoints==2) {
      endpoint3[0] = x/25;
      endpoint3[1] = (screen_height-y)/25;
      ++num_endpoints;
    }
    else {
      endpoint1[0] = -1;
      endpoint1[1] = -1;
      endpoint2[0] = -1;
      endpoint2[1] = -1;
	  endpoint3[0] = -1;
	  endpoint3[1] = -1;
      num_endpoints=0;
    }
    glutPostRedisplay();
  }
}



/* Keyboard callback */
void keyb(unsigned char key, int x, int y)
{
  switch((char)key) {

  case 'q':
  case 27:
    glutDestroyWindow(window_handle);
    break;

  case ' ':
    endpoint1[0] = -1;
    endpoint1[1] = -1;
    endpoint2[0] = -1;
    endpoint2[1] = -1;
	endpoint3[0] = -1;
	endpoint3[1] = -1;
    glutPostRedisplay();
    break;

  case 'a':
      aa = !aa;
      glutPostRedisplay();
      break;

  }
}


/* OpenGL resize callback */

void reshape(int w, int h)
{
  /* Save new screen dimensions */
  screen_width = (GLdouble) w;
  screen_height = (GLdouble) h;

  /* Instruct Open GL to use the whole window for drawing */
  glViewport(0, 0, (GLsizei) w, (GLsizei) h);

  /* Do not perform projection transformations - later in the course */
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  /* Set coordinate system to first quadrant and bound x and y */
  gluOrtho2D(0.0, screen_width, 0.0, screen_height);
}

