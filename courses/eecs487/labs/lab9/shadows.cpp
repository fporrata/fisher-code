#include <stdlib.h>
#include <stdio.h>

extern "C" {
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
}

#include <math.h>

#ifndef M_PI
#define M_PI 3.1415926535897932384626433
#endif /* M_PI */

float offset0 = 0.0f, offset1 = 0.0f;

/* GLUT callbacks */
void initGL();
void display();
void keyboard(unsigned char key, int x, int y);

/* the tesselation of the walls. higher
 tesselation will result in better
 lighting because it is per-vertex and
 not per-fragment */
int quadTessellation = 16;

/* The different planes in the scene. there
 is not 'front' wall behind the viewer. */
enum {FLOOR = 0, LEFT_WALL, RIGHT_WALL, CEILING, REAR_WALL};
#define NUM_PLANAR_SURFACES 5

/* whether the shadows are soft */
bool softShadows = false;

/* clip shadows to recievers */
bool stencilClipping = false;

/* how many times the lights is moved along
 each axis and by how much. this produces
 the soft shadows */
int numJitters = 8;
float jitterAmount = 0.02f;

/* whether to draw the whole scene */
bool onlyFLoorVisible = false;

/* global rotation */
GLfloat rotation = 0.0f;

/* the location of the lights and the current
 jittered light */
GLfloat lightPos[] = {0.5f, 0.4f, 0.5f, 1.0f};
GLfloat jitterLightPos[] = {0.5f, 0.4f, 0.5f, 1.0f};

/* the current plane upon which geometry
 will be projected/flattened */
GLfloat *flatteningPlane;

/* the plane equations for each of the
 five walls in the scene. Each is four
 numbers (a,b,c,d), representing the
 plane ax+by+cz+d=0. */
GLfloat planeEquations[NUM_PLANAR_SURFACES][4] = {
	/* floor */
	{0.0f, 1.0f, 0.0f, 1.0f},
	/* left wall */
	{1.0f, 0.0f, 0.0f, 1.0f},
	/* right wall */
	{-1.0f, 0.0f, 0.0f, 1.0f},
	/* ceiling */
	{0.0f, -1.0f, 0.0f, 1.0f},
	/* rear wall */
	{0.0f, 0.0f, 1.0f, 1.0f}
};

/* set the shadow projection plane */
void setShadowProjectionPlane(GLfloat *plane)
{
	flatteningPlane = plane;
}

GLfloat dot(const GLfloat * v0, const GLfloat * v1, int dim)
{
	GLfloat res = 0;
	for (int i = 0; i < dim; i++) {
		res += v0[i] * v1[i];
	}
	return res;
}

GLfloat length(const GLfloat * v, int dim)
{
	return sqrt(dot(v, v, dim));
}

void mul(const GLfloat * src, GLfloat c, GLfloat * dst, int dim)
{
	for (int i = 0; i < dim; i++) {
		dst[i] = src[i] * c;
	}
}

void div(const GLfloat * src, GLfloat c, GLfloat * dst, int dim)
{
	for (int i = 0; i < dim; i++)
		dst[i] = src[i] / c;
}

GLfloat frand()
{
	return (float)rand() / RAND_MAX;
}

void add(const GLfloat * s0, const GLfloat * s1, GLfloat * dst, int dim)
{
	for (int i = 0; i < dim; i++)
		dst[i] = s0[i] + s1[i];
}

