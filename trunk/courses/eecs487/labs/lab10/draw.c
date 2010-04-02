#ifdef WIN32
	#include <windows.h>
#endif

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif

#include "main.h"
#include "spline.h"


extern GLfloat points[NUM_POINTS][4];
extern GLdouble screen_height;

void draw_points(void)
{
	int i;
	int h = (int)screen_height;
	/* Set your y coordinate to be (h-y) */
	
	glColor4f(1.0, 0.0, 1.0, 1.0);
	
	/* Highlight points as filled squares */
	glBegin(GL_QUADS);
	for(i=0; i<NUM_POINTS; ++i) {
		glVertex2f(points[i][0]-10, (h-points[i][1])-10);
		glVertex2f(points[i][0]-10, (h-points[i][1])+10);
		glVertex2f(points[i][0]+10, (h-points[i][1])+10);
		glVertex2f(points[i][0]+10, (h-points[i][1])-10);
	}
	glEnd();
}

void draw_linear(void)
{
	int i;
	/* the y-coordinate will be (h-y) */
	int h = (int) screen_height;
	
	/* Enable stippling on the linear segment */
	glEnable(GL_LINE_STIPPLE);
	
	/* TODO */
	/* Draw line segments with points (x, h-y) */
	
	
	/* Disable the dotted-line effect for drawing */
	glDisable(GL_LINE_STIPPLE);
}

void draw_catmull_rom(void)
{
	/* the y-coordinate will be (h-y) */
	int h = (int) screen_height;
	
	/* TODO */
	/* Draw line segments with points (x, h-y) */
	
}

void draw_bezier(void)
{
	/* the y-coordinate will be (h-y) */
	int h = (int) screen_height;
	
	/* TODO */
	/* Draw line segments with points (x, h-y) */
	
}
