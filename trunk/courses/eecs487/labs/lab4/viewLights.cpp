/* Include the GL headers */
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <math.h>
#include "xvec.h"
#include "xmat.h"

#include "fakeGL.h"

XVec3f position, gazeDir, topDir;

XVec2i mouseDown;
XVec3f prevGazeDir, prevTopDir;

/* YOUR CODE HERE */
/* Put these lines somewhere so that light0 has fixed
 location with respect to the spheres 
 
 GLfloat pos[4] = {0.0, 2.0, 0.0, 1.0};
 glLightfv(GL_LIGHT0, GL_POSITION, pos);
 
*/

/* YOUR CODE HERE */
/* Put these lines somewhere so that light1 has fixed
 location with respect to the 'camera' or 'viewer' 
 
 GLfloat pos2[4] = {0.0, 0.0, 0.0, 1.0};
 glLightfv(GL_LIGHT1, GL_POSITION, pos2);

*/

void drawSphere();

void disp() {
	/* Clear the color and depth information */
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	/* reset the modelview matrix */
	_glMatrixMode(GL_MODELVIEW);
	_glLoadIdentity();
	
	/* do the view transformation */
	/* the first three args are the eye position,
	 the next three are a point in front of the eye,
	 and then the up direction */
	_gluLookAt(position.x(), position.y(), position.z(),
			   gazeDir.x()+position.x(), gazeDir.y()+position.y(), gazeDir.z()+position.z(),
			   topDir.x(), topDir.y(), topDir.z());

	/* draw a small hill of spheres */
	glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
	for( int i = -2; i <= 2; ++i ) {
		for( int j = -2; j <= 2; ++j ) {
			/* save the current transformation */
			_glPushMatrix();
			
			/* position each sphere */
			_glTranslatef(0.3f*i, -0.08*sqrtf(i*i+j*j), 0.3f*j);
			_glScalef(0.15f, 0.15f, 0.15f);
			drawSphere();
			
			/* restore the previous transformation */
			_glPopMatrix();
		}
	}
	
	glutSwapBuffers();
}

void reshape(int w, int h) {
	/* update the location of the window
	 that drawing occurs inside */
	glViewport(0, 0, w, h);

	/* apply a perspective transformation */
	_glMatrixMode(GL_PROJECTION);
	_glFrustum(-1.0f, 1.0f, -float(h)/w, float(h)/w, 2.0f, 20.0f);
}

/* keyboard callback */
void keyb(unsigned char key, int x, int y) {
	
	XVec3f gaze = gazeDir;
	gaze.normalize();
	
	/* YOUR CODE HERE */
	/* Using the vectors right and gaze, implement the following.
	 'w' moves the viewer forward , 's' moves the viewer backward,
	 'd' moves the viewer to the right, and 'a' moves the viewer to
	  the left */
	
	/* These may help: */
	XVec3f right = gazeDir.cross(topDir);
	right.normalize();
	
	switch (key) {
		case 'w':
			position += gaze*0.2f;
			break;
		case 's':
			position -= gaze*0.2f;
			break;
		case 'd':
			/* YOUR CODE HERE */
			position -= right * 0.2f;
			break;
		case 'a':
			/* YOUR CODE HERE */
			position += right * 0.2f;
			break;
		default:
			break;
	}
	glutPostRedisplay();
}

void mouse(int button, int state, int x, int y) {
	/* save the mouse location, the old gaze direction,
	 and the old top direction */
	mouseDown = XVec2i(x, y);
	prevGazeDir = gazeDir;
	prevTopDir = topDir;
}

void motion(int x, int y) {
	/* get the displacement of the mouse from
	 the initial click point */
	XVec2i displacement(x-mouseDown.x(), y-mouseDown.y());
	
	/* Get the new gaze direction by rotating the gaze direction
	 around the top direction. Notice that is uses the gaze
	 direction from when the mouse was pressed. That way floating
	 point errors do not accumulate and so the behavior is more
	 intuitive. */
	
	/* The amount the mouse moved in x,
	 scaled to make it a smaller number */
	float dx = (float)displacement.x()*-0.05f;
	
	/* Rotate the gaze direction (from when click began)
	 around the top vector by dx */
	//cout << "gaze " << gazeDir << endl << endl;
	gazeDir = prevGazeDir.rotate(topDir, dx);
	position = position.rotate(topDir, dx);
	prevGazeDir = gazeDir;
	//cout << "current gaze " << gazeDir << endl << endl;
	/* YOUR CODE HERE */
	/* Implement pitch (nose up and down). Use the previous top direction
	 and the 'current' gaze direction calculated above. This should
	 produce the best results. */
	/* Also, just to be safe, update the gaze AND the top vector. */
	
	/* Here is the right vector in case: */
	XVec3f right = gazeDir.cross(topDir);
	right.normalize();
	float dy = (float)displacement.y()*-0.05f;
	//cout << "dy " << dy << endl << endl;
	//cout << "gaze " << gazeDir << endl << endl;
	gazeDir = prevGazeDir.rotate(right, dy);
	//cout << "current gaze " << gazeDir << endl << endl;
	topDir = prevTopDir.rotate(right, dy);
	//cout << "Position " << position << endl << endl;
	position = position.rotate(right, dy);
	//cout << "Current position " << position << endl << endl;

	prevGazeDir = gazeDir;
	prevTopDir = topDir;
	mouseDown = XVec2i(x, y);
	
	glutPostRedisplay();
}

