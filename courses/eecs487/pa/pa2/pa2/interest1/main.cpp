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
const int HEIGHT = 640;

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


void init() {
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_ALPHA_TEST);
	glEnable(GL_BLEND);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(55, WIDTH/HEIGHT, 0.1, 25);
	InitShaders("Interesting", &g_hpo_interesting);
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

const char * envImageNames[] = {"cm_left.tga",
	"cm_right.tga",
	"cm_top.tga",
	"cm_bottom.tga",
	"cm_front.tga",
	"cm_back.tga"};
string imgDir = "./images/";
GLuint envTexNames[6];

GLuint cubeTex;

void loadEnvImages(void)
{
	glGenTextures(6, envTexNames);
	for (int i = 0; i < 6; i++) {
		loadTex(imgDir+envImageNames[i], envTexNames[i]);
	}
}

void loadCubeTex()
{
	glGenTextures(1, &cubeTex);
	LTGA ltga[6];
	GLenum format[6];
	for (int i = 0; i < 6; i++) {
		ltga[i].LoadFromFile(imgDir+envImageNames[i]);
		switch (ltga[i].GetImageType()) {
			case itRGB: format[i] = GL_RGB; break;
			case itRGBA: format[i] = GL_RGBA; break;
			case itGreyscale: format[i] = GL_LUMINANCE; break;
			case itUndefined:
							  cerr << "Unknow image format\n";
							  break;
		}
	}

	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeTex);
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, format[0],
			ltga[0].GetImageWidth(), ltga[0].GetImageHeight(), 0, format[0], GL_UNSIGNED_BYTE, ltga[0].GetPixels());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, format[1],
			ltga[1].GetImageWidth(), ltga[1].GetImageHeight(), 0, format[1], GL_UNSIGNED_BYTE, ltga[1].GetPixels());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, format[2],
			ltga[2].GetImageWidth(), ltga[2].GetImageHeight(), 0, format[2], GL_UNSIGNED_BYTE, ltga[2].GetPixels());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, format[3],
			ltga[3].GetImageWidth(), ltga[3].GetImageHeight(), 0, format[3], GL_UNSIGNED_BYTE, ltga[3].GetPixels());
GLUquadricObj *quadratic;
	glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, format[4],
			ltga[4].GetImageWidth(), ltga[4].GetImageHeight(), 0, format[4], GL_UNSIGNED_BYTE, ltga[4].GetPixels());
	glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, format[5],
			ltga[5].GetImageWidth(), ltga[5].GetImageHeight(), 0, format[5], GL_UNSIGNED_BYTE, ltga[5].GetPixels());
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S,
			GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T,
			GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R,
			GL_REPEAT);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER,
			GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER,
			GL_LINEAR);
}
	




void initTex()
{
	loadEnvImages();
	loadCubeTex();

	glEnable(GL_TEXTURE_2D);
}

GLfloat position[] = { 4.0, 2.0, 2.0, 1.0 };
GLfloat dir[] = {0.0, 0.0, -1.0};
//GLfloat diffuse[] = { 0.8, 0.5, 0.6, 1.0 };
//GLfloat specular[] = {0.0, 0.8, 0.0, 1.0 };
//GLfloat mat_specular[] = { .7, 1.0, 4.0, 1.0 };
GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
GLfloat specular[] = {1.0, 1.0, 1.0, 1.0};
GLfloat mat_specular[] =  {1.0, 1.0, 1.0, 1.0};
GLfloat mat_shininess[] = { 50.0 };

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

void DisplayCallBack() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Draw background
	GLfloat u = 3.0f; // Unit
	GLfloat cubeFaces[6][4][3] = {{{u, -u, -u}, // Right
								   {u, -u, u}, 
								   {u, u, u},
								   {u, u, -u}},
								  {{-u, -u, u}, //Left
								   {-u, -u, -u},
								   {-u, u, -u},
								   {-u, u, u}},
								  {{-u, u, -u},
								   {u, u, -u},
								   {u, u, u}, //Top
								   {-u, u, u}},
								  {{-u, -u, u}, //Bottom
								   {u, -u, u},
								   {u, -u, -u},
								   {-u, -u, -u}},
								  {{u, -u, u}, // Front
								   {-u, -u, u},
								   {-u, u, u},
								   {u, u, u}},
								  {{-u, -u, -u}, // Back
								   {u, -u, -u},
								   {u, u, -u},
								   {-u, u, -u}}};
	for (int i = 0; i < 6; i++) {
		glBindTexture(GL_TEXTURE_2D, envTexNames[i]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3fv(cubeFaces[i][0]);
		glTexCoord2f(1.0f, 0.0f); glVertex3fv(cubeFaces[i][1]);
		glTexCoord2f(1.0f, 1.0f); glVertex3fv(cubeFaces[i][2]);
		glTexCoord2f(0.0f, 1.0f); glVertex3fv(cubeFaces[i][3]);
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
	//	glUseProgramObjectARB(0);
	glUseProgramObjectARB(g_hpo_interesting);
	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_NORMAL_MAP);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);
	/*  turn on cube map texturing  */
	glEnable(GL_TEXTURE_CUBE_MAP);

	GLUquadricObj *	quadratic=gluNewQuadric();			
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals 
	gluQuadricTexture(quadratic, GL_TRUE);		// Create Texture Coords

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	glPushMatrix();
	//glRotatef(180, 1.0f, 0.0f, 0.0f);
	//glRotatef(30, 0.0f, 1.0f, 0.0f);
	//glutSolidTeapot	(0.5);
	gluSphere(quadratic, 0.5, 128, 128);
	glPopMatrix();
	glutSwapBuffers();
}
