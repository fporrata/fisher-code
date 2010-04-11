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

#define NUM_SAMPLES 30

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

	glLineStipple(1, 0x00FF);
	glMatrixMode(GL_COLOR);
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f);
	glLineWidth(3.0);
	glBegin(GL_LINE_STRIP);
	for (i = 0; i < NUM_POINTS; i++) {
		glVertex2f(points[i][0], h - points[i][1]);
	}
	glEnd();
	

	glMatrixMode(GL_COLOR);
	glPopMatrix();
	/* Disable the dotted-line effect for drawing */
	glDisable(GL_LINE_STIPPLE);
}

void draw_catmull_rom(void)
{
	/* the y-coordinate will be (h-y) */
	int h = (int) screen_height;
	
	/* TODO */
	/* Draw line segments with points (x, h-y) */
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0f, 0.0f, 1.0f);
	int i, j, k;
	
		GLfloat a[3][4];
			calculate_quad_spline(points, a);
		for (k = 0; k < NUM_SAMPLES/2; k++) {
			GLfloat u = (float)k / NUM_SAMPLES;
			GLfloat node[2];
			node[0] = a[0][0] + a[1][0] * u + a[2][0] * u * u;
			node[1] = a[0][1] + a[1][1] * u + a[2][1] * u * u;
			node[1] = h - node[1];
			glVertex2f(node[0], node[1]);
		}

		GLfloat p [4][4];
		for (i = 0; i < 3; i++) {
			for (j = 0; j < 4; j++) {
				p[i][j] = points[i+5][j];
			}
		}
	glEnd();
	glBegin(GL_LINE_STRIP);
		calculate_quad_spline(p, a);
		for (k = NUM_SAMPLES/2; k < NUM_SAMPLES; k++) {
			GLfloat u = (float)k / NUM_SAMPLES;
			GLfloat node[2];
			node[0] = a[0][0] + a[1][0] * u + a[2][0] * u * u;
			node[1] = a[0][1] + a[1][1] * u + a[2][1] * u * u;
			node[1] = h - node[1];
			glVertex2f(node[0], node[1]);
		}
		glEnd();
		glBegin(GL_LINE_STRIP);
		
	for (i = 0; i < NUM_POINTS - 3; i++) {
		GLfloat a[4][4];
		calculate_catmull_rom_spline(points[i], points[i+1], 
																 points[i+2], points[i+3], a);

		//Draw curve with line segments
		for (k = 0; k < NUM_SAMPLES; k++) {
			GLfloat u = (float)k / NUM_SAMPLES;
			GLfloat node[2];
			node[0] = a[0][0] + a[1][0] * u + a[2][0] * u * u + a[3][0] * u * u * u;
			node[1] = a[0][1] + a[1][1] * u + a[2][1] * u * u + a[3][1] * u * u * u;
			node[1] = h - node[1];
			glVertex2f(node[0], node[1]);
		}
	}

	glEnd();
	
}

void draw_bezier(void)
{
	/* the y-coordinate will be (h-y) */
	int h = (int) screen_height;
	
	/* TODO */
	/* Draw line segments with points (x, h-y) */
	int idx[][4] = {{0, 1, 2, 3},
								 {3, 4, 5, 6}};
	int i, j, k;
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0f, 1.0f, 0.0f);
	for (i = 0; i < 2; i++) {
			GLfloat a[4][4];
			calculate_bezier_curve(points[idx[i][0]], points[idx[i][1]],
														 points[idx[i][2]], points[idx[i][3]], a);
			for (k = 0; k < NUM_SAMPLES; k++) {
				GLfloat u = (float)k / NUM_SAMPLES;
				GLfloat node[2];
				node[0] = a[0][0] + a[1][0] * u + a[2][0] * u * u + a[3][0] * u * u * u;
				node[1] = a[0][1] + a[1][1] * u + a[2][1] * u * u + a[3][1] * u * u * u;
				node[1] = h - node[1];
				glVertex2f(node[0], node[1]);
			}
	}
	
	glEnd();
}
