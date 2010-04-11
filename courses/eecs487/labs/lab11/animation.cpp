#include <cstdlib>
#include <sys/time.h>
#include <math.h>

#include "interpolator.h"
#include "xvec.h"
#include "xmat.h"

#include <iostream>

extern "C" {
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif
}

using namespace std;

/* TODO 2: */
/* place the following line somewhere within drawRobotArm() */
/*		glRotatef(ikInPlaneRotation*180.0f/M_PI, 0.0f, 0.0f, 1.0f); */

/* TODO 4: */
/* place the following line somewhere within drawRobotArm()*/
/*		glRotatef(ikyAxisRotation*180.0f/M_PI, 0.0f, 1.0f, 0.0f); */

/* TODO 5: */
/* place the following line somewhere within drawRobotArm()*/
/*		glRotatef(ikPoleRotation*180.0f/M_PI, 1.0f, 0.0f, 0.0f); */

#ifndef M_PI
#define M_PI 3.1415926535897932384626433
#endif /* M_PI */

#define EPSILON 0.0001f

/* The number of draws per keyframe step. You may
 want to tweak this value for the best experience */
#define KEYFRAME_STEP 1500

/* The interpolator for the ik position */
Interpolator<XVec4f> ikPositionInterpolator;

/* if playback is on/off */
bool isPlayback = false;

/* the current frame number being rendered,
 only relevant if playback is on */
int currentFrame = 0;

/* the length and polar rotation angles of the two joints */
GLfloat jointRotation0 = 0.0f, jointRotation1 = 0.0f;
GLfloat jointLength0 = 0.4f, jointLength1 = 0.3f;

/* the ik system rotation */
GLfloat ikInPlaneRotation = 0.0f;
GLfloat ikyAxisRotation = 0.0f;

/* the ik pole rotation */
GLfloat ikPoleRotation = 0.0f;

/* the location of the ik handle */
XVec4f ikPosition(jointLength0, 0.0f, 0.0f, 1.0f);

int flip_x = 0;

/* GLUT callbacks */
void initGL();
void display();
void keyboard(unsigned char key, int x, int y);
void refresh();

/* calculate the ik angles */
void updateAngles();

int main (int argc, char *argv[])
{
	/* init GLUT */
    glutInit(&argc, argv);
	
	/* prepare the window size */
    glutInitWindowSize(800, 600);
	
	/* create a window with the necessary buffers */
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
	
	/* cast to void to avoid the warning that the
	 returned value (the window handle) is not used */
    (void)glutCreateWindow("Lab11: IK Animation");
	
	/* set the GLUT callbacks. there is
	 no reshape callback since the
	 default one provided by GLUT does
	 all that is needed here (it simply
	 updates the viewport) */
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
	
	/* used to draw/animate while idle */
	glutIdleFunc(refresh);
	
	/* prepare OpenGL state */
	initGL();
	
	/* initialize the ik angles */
	updateAngles();
	
	/* enter GLUT's event loop */
    glutMainLoop();
	
    return 0;
}

void initGL()
{
	/* clear with a dark, pale blue */
	glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
	
	/* enable depth testing */
	glEnable(GL_DEPTH_TEST);

	/* load a simple projection (it will not
	 be changed anywhere else) */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glFrustum(-0.5, 0.5, -0.375, 0.375, 1.0, 7.0);
}

void drawIkHandle()
{
	/* draw the ik handle as a floating cross hair */
	
	/* save the state */
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	
	/* move to the handle */
	glTranslatef(ikPosition.x(), ikPosition.y(), ikPosition.z());
	
	/* draw the handle */
	GLfloat size = 0.1f;
	
	/* draw a crosshair */
	glColor3f(0.0f, 0.0f, 1.0f);
	glBegin(GL_LINES);
	glVertex3f(-size, 0.0f, 0.0f);
	glVertex3f( size, 0.0f, 0.0f);
	glVertex3f(0.0f,  size, 0.0f);
	glVertex3f(0.0f, -size, 0.0f);
	glVertex3f(0.0f, 0.0f,  size);
	glVertex3f(0.0f, 0.0f, -size);
	glEnd();

	/* restore the state */
	glPopMatrix();
}

void drawRobotArm()
{
	/* save the state */
	glPushMatrix();
	
	/* TODO */
	/* put the lines at the top of this file somewhere inside this function */
	glRotatef(ikyAxisRotation*180.0f/M_PI, 0.0f, 1.0f, 0.0f);
	glRotatef(ikInPlaneRotation*180.0f/M_PI, 0.0f, 0.0f, 1.0f);
	glRotatef(ikPoleRotation*180.0f/M_PI, 1.0f, 0.0f, 0.0f);
	if (flip_x) {
		glRotatef(180.0f, 1.0f, 0.0f, 0.0f);
		//cout << "flip\n";
	}
	/* rotate for the first joint */
	glRotatef(jointRotation0*180.0f/M_PI, 0.0f, 0.0f, 1.0f);
	
	/* save the state to retrun to after drawing the first bone */
	glPushMatrix();
	
	/* move and scale in preparation to draw the first bone */
	glTranslatef(jointLength0/2.0f, 0.0f, 0.0f);
	glScalef(jointLength0, 0.05f, 0.05f);
	
	/* draw the first bone */
	glColor3f(0.5f, 0.3f, 0.7f);
	glutSolidCube(1.0f);
	
	/* restore to the state at the skeleton root */
	glPopMatrix();
	
	/* move to the end of the first joint */
	glTranslatef(jointLength0, 0.0f, 0.0f);
	
	/* rotate for the second joint */
	glRotatef(jointRotation1*180.0f/M_PI, 0.0f, 0.0f, 1.0f);
	
	/* move and scale in preparation to draw the second bone */
	glTranslatef(jointLength1/2.0f, 0.0f, 0.0f);
	glScalef(jointLength1, 0.04f, 0.04f);
	
	/* draw the second bone */
	glColor3f(0.3f, 0.5f, 0.7f);
	glutSolidCube(1.0f);
	
	/* restore the state */
	glPopMatrix();
}