/* draw a wall */
void drawWall(int wall)
{
	/* save the OpenGL state since this function
	 draws a generic wall and simply rotates it
	 first, depending which wall it is */
	glPushMatrix();
	
	/* for each wall, set the color, and if needed,
	 rotate the wall into position */
	if( wall == FLOOR )
		glColor3f(0.4f, 0.8f, 0.7f);
	else if( wall == RIGHT_WALL )
	{
		glColor3f(0.7f, 0.8f, 0.4f);
		glRotatef(90.0f, 0.0f, 0.0f, 1.0f);
	}
	else if( wall == CEILING )
	{
		glColor3f(0.4f, 0.7f, 0.8f);
		glRotatef(180.0f, 0.0f, 0.0f, 1.0f);
	}
	else if( wall == LEFT_WALL )
	{
		glColor3f(0.8f, 0.7f, 0.4f);
		glRotatef(270.0f, 0.0f, 0.0f, 1.0f);
	}
	else if( wall == REAR_WALL )
	{	
		glColor3f(0.8f, 0.4f, 0.7f);
		glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	}
	
	/* draw a grid of quads in the xz-plane.
	 the number of quads drawn is the square
	 of the tessellation */
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 1.0f, 0.0f);
	for( int i = 0; i < quadTessellation; ++i )
	{
		for( int j = 0; j < quadTessellation; ++j )
		{
			float x0 = (float)i/quadTessellation*2.0f - 1.0f;
			float z0 = (float)j/quadTessellation*2.0f - 1.0f;
			
			float x1 = (float)(i+1)/quadTessellation*2.0f - 1.0f;
			float z1 = (float)(j+1)/quadTessellation*2.0f - 1.0f;
			
			glVertex3f(x0, -1.0f, z0);
			glVertex3f(x0, -1.0f, z1);
			glVertex3f(x1, -1.0f, z1);
			glVertex3f(x1, -1.0f, z0);
		}
	}
	glEnd();
	
	/* return the state, since a rotation may
	 have been used to position the wall */
	glPopMatrix();
}

int main (int argc, char *argv[])
{
	/* init GLUT */
    glutInit(&argc, argv);
	
	/* prepare the window size */
    glutInitWindowSize(600, 600);
	
	/* create a window with the necessary buffers */
#if defined(__APPLE__)
    glutInitDisplayString("acca>8 rgba=8 depth>=12 double stencil>=8");
#else
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_ACCUM | GLUT_DOUBLE | GLUT_STENCIL);
#endif

	/* cast to void to avoid the warning that the
	 returned value (the window handle) is not used */
    (void)glutCreateWindow("Lab9: OpenGL (Soft) Shadows");
	
	/* set the GLUT callbacks. there is
	 no reshape callback since the
	 default one provided by GLUT does
	 all that is needed here (it simply
	 updates the viewport) */
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
	
	/* prepare OpenGL state */
	initGL();
	
	/* enter GLUT's event loop */
    glutMainLoop();
	
    return 0;
}

void initGL()
{
	/* clear with a pale blue */
	glClearColor(0.7f, 0.8f, 1.0f, 1.0f);
	
	/* enable depth testing */
	glEnable(GL_DEPTH_TEST);
	
	/* setup one light */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightf(GL_LIGHT0, GL_LINEAR_ATTENUATION, 0.5f);
	
	/* set a polygon offset. its use is disabled
	 by default, but it will be enabled lated. it
	 puts an offset into the depth buffer, used
	 here to avoid z-fighting when drawing shadows */
	glPolygonOffset(-1.0f, -5.0f);
	
	/* let OpenGL calculate material properties
	 from calls to glColor instead of glMaterial */
	glEnable(GL_COLOR_MATERIAL);
	
	/* load a simple projection (it will not
	 be changed anywhere else) */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-1.0, 1.0, -1.0, 1.0, 2.0, 7.0);
}

void positionAndDrawLight()
{
	/* position the light for hard/soft shadows */
	if( softShadows )
		glLightfv(GL_LIGHT0, GL_POSITION, jitterLightPos);
	else
		glLightfv(GL_LIGHT0, GL_POSITION, lightPos);
	
	/* save the state, move to the light position,
	 draw a near-white sphere, and restore the state.
	 the light is always drawn at the same place so
	 that it does not turn out blurry in the final
	 render. */
	glPushMatrix();
	glTranslatef(lightPos[0], lightPos[1], lightPos[2]);
	
	glDisable(GL_LIGHTING);
	glColor3f(1.0f, 1.0f, 0.8f);
	glutSolidSphere(0.05f+0.13f*(onlyFLoorVisible), 20, 20);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}



/* project onto plane 'flatteningPlane' with 'lightPos'
 being the point of convergence of the projecion. think
 of it as a perspective projection onto 'flattening plane' */
