
#include <stdio.h>
#include <stdlib.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <string.h>

/* -- GLUT functions and callbacks -- */

/* Paint the window - this is where all the GL action is */
void disp(void);

/* What to do when the window is resized? */
void reshape(int w, int h);

/* Trap and process keyboard events */
void keyb(unsigned char key, int x, int y);

/* Trap and process mouse-click events */
void mouse(int button, int state, int x, int y); /* click events */

/* Refresh the display when system is idle */
void refresh_display(void);



/* Function that draws a 'point' on the square grid of 'pixels' */

void drawpoint(GLint x, GLint y, GLclampf r, GLclampf g, GLclampf b);



/* --- Global variables for the program --- */

#define NPOINTS 2
int points[NPOINTS][2]; /* Array of 2D points */

/* screen width and height - screen refers to
   window within which we draw*/

#define DELTA 25
GLdouble screen_width=51*DELTA, screen_height=32*DELTA;

/* Where was the mouse last clicked? */
int endpoint1[2], endpoint2[2];

/* Flag to indicate the number of endpoints that the user has set by clicking */
int num_endpoints = 0;

/* Handle to the GLUT window */
int window_handle;



/* ------- Actual Code --------- */



int main(int argc, char *argv[])
{
	/* Initialize GLUT, let it extract command-line GLUT options that
	 you may provide 
	 - NOTE THE & BEFORE argc
	*/
	glutInit(&argc, argv);

	/* Want single window and color will be specified as RGBA values */
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	/* Set initial window size
	screen_width and screen_height have been above
	*/
	glutInitWindowSize((int)screen_width, (int)screen_height);

	/* Create the window and store the handle to it */
	window_handle = glutCreateWindow("Experiment with line drawing" /* title */ );


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

void swap(int * a, int * b)
{
    if (a != b)
	*a ^= *b, *b ^= *a, *a ^= *b;
}

void drawline(int x0, int x1, int y0, int y1, int ispos, int isswap, GLfloat * p1color, GLfloat * p2color)
{
    //printf("%d %d %d %d\n", x0, y0, x1, y1);
    int x = x0;
    int y = y0;
    int dx = x1 - x0;
    int dy = y1 - y0;
    int fmid = dx * ((y0 << 1) + 1) - ((dy * (x0 + 1) - x0 * y1 + x1 * y0) << 1);

    GLfloat dr = (p2color[0] - p1color[0]) / dx;
    GLfloat dg = (p2color[1] - p1color[1]) / dx;
    GLfloat db = (p2color[2] - p1color[2]) / dx;
    printf("p1 %f %f %f\n", p1color[0], p1color[1], p1color[2]);
    printf("p2 %f %f %f\n", p2color[0], p2color[1], p2color[2]);
    printf("%f %f %f\n", dr, dg, db);
    GLfloat * curcolor = (GLfloat*)malloc(sizeof(GLfloat) * 3);
    memcpy(curcolor, p1color, sizeof(GLfloat) * 3);

    dx <<= 1;
    dy <<= 1;

    for (x = x0;  x <= x1; x++) {
	printf("cur color %f %f %f\n", curcolor[0], curcolor[1], curcolor[2]);
	int real_y = y;
	int real_x = x;
	if (!ispos) real_y = (y0 << 1) - real_y;
	if (isswap) swap(&real_x, &real_y);
	drawpoint(real_x, real_y, curcolor[0], curcolor[1], curcolor[2]);
	if (fmid < 0) {
	    y++;
	    fmid += (dx - dy);
	}
	else
	    fmid -= dy;
	// Update the color for the next point
	curcolor[0] += dr;
	curcolor[1] += dg;
	curcolor[2] += db;
    }
    free(curcolor);
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
	if(endpoint1[0]>0 && endpoint1[1]>0)
		drawpoint(endpoint1[0], endpoint1[1], 1.0, 0.0, 0.0);

	/* Draw the second point (square) in blue, if value) */
	if(endpoint2[0]>0 && endpoint2[1]>0)
		drawpoint(endpoint2[0], endpoint2[1], 0.0, 0.0, 1.0);

	/* Draw straight line connecting end points if both are valid*/
	if(num_endpoints == 2) {
		glLineWidth(3.0);
		glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_LINES);
		glVertex2i(endpoint1[0]*DELTA+DELTA/2, endpoint1[1]*DELTA+DELTA/2);
		glVertex2i(endpoint2[0]*DELTA+DELTA/2, endpoint2[1]*DELTA+DELTA/2);
		glEnd();


		/* TODO - implement the line drawing algorithm here
			int endpoint1[2], endpoint2[2] store integer coordinates of the 'endpoint pixels'
			The support code does the task of trapping mouse clicks and setting these
			NOTE: Interpolate the color between the endpoints to get a gradual transition
		 */
		int ispos, isswap; /* To record whether the line is positive scope and 
				      whether the coordinates are swapped.*/
		int x0 = endpoint1[0];
		int y0 = endpoint1[1];
		int x1 = endpoint2[0];
		int y1 = endpoint2[1];
		int dx = x1 - x0;
		int dy = y1 - y0;

		GLfloat * p1color = (GLfloat*)malloc(sizeof(GLfloat) * 3);
		GLfloat * p2color = (GLfloat*)malloc(sizeof(GLfloat) * 3);
		//Red start
		p1color[0] = 1.0;
		p1color[1] = 0.0;
		p1color[2] = 0.0;
		// Blue end
		p2color[0] = 0.0;
		p2color[1] = 0.0;
		p2color[2] = 1.0;
		if (dy * dx >= 0) {
		    ispos = 1;
		    if (dx * dx > dy * dx)
			isswap = 0;
		    else {
			isswap = 1;
			swap(&x0, &y0);
			swap(&x1, &y1);
		    }
		    if (y1 - y0 < 0 || x1 - x0 < 0) {
			// Swap the two points
			swap(&y0, &y1);
			swap(&x0, &x1);
			GLfloat * tmp = p1color;
			p1color = p2color;
			p2color = tmp;
		    }
		} else {
		    ispos = 0;
		    if (dy * dx > - dx * dx) 
			isswap = 0;
		    else {
			isswap = 1;
			swap(&x0, &y0);
			swap(&x1, &y1);
		    }
		    if (x1 - x0 < 0) {
			// Swap the two points
			swap(&y0, &y1);
			swap(&x0, &x1);
			GLfloat * tmp = p1color;
			p1color = p2color;
			p2color = tmp;
		    }
		    y1 = 2 * y0 - y1;
		}
		drawline(x0, x1, y0, y1, ispos, isswap, p1color, p2color);
		
		free(p1color);
		free(p2color);

	}

	/* Force OpenGL to start drawing */
	// glFlush();
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
		else {
			endpoint1[0] = -1;
			endpoint1[1] = -1;
			endpoint2[0] = -1;
			endpoint2[1] = -1;
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