int main (int argc, char *argv[]) {
    glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(640, 480);
	glutCreateWindow("Lab4: Experiment with transformations and views");
	glutDisplayFunc(disp);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyb);
	glutMouseFunc(mouse);
	glutMotionFunc(motion);

	/* init depth sorting and lighting */
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	
	
	_glMatrixMode(GL_PROJECTION);
	_glLoadIdentity();
	
	_glMatrixMode(GL_MODELVIEW);
	_glLoadIdentity();
	
	/* gives the two light some color */
	GLfloat color[4] = {0.0, 0.0, 1.0, 1.0};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, color);
	GLfloat pos[4] = {0.0, 2.0, 0.0, 1.0};
	glLightfv(GL_LIGHT0, GL_POSITION, pos);
	
	GLfloat color2[4] = {1.0, 0.0, 0.0, 1.0};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, color2);
	GLfloat pos2[4] = {0.0, 0.0, 0.0, 1.0};
	glLightfv(GL_LIGHT1, GL_POSITION, pos2);

	/* give the second light some attenuation,
	 that is, it's effect is distance-dependent */
	glLightf(GL_LIGHT1, GL_LINEAR_ATTENUATION, 0.2f);
	
	/* initialize the viewing vectors */
	position = XVec3f(0.0, 0.0, 10.0);
	gazeDir = XVec3f(0.0, -0.2, -1.0);
	topDir = XVec3f(0.0, 1.0, 0.0);
	
	glutMainLoop();
	return 0;
}

/* draw a sphere */
void drawSphere() {
	glBegin(GL_QUADS);
	
	int divisionsAround = 12;
	int divisionsHeight = 12;
	
	for( int i = 0; i < divisionsHeight; i++ )
	{
		for( int j = 0; j < divisionsAround; j++ )
		{
			float theta = M_PI*i/divisionsHeight;
			float thetaNext = M_PI*(i+1)/divisionsHeight;
			
			float phi = M_PI*2.0*j/divisionsAround;
			float phiNext = M_PI*2.0*(j+1)/divisionsAround;
			
			_glNormal3f(sinf(theta)*sinf(phi), cosf(theta), sinf(theta)*cosf(phi));
			_glVertex3f(sinf(theta)*sinf(phi), cosf(theta), sinf(theta)*cosf(phi));
			
			_glNormal3f(sinf(thetaNext)*sinf(phi), cosf(thetaNext), sinf(thetaNext)*cosf(phi));
			_glVertex3f(sinf(thetaNext)*sinf(phi), cosf(thetaNext), sinf(thetaNext)*cosf(phi));
			
			_glNormal3f(sinf(thetaNext)*sinf(phiNext), cosf(thetaNext), sinf(thetaNext)*cosf(phiNext));
			_glVertex3f(sinf(thetaNext)*sinf(phiNext), cosf(thetaNext), sinf(thetaNext)*cosf(phiNext));
			
			_glNormal3f(sinf(theta)*sinf(phiNext), cosf(theta), sinf(theta)*cosf(phiNext));
			_glVertex3f(sinf(theta)*sinf(phiNext), cosf(theta), sinf(theta)*cos(phiNext));
/*glNormal3f(sinf(theta)*sinf(phi), cosf(theta), sinf(theta)*cosf(phi));
			glVertex3f(sinf(theta)*sinf(phi), cosf(theta), sinf(theta)*cosf(phi));
			
			glNormal3f(sinf(thetaNext)*sinf(phi), cosf(thetaNext), sinf(thetaNext)*cosf(phi));
			glVertex3f(sinf(thetaNext)*sinf(phi), cosf(thetaNext), sinf(thetaNext)*cosf(phi));
			
			glNormal3f(sinf(thetaNext)*sinf(phiNext), cosf(thetaNext), sinf(thetaNext)*cosf(phiNext));
			glVertex3f(sinf(thetaNext)*sinf(phiNext), cosf(thetaNext), sinf(thetaNext)*cosf(phiNext));
			
			glNormal3f(sinf(theta)*sinf(phiNext), cosf(theta), sinf(theta)*cosf(phiNext));
			glVertex3f(sinf(theta)*sinf(phiNext), cosf(theta), sinf(theta)*cos(phiNext));*/

		}
	}
	glEnd();
}
