#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <string>

#include <GL/glew.h>
#ifdef __APPLE__
#include <GLUT/glut.h>
#else 
#include <GL/glut.h>
#endif

#include "shaders.h"
#include "ltga.h"

using namespace std;

// Global constants (image dimensions).
const int WIDTH = 640;
const int HEIGHT = 480;

GLuint backTexName;

enum ShaderModeEnum {
	NO_SHADERS,
	INTERESTING_SHADERS,
	PHONG_SHADERS
} shader_mode = NO_SHADERS;

GLhandleARB g_hpo_interesting; // interesting program object handle
GLhandleARB g_hpo_phong; // Phong program object handle
GLhandleARB g_hpo_glass;

void DisplayCallBack();
void initTex();
void init();
bool init_lights();

int main(int argc, char ** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(WIDTH, HEIGHT); 
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);

	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
		/* Problem: glewInit failed, something is seriously wrong. */
		cerr << "Error: " << glewGetErrorString(err) << endl;
	}
	cout <<  "Status: Using GLEW " << glewGetString(GLEW_VERSION) << endl;

	init();
	glutDisplayFunc(DisplayCallBack);

	glutMainLoop();

	return 0;
}

void DisplayCallBack() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw background
	GLfloat u = 5;
	GLfloat depth = -5;
	glBindTexture(GL_TEXTURE_2D, backTexName);
	glColor3f(1.0f, 1.0f, 1.0f);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-u, -u, depth);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(u, -u, depth);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(u, u, depth);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-u, u, depth);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	// Draw the teapot

	glUseProgramObjectARB(g_hpo_interesting);
	//glUseProgramObjectARB(g_hpo_phong);	
	//glUseProgramObjectARB(g_hpo_glass);
	//glUseProgramObjectARB(0);
	glColor4f(1.0f, 1.0f, 1.0f, 0.2f);
	glPushMatrix();
//	glTranslatef(0.5f, 0.0f, 0.0f);
	glRotatef(30, 1.0f, 0.0f, 0.0f);
	glRotatef(30, 0.0f, 1.0f, 0.0f);
	glutSolidTeapot	(0.5);
	glPopMatrix();
	
	glutSwapBuffers();
}

void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(55, WIDTH/HEIGHT, 0.1, 25);
	InitShaders("interesting", &g_hpo_interesting);
	InitShaders("phong", &g_hpo_phong);	
	InitShaders("Glass", &g_hpo_glass);
	init_lights();
	initTex();
}

void loadTex(const string & imageName, GLuint texName)
{
	LTGA ltga(imageName);
	GLenum format = GL_RGB;
	switch (ltga.GetImageType()) {
		case itRGB: format = GL_RGB; break;
		case itRGBA: format = GL_RGBA; break;
		case itGreyscale: format = GL_LUMINANCE; break;
		case itUndefined:
						  cerr << "Unknow image format\n";
						  break;
	}

	//Set the texture
	glBindTexture(GL_TEXTURE_2D, texName);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexImage2D(GL_TEXTURE_2D, 0, format, ltga.GetImageWidth(),
			ltga.GetImageHeight(), 0, format, GL_UNSIGNED_BYTE,
			ltga.GetPixels());
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}


void initTex()
{
	const char * imageName = "back1.tga";
	glGenTextures(1, &backTexName);
	loadTex(imageName, backTexName);
	glEnable(GL_TEXTURE_2D);
}

GLfloat position[] = { 3.0, 3.0, 3.0, 1.0 };
GLfloat dir[] = {0.0, 0.0, -1.0};
GLfloat diffuse[] = { 0.8, 0.5, 0.6, 1.0 };
//GLfloat specular[] = {0.0, 0.8, 0.0, 1.0 };
GLfloat specular[] = {1.0, 1.0, 0.0, 1.0};
GLfloat mat_specular[] = { .7, 1.0, 4.0, 1.0 };
GLfloat mat_shininess[] = { 25.0 };

bool init_lights(void)
{
	cerr << "Initializing lights." << endl;

	/* Lighting and material properties */

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	glMaterialfv(GL_FRONT, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);

	/* Initial camera location and orientation */
	gluLookAt(0, 0, 3, 0, 0, 0, 0, 1, 0);

	return true;
}