void multShadowMat()
{
	static GLfloat shadowMat[] = {
		1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f
	};
	
	/* prepare the position of the light */
	GLfloat *posLight = lightPos;
	if( softShadows )
		posLight = jitterLightPos;
	
	/* BEGIN TODO */
	/* set the 16 components of the shadow matrix using
	 posLight[4] and the global float flatteningPlane[4] */
	/* for matrix multiplication Mv, these are the rows.
	 If one imagines (v^T)M^T, then these are the columns. */
	GLfloat nDotL = dot(flatteningPlane, posLight, 3);
	GLfloat d = flatteningPlane[3];
	GLfloat * L = posLight;
	GLfloat * n = flatteningPlane;
	shadowMat[0] = nDotL + d - L[0] * n[0];
	shadowMat[4] = - L[0] * n[1];
	shadowMat[8] = - L[0] * n[2];
	shadowMat[12] = - L[0] * d;
	
	shadowMat[1] = - L[1] * n[0];
	shadowMat[5] = nDotL + d - L[1] * n[1];
	shadowMat[9] = - L[1] * n[2];
	shadowMat[13] = - L[1] * d;
	
	shadowMat[2] = - L[2] * n[0];
	shadowMat[6] = - L[2] * n[1];
	shadowMat[10]= nDotL + d - L[2] * n[2];
	shadowMat[14] = - L[2] * d;
	
	shadowMat[3] = - n[0];
	shadowMat[7] = - n[1];
	shadowMat[11]= - n[2];
	shadowMat[15] = nDotL;
	/* END TODO */
	
	/* add the transformation to the ModelView matrix */
	glMultMatrixf(shadowMat);
}

void drawOccludersFlattened(bool flattened)
{
	/* if the occluders will be drawn as shadows,
	 set the draw color to black, enable polygon
	 offset to avoid z-fighting, turn off lighting,
	 save the state, and set up the shadow
	 projection */

	if( flattened )
	{
		GLfloat color[4];
		glGetFloatv(GL_CURRENT_COLOR, color);
		glColor3f(0.3f*color[0], 0.3f*color[1], 0.3f*color[2]);
		glEnable(GL_POLYGON_OFFSET_FILL);
		
		glDisable(GL_LIGHTING);
		glPushMatrix();
		multShadowMat();
	}
	
	/* save the state and put on some funcky rotations */
	glPushMatrix();
	glRotatef(rotation, 0.7f, 1.0f, -0.2f);
	glRotatef(0.434658f*rotation, 0.8f, 0.1f, 0.65f);
	
	/* place some objects in the scene */
	/* a torus in the corner */
	glPushMatrix();
	if( !flattened )
		glColor3f(1.0f, 0.5f, 0.3f);
	/* this makes the torus move forward and back */
	glTranslatef(-0.3f*sinf(rotation/80.0f), 0.3f*sinf(rotation/100.0f), 0.0f);
	glRotatef(80.0f, 1.0f, 1.0f, 0.0f);
	glutSolidTorus(0.05f, 0.35f, 16, 24);
	glPopMatrix();
	
	/* a tepot floating */
	glPushMatrix();
	if( !flattened )
		glColor3f(0.5f, 0.3f, 1.0f);
	glTranslatef(-0.3f, 0.16f, 0.1f);
	glRotatef(60.0f*sinf(rotation/60.0f), 0.2f, -0.6f, -0.6f);
	glutSolidTeapot(0.16f);
	glPopMatrix();
	
	/* pop back from the rotation for the objects */
	glPopMatrix();
	
	/* if the objects were drawn as shadows,
	 return the state to what it was at the
	 start */
	if( flattened )
	{
		glDisable(GL_POLYGON_OFFSET_FILL);
		glEnable(GL_LIGHTING);	
		glPopMatrix();
	}
}

void drawScene()
{	
	/* clear the buffers */
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	/* position light and draw a sphere there */
	positionAndDrawLight();
	
	/* draw the world (it's a cube) */
	for( int i = FLOOR; i <= REAR_WALL*(!onlyFLoorVisible); ++i )
	{
		/* clear the stencil buffer for each wall.
		 thus the buffer is full of zeroes */
		glClear(GL_STENCIL_BUFFER_BIT);
		
		/* enable stencil test for fragment selection */
		if( stencilClipping )
			glEnable(GL_STENCIL_TEST);
		
		/* TODO */
		/* Set the stencil buffer operation to always
		 place 0x1 into the stencil buffer when a
		 fragment is drawn and to always accept a fragment */
		glStencilFunc(GL_ALWAYS, 0x1, 0xffffffff);
		
		/* TODO */
		/* if a fragment fails the stencil test,
		 keep the old value in the stencil buffer.
		 if a fragment passes the stencil test
		 but fails the depth test, keep the old
		 stencil value. if stencil and depth tests
		 pass, replace the stencil buffer value,
		 which should be 0x1, specified previously */
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
		
		/* draw the wall */
		drawWall(i);

		/* TODO */
		/* set the stencil accept/reject function to
		 only accept fragments that are drawn 'in' the
		 current wall. the stencil values there are 0x1
		 and 0x0 every where else. use 0xffffffff as
		 the stencil mask */
		glStencilFunc(GL_EQUAL, 0x1, 0xffffffff);
		
		/* set the wall's plane equation to be current */
		setShadowProjectionPlane(planeEquations[i]);
		
		/* draw the occluders with black,
		 flattened onto the specified plane */
		drawOccludersFlattened(true);
		
		/* no longer use the stencil buffer to
		 reject fragments */
		if( stencilClipping )
			glDisable(GL_STENCIL_TEST);
	}

	/* draw the occluders normally */
	drawOccludersFlattened(false);
}