void updateAngles()
{
	/* TODO 0: */
	/* update jointRotation0 and jointRotation1 */
	
	float x = ikPosition(0);
	float y = ikPosition(1);
	float z = ikPosition(2);
	float l1 = jointLength0;
	float l2 = jointLength1;

	jointRotation1 = acos((x*x + y*y + z*z - l1*l1 - l2*l2) / (2*l1*l2));
	float theta2 = jointRotation1;
	jointRotation0 = atan((-l2 * sin(theta2)) / (l1 + l2 * cos(theta2)));
	
	/* TODO 1: */
	/* update ikInPlaneRotation */
	ikInPlaneRotation = atan2(y, (sqrt(x*x + z*z) * x / fabs(x)));
	//ikInPlaneRotation = atan(y / sqrt(x*x + z*z));
	
	/* TODO 3: */
	/* update ikyAxisRotation */
	
	ikyAxisRotation = atan(-z/x);
	//ikyAxisRotation = atan2(-z, x);
}

void drawScene()
{	
	/* draw the ik handle crosshair */
	drawIkHandle();

	/* draw the arm */
	drawRobotArm();
}

void refresh()
{
	/* if playback is enabled, update the ik handle
	 position from keyframe value interpolation */
	if( isPlayback )
	{
		/* get the number of keyframes */
		unsigned int keyframeCount = ikPositionInterpolator.keyframeCount();
		
		/* if only on keyframe, just set it to that value */
		if( keyframeCount == 1 )
		{
			ikPosition = ikPositionInterpolator.interpolatedValueAtTime(0.0f);
		}
		
		/* when the animation is over, or there are 0/1 keyframes, stop */
		if( (unsigned int)(currentFrame/KEYFRAME_STEP) == keyframeCount || keyframeCount < 2 )
		{
			isPlayback = false;
			std::cout << "Playback finished." << std::endl << std::endl;
		}
		
		/* interpolate the value */
		else
		{
			float time = (float)currentFrame/(float)KEYFRAME_STEP;
			currentFrame++;
			
			ikPosition = ikPositionInterpolator.interpolatedValueAtTime(time);
		}
		
		glutPostRedisplay();
	}
}

void display()
{		
	/* prepare the angles */
	updateAngles();
	
	/* clear the buffers */
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	
	/* prepare the modelview with an offset backward */
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0f, 0.0f, -1.6f);
	
	/* draw the scene */
	drawScene();
	
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
			
			/* erase the keyframes */
		case '0':
			ikPositionInterpolator.clearKeyframes();
			std::cout << "Keyframe data erased." << std::endl;
			break;
			
			/* add a keyframe */
		case ' ':
			ikPositionInterpolator.appendKeyframeValue(ikPosition);
			std::cout << "Added Keyframe (" << XVec3f(ikPosition) << ")" << std::endl;
			break;
			
			/* play */
		case '.':
			std::cout << "Begin playback..." << std::endl;
			currentFrame = 0;
			isPlayback = true;
			break;
			
			/* rotation of the ik pole */
		case 'r':
			ikPoleRotation += 0.08f;
			break;
			
			/* move the ik handle left */
		case 'h':
			if (ikPosition.x() <= 0.02f && ikPosition.x() > 0 &&
					fabs(ikPosition.z()) > EPSILON) flip_x = ~flip_x;
			ikPosition.x() -= 0.02f;
			break;
			
			/* move the ik handle right */
		case 'l':
			if (ikPosition.x() < 0 && ikPosition.x() >= -0.02f &&
					fabs(ikPosition.z()) > EPSILON) flip_x = ~flip_x;
			ikPosition.x() += 0.02f;
			break;
			
			/* move the ik handle down */
		case 'j':
			ikPosition.y() -= 0.02f;
			break;
			
			/* move the ik handle up */
		case 'k':
			ikPosition.y() += 0.02f;
			break;
			
			/* move the ik handle backward */
		case 'b':
			ikPosition.z() -= 0.02f;
			break;
			
			/* move the ik handle forward */
		case 'f':
			ikPosition.z() += 0.02f;
			break;

		default:
			break;
	}
	
	/* ask GLUT to draw again */
	glutPostRedisplay();
}
