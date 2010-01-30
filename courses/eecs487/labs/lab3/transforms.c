#include <stdio.h>
#include <stdlib.h>

#include "math.h"

/* Include the GL headers */
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <unistd.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433
#endif

/* -- GLUT functions and callbacks -- */

/* Initialize OpenGL state */
void initGL();

/* Paint the window - this is where all the GL action is */
void disp(void);

/* What to do when the window is resized? */
void reshape(int w, int h);

/* Trap and process keyboard events */
void keyb(unsigned char key, int x, int y);

/* Refresh the display, called when system is idle */
void refresh(void);

/* To draw the sun */
void drawSun();

/* To draw the planet */
void drawPlanet();

/* To draw the moon */
void drawMoon();

/* To draw the satellite */
void drawSatellite();

/* --- Global variables for the program --- */

/* screen dimensions */
double width = 640, height = 480;

/* Handle to the GLUT window */
int window_handle;

/* For selection record */
#define BUFFER_SIZE 16
GLuint buffer[BUFFER_SIZE];

/* celestial variables */
double planetRevolve = 0.0;
double planetRotate = 0.0;
double moonRevolve = 0.0;
double moonRotate = 0.0;
double satelliteRevolve = 0.0;
double satelliteRotate = 0.0;

double planetOrbitRadius = 200.0;
double moonOrbitRadius = 80.0;
double satelliteOrbitRadius = 30.0;

/* ------- Actual Code --------- */

void rotateZ(double theta) {
	/* rotate around z-axis by theta
	 in degress */
	/* YOUR CODE HERE */
	double mat[16] = {
	    cos(theta), -sin(theta), 0.0, 0.0,
	    sin(theta), cos(theta), 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		0.0, 0.0, 0.0, 1.0
	};
	
	glMultMatrixd(mat);
}

void translateX(double x) {
	/* translate along x-axis */
	/* YOUR CODE HERE */
	double mat[16] = {
		1.0, 0.0, 0.0, 0.0,
		0.0, 1.0, 0.0, 0.0,
		0.0, 0.0, 1.0, 0.0,
		x, 0.0, 0.0, 1.0
	};
	
	glMultMatrixd(mat);
	//glTranslated(x, 0.0, 0.0);
}

int main(int argc, char *argv[])
{
	/* Initialize GLUT, let it extract command-line GLUT options that
     you may provide 
     - NOTE THE & BEFORE argc
	 */
	glutInit(&argc, argv);
	
	/* Want single window and color will be specified as RGBA values */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	
	/* Set initial window size
     screen_width and screen_height have been above
	 */
	glutInitWindowSize((int)width, (int)height);
	
	/* Create the window and store the handle to it */
	window_handle = glutCreateWindow("Modeling Transformations" /* title */ );
	
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
	
	/* Register the refresh callback for when system is idle */
	glutIdleFunc(refresh);
	
	/* Init OpenGL */
	initGL();
	
	/* Start the GLUT event loop */
	glutMainLoop();
	
	return 0;
}