void display()
{
	/* prepare the modelview with an offset backward */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -3.0f);

	/* if soft shadows are desired, jitter the light position,
	 each time accumulating the contents of the framebuffer,
	 then copy the accumulation buffer into the framebuffer */
	if( softShadows )
	{		
		glClear(GL_ACCUM_BUFFER_BIT);
		/* BEGIN TODO */
		/* Jitter the light position numJitters times */
		for( int i = 0; i < numJitters; ++i )
		{
			/* The following three lines don't do much. They
			 just copy the actual position into the current
			 jitter position. Change the lines so that they
			 actually jitter the position by the global float
			 'jitterAmount.' How, and in what direction is up
			 to you. */
			GLfloat dir[3] = {frand(), frand(), frand()};
			div(dir, length(dir, 3), dir, 3);

			printf("%f %f %f\n", dir[0], dir[1], dir[2]);
			GLfloat pert[3];
			mul(dir, i * jitterAmount, pert, 3);
			add(lightPos, pert, jitterLightPos, 3);
			
			/* Render the scene. Do not remove this line! */
			drawScene();
			
			/* Copy the contents of the framebuffer into the
			 accumulation buffer with weight 1/numJitters. */
			glAccum(GL_ACCUM, 1.0f / numJitters);
			
			/*END TODO */
		}

		glAccum(GL_RETURN, 1.0f);
	}
	/* for hard shadows, just drawn the scene */
	else
		drawScene();

	/* with really soft shadows, the application may
	 slow greatly, so wait for the pending operations
	 to	finish */
	glFinish();

	/* and then swap the framebuffer into view */
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y)
{
	switch(key)
	{
		/* quit on 'escape' */
		case '\033':
			exit(0);
			break;
			
		/* move the light left */
		case 'h':
			lightPos[0] -= 0.02f;
			break;
			
		/* move the light right */
		case 'l':
			lightPos[0] += 0.02f;
			break;
			
		/* move the light down */
		case 'j':
			lightPos[1] -= 0.02f;
			break;
			
		/* move the light up */
		case 'k':
			lightPos[1] += 0.02f;
			break;
			
		/* move the light backward */
		case 'b':
			lightPos[2] -= 0.02f;
			break;
			
		/* move the light forward */
		case 'f':
			lightPos[2] += 0.02f;
			break;
			
		/* increase the wall tessellation */
		case '+':
			quadTessellation++;
			break;
			
		/* decrease the wall tessellation */
		case '-':
			if( quadTessellation > 1 )
				quadTessellation--;
			break;
			
		/* increase the number of light jitters */
		case ']':
			numJitters++;
			break;
			
		/* decrease the number of light jitters */
		case '[':
			if( numJitters > 1 )
				numJitters--;
			break;
			
		/* increase the amount of light jitter */
		case '\'':
			jitterAmount += 0.01f;
			break;
			
		/* decrease the amount of light jitter */
		case ';':
			if( jitterAmount > 0.01f )
				jitterAmount -= 0.01f;
			break;
			
		/* toggle the visibility of the walls/ceiling */
		case 'x':
			onlyFLoorVisible = !onlyFLoorVisible;
			break;
			
		/* rotate the world */
		case 'r':
			rotation += 5.0f;
			break;
			
		/* toggles hard/soft shadows */
		case 's':
			softShadows = !softShadows;
			break;
			
		/* toggles stencil-based			GLfloat dir[3] = {frand(), frand(), frand()};
			div(dir, length(dir, 3), dir, 3); clipping */
		case 'c':
			stencilClipping = !stencilClipping;
			break;
			
		default:
			break;
	}
	
	/* ask GLUT to draw again */
	glutPostRedisplay();
}
