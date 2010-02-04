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
	
	*currentMatrix = *currentMatrix * m;
}

void _glVertex3f(float x, float y, float z) {
	XVec4f v(x, y, z, 1.0);
	
	v =  ProjectionMatrix * ModelViewMatrix * v;
	
	/* YOUR CODE HERE */
	/* What do you have to do after multiplying
	 by the projection matrix? */
	
	glVertex3fv(v);
}

void _glNormal3f(float x, float y, float z) {
	XVec4f v(x, y, z, 1.0);
	
	v = (ProjectionMatrix * ModelViewMatrix).inverse().transpose() * v;
	
	/* YOUR CODE HERE */
	/* What do you have to do after multiplying
	 by the projection matrix? */
	
	glNormal3f(x, y, z);
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

