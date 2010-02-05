#include <vector>

/* Include the GL headers */
#ifdef __APPLE__
#include <OpenGL/OpenGL.h>
#else
#include <GL/gl.h>
#endif

#include <math.h>
#include "xvec.h"
#include "xmat.h"

#include <iostream>


using namespace std;

/* Variables to mimic OpenGL state */
/* Modelview Matrix and stack for push/pop */
XMat4f ModelViewMatrix;
std::vector<XMat4f> ModelViewMatrixStack;

/* Projection Matrix and stack for push/pop */
XMat4f ProjectionMatrix;
std::vector<XMat4f> ProjectionMatrixStack;

/* The current matrix and current stack */
XMat4f *currentMatrix = &ModelViewMatrix;
std::vector<XMat4f> *currentMatrixStack = &ModelViewMatrixStack;

/* apple a viewing transformation to the current matrix */
void _gluLookAt(GLdouble eyeX, GLdouble eyeY, GLdouble eyeZ,
				GLdouble centerX, GLdouble centerY, GLdouble centerZ,
				GLdouble upX, GLdouble upY, GLdouble upZ) {
	
	XMat4f m;
	
	/* YOUR CODE HERE */
	/* Implement the viewing transform by modifying
	 the matrix m. The eye represents the location of
	 the viewer, the center represents a point in
	 front of the viewer, and up represents a point
	 above the view */

	XVec3d v[3]; // The 3 bases of the view coordinates
	v[2] = -XVec3d(centerX - eyeX, centerY - eyeY, centerZ - eyeZ);
	v[0] = -v[2].cross(XVec3d(upX, upY, upZ));
	v[1] = -v[0].cross(v[2]);

	XVec3d d; // Displacement
	XVec3d eye(eyeX, eyeY, eyeZ);
	d(0) = - v[0].dot(eye);
	d(1) = - v[1].dot(eye);
	d(2) = - v[2].dot(eye);

	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++)
			m(i, j) = v[i](j);
		m(i, 3) = d(i);
	}
	
	/*cout << "eye" << endl;
	cout << eyeX << " " << eyeY << " " << eyeZ << endl << endl;
	cout << "center" << endl;
	cout << centerX << " " << centerY << " " << centerZ << endl << endl;
	cout << "up" << endl;
	cout << upX << " " << upY << " " << upZ << endl << endl;
	cout << "displacement" << endl;
	cout << d << endl << endl;;
	cout << "vector" << endl;
	for (int i = 0; i < 3; i++) {
		cout << v[i] << endl;
	}
	cout << endl << m << endl;*/

	*currentMatrix = *currentMatrix * m;
}

/* apply perspective transformation to the current matrix */
void _glFrustum(float left, float right, float bottom, float top, float near, float far) {
	/* near and far _should_ be specified as positive values, but
	 take the absolute value in case, and by OpenGL specification. */
	near = fabs(near);
	far = fabs(far);
	
	XMat4f m;
	
	/* YOUR CODE HERE */
	/* Implement the perspective transform by modifying
	 the matrix m. */
	
	m(0, 0) = 2 * near / (right - left);
	m(1, 1) = 2 * near / (top - bottom);
	m(2, 2) = -(near + far) / (far - near);
	m(0, 2) = (right + left) / (right - left);
	m(1, 2) = (top + bottom) / (top - bottom);
	m(2, 3) = -2 * far * near / (far - near);
	m(3, 2) = -1;
	m(3, 3) = 0;

	cout << m << endl << endl;
	printf("%f %f %f %f %f %f\n", left, right, bottom, top, near, far);

	*currentMatrix = *currentMatrix * m;
}

void _glVertex3f(float x, float y, float z) {
	XVec4f v(x, y, z, 1.0);
	//cout << v << endl << endl;
	/*cout << ProjectionMatrix << endl << endl;
	cout << ModelViewMatrix << endl << endl;*/
	
	v =  ProjectionMatrix * ModelViewMatrix * v;
	
	/* YOUR CODE HERE */
	/* What do you have to do after multiplying
	 by the projection matrix? */
	for (int i = 0; i < 3; i++)
		v(i) = v(i) / v(3);
	//cout << v << endl << endl;
	
	glVertex3fv(v);
}

void _glNormal3f(float x, float y, float z) {
	XVec4f v(x, y, z, 1.0);
	
	/* Update the vertex with the current modelview matrix */
	v = ModelViewMatrix.inverse().transpose() * v;
	
	glNormal3fv(v);
}

/* Change the current matrix mode */
void _glMatrixMode(GLenum mode) {
	if( mode == GL_PROJECTION ) {
		currentMatrix = &ProjectionMatrix;
		currentMatrixStack = &ProjectionMatrixStack;
	}
	else {
		currentMatrix = &ModelViewMatrix;
		currentMatrixStack = &ModelViewMatrixStack;
	}
}

/* Reset the current matrix */
void _glLoadIdentity() {
	currentMatrix->toIdentity();
}

/* Save the current matrix */
void _glPushMatrix() {
	currentMatrixStack->push_back(ModelViewMatrix);
}

/* restore the previous matrix */
void _glPopMatrix() {
	*currentMatrix = currentMatrixStack->back();
	currentMatrixStack->pop_back();
}

/* apply a translation to the current matrix */
void _glTranslatef(float x, float y, float z) {
	XMat4f trans;
	trans.translation(XVec3f(x, y, z));
	*currentMatrix = *currentMatrix * trans;
}

/* apply a scale to the current matrix */
void _glScalef(float x, float y, float z) {
	XMat4f scale;
	scale.scale(XVec3f(x, y, z));
	*currentMatrix = *currentMatrix * scale;
}

/* apply a rotation to the current matrix */
void _glRotatef(float a, float x, float y, float z) {
	XMat4f rotation;
	rotation.rotate(XVec4f(a, x, y, z));
	*currentMatrix = *currentMatrix * rotation;
}