/* Initialization */
void initGL() {
	/* clear buffer with black */
	glClearColor(0.0, 0.0, 0.0, 1.0);
	
	/* alpha blending */
	glEnable(GL_BLEND);

	glEnable(GL_POLYGON_SMOOTH);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

/* Refresh the display, called when system is idle */
void refresh(void) {
	/* update celestial variables */
	/* YOUR CODE HERE */
	
    planetRevolve += M_PI / 36000;
    planetRotate  += M_PI / 36000;
    moonRevolve += M_PI / 36000;
//    moonRotate = moonRevolve;
    satelliteRevolve += M_PI / 18000;
    satelliteRotate += M_PI / 72000;
	
	glutPostRedisplay();
}

/* Display callback */
void disp(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	
	/* Draw planet orbiting sun, with a satellite and moon
	 orbiting it; a satellite also orbits the moon. Make
	 it so that the 'light' side of the moon ALWAYS faces
	 the planet. */
	/* YOUR CODE HERE */
	
	glPushMatrix();
	drawSun();
	rotateZ(planetRevolve);
	translateX(planetOrbitRadius);

	// Planet
	glPushMatrix();
	rotateZ(planetRotate);
//	glPopMatrix();
	drawPlanet();
	glPopMatrix();
	
	// Planet Satellite
	glPushMatrix();
	rotateZ(satelliteRevolve);
	translateX(satelliteOrbitRadius);
	rotateZ(satelliteRotate);
	drawSatellite();
	glPopMatrix();

	glPushMatrix();
	rotateZ(moonRevolve);
	translateX(moonOrbitRadius);
	
	// Moon
	glPushMatrix();
	rotateZ(moonRotate);
	drawMoon();
	glPopMatrix();
	
	// Moon Satellite
	glPushMatrix();
	rotateZ(satelliteRevolve);
	translateX(satelliteOrbitRadius);
	rotateZ(satelliteRotate);
	drawSatellite();
	glPopMatrix();

	glPopMatrix();
	

	
	glPopMatrix();
	
	
	glutSwapBuffers();
}

/* Keyboard callback */
void keyb(unsigned char key, int x, int y)
{
	switch((char)key) {
		case 'q':
		case 27:
			glutDestroyWindow(window_handle);
			break;
	}
}

/* OpenGL resize callback */

void reshape(int w, int h)
{
	/* Save new screen dimensions */
	width = (GLdouble)w;
	height = (GLdouble)h;
	
	/* Instruct Open GL to use the whole window for drawing */
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	
	/* Do not perform projection transformations - later in the course */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	/* Set coordinate system to first quadrant and bound x and y */
	gluOrtho2D(-width/2.0, width/2.0, -height/2.0, height/2.0);
}

/* To draw the sun */
void drawSun() {
	
	/* sun */
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(0.9, 0.9, 0.9, 1.0);
	glVertex2f(0.0, 0.0);
	
	glColor4f(1.0, 1.0, 0.5, 1.0);
	float i;
	for( i = 0; i < 2*M_PI; i += M_PI/20.0 ) {
		glVertex2f(40.0*cos(i), 40.0*sin(i));
	}
	glVertex2f(40.0, 0.0);
	glEnd();
	
	/* solar corona */
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(1.0, 1.0, 0.5, 0.5);
	glVertex2f(0.0, 0.0);
	
	glColor4f(1.0, 1.0, 0.5, 0.0);
	for( i = 0; i < 2*M_PI; i += M_PI/20.0 ) {
		glVertex2f(80.0*cos(i), 80.0*sin(i));
	}
	glVertex2f(80.0, 0.0);
	glEnd();
}

/* To draw the planet */
void drawPlanet() {
	
	/* blue */
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(0.3, 0.7, 1.0, 1.0);
	glVertex2f(0.0, 0.0);
	
	glColor4f(0.2, 0.6, 1.0, 1.0);
	float i;
	for( i = 0; i < 2*M_PI; i += M_PI/20.0 ) {
		glVertex2f(15.0*cos(i), 15.0*sin(i));
	}
	glVertex2f(15.0, 0.0);
	glEnd();
	
	/* green */
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(0.1, 0.6, 0.2, 1.0);
	glVertex2f(0.0, 0.0);
	
	glColor4f(0.0, 0.4, 0.0, 1.0);
	glVertex2f(10.0, 3.0);
	glVertex2f(8.0, 1.0);
	glVertex2f(9.0, -4.0);
	glVertex2f(0.0, -13.0);
	glVertex2f(-8.0, -11.0);
	glVertex2f(-10.0, -3.0);
	glVertex2f(-6.0, 0.0);
	glVertex2f(-8.0, 9.0);
	glVertex2f(3.0, 10.0);
	glVertex2f(10.0, 3.0);
	glEnd();
	
}

/* To draw the moon */
void drawMoon() {
	
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(0.4, 0.4, 0.4, 1.0);
	glVertex2f(0.0, 0.0);
	
	glColor4f(0.2, 0.2, 0.2, 1.0);
	float i;
	for( i = 0; i < 2*M_PI; i += M_PI/20.0 ) {
		if( i > M_PI/2.0 )
			glColor4f(0.6, 0.6, 0.6, 1.0);
		if( i > M_PI/2.0*3.0 )
			glColor4f(0.2, 0.2, 0.2, 1.0);
		glVertex2f(12.0*cos(i), 12.0*sin(i));
	}
	glVertex2f(12.0, 0.0);
	glEnd();
}

/* To draw the satellite */
void drawSatellite() {
	
	glBegin(GL_QUADS);
	glColor4f(0.6, 0.0, 0.6, 1.0);
	glVertex2f(-3.0, -6.0);
	glVertex2f(-3.0, 6.0);
	glVertex2f(-8.0, 6.0);
	glVertex2f(-8.0, -6.0);
	
	glColor4f(0.8, 0.0, 0.8, 1.0);
	glVertex2f(3.0, -6.0);
	glVertex2f(3.0, 6.0);
	glVertex2f(8.0, 6.0);
	glVertex2f(8.0, -6.0);
	
	glColor4f(0.0, 0.0, 0.8, 1.0);
	glVertex2f(3.0, 2.0);
	glVertex2f(3.0, -2.0);
	glVertex2f(-3.0, -2.0);
	glVertex2f(-3.0, 2.0);
	
	glEnd();
}
