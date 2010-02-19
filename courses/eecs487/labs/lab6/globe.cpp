
#include <iostream>
#include <cmath>
#include "ltga.h"
#include "xvec.h"

extern "C" {
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

}

using namespace std;

#define PI 3.14159265

bool init(const char *const texture_image_filename);
void disp(void);
void reshape(int w, int h);
void keyb(unsigned char key, int x, int y);
void print_help(void);


int window_handle;
GLdouble screen_width=800, screen_height=600;
bool in_texture_mode = true;


int main(int argc, char *argv[])
{
	if(argc != 2) {
		cerr << "Usage: globe <texture_image_filename>.tga" << endl;
		return -1;
	}
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize((int)screen_width, (int)screen_height);
	window_handle = glutCreateWindow("Lab6: The World Ain't Flat! [Textures]");
	if(!init(argv[1])) return -1;
	glutDisplayFunc(disp);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyb);
	glutMainLoop();

	return 0;
}



GLfloat position[] = { 0.0, 0.0, 2.0, 1.0 };
GLfloat dir[] = {0.0, 0.0, -1.0};
GLfloat diffuse[] = { 0.8, 0.5, 0.6, 1.0 };
GLfloat specular[] = {0.0, 0.8, 0.0, 1.0 };
GLfloat mat_specular[] = { .7, 1.0, 4.0, 1.0 };
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

GLuint sphere;
GLUquadricObj *quadratic;

const char * envImageNames[] = {"cm_left.tga",
	"cm_right.tga",
	"cm_top.tga",
	"cm_bottom.tga",
	"cm_front.tga",
	"cm_back.tga"};
string imgDir = "./images/";
GLuint envTexNames[6];

void loadEnvImages(void)
{
	glGenTextures(6, envTexNames);
	for (int i = 0; i < 6; i++) {
		loadTex(imgDir+envImageNames[i], envTexNames[i]);
	}
}

GLuint globeTexName;
const char * globeImageName = "images/world-map-country-names.tga";
void loadGlobeImage()
{
	glGenTextures(1, &globeTexName);
	loadTex(globeImageName, globeTexName);
}
	
bool init(const char *const texture_image_filename)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	// TODO: initialize texture and texture parameters.
	loadEnvImages();
	
	// TODO: Create sphere-drawing display list. Map texture coordinates as you create the sphere.

	quadratic=gluNewQuadric();			
	gluQuadricNormals(quadratic, GLU_SMOOTH);	// Create Smooth Normals 
	gluQuadricTexture(quadratic, GL_TRUE);		// Create Texture Coords

	// Set up the shpere:
	GLint slices = 32;
	GLint stacks = 32;
	GLuint radius = 1;
	GLfloat dTheta = 2 * PI / slices;
	GLfloat dPhi = PI / stacks;

	loadGlobeImage();

	GLfloat ds = 1.0f / slices;
	GLfloat dt = 1.0f / stacks;

	sphere = glGenLists(1);
	glNewList(sphere, GL_COMPILE);
	//glColor3f(1.0f, 1.0f, 1.0f);

	//gluSphere(quadratic, radius, 32, 32);
	glBindTexture(GL_TEXTURE_2D, globeTexName);
	glBegin(GL_QUADS);
	for (int i = 0; i < slices; i++) {
		GLfloat theta0 = dTheta * i;
		GLfloat theta1 = theta0 + dTheta;
		GLfloat s0 = ds * i;
		GLfloat s1 = s0 + ds;
		for (int j = 0; j < stacks; j++) {
			GLfloat phi0 = dPhi * j - PI / 2;
			GLfloat phi1 = phi0 + dPhi;
			GLfloat t0 = dt * j;
			GLfloat t1 = t0 + dt;

			glTexCoord2f(s0, t0); 
			glVertex3fv(radius*XVec3f(cos(phi0)*cos(theta0), cos(phi0)*sin(theta0), sin(phi0)));
			glTexCoord2f(s1, t0);
			glVertex3fv(radius*XVec3f(cos(phi0)*cos(theta1), cos(phi0)*sin(theta1), sin(phi0)));
			glTexCoord2f(s1, t1);
			glVertex3fv(radius*XVec3f(cos(phi1)*cos(theta1), cos(phi1)*sin(theta1), sin(phi1)));
			glTexCoord2f(s0, t1);
			glVertex3fv(radius*XVec3f(cos(phi1)*cos(theta0), cos(phi1)*sin(theta0), sin(phi1)));
		}
	}
	glEnd();
			
	glEndList();

	glEnable(GL_TEXTURE_2D);						// Enable Texture Mapping
	return init_lights();
}




void disp(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
	//GLfloat hu = u * 0.5f; // Half unit
	//Draw the cube

	/*// Right
	glBindTexture(GL_TEXTURE_2D, envTexNames[0]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(u, -u, -u);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(u, -u, u);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(u, u, u);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(u, u, -u);
	glEnd();

	//Left
	glBindTexture(GL_TEXTURE_2D, envTexNames[1]);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-u, -u, u);
	glTexCoord2f(1.0f, 0.0f); glVertex3f(-u, -u, -u);
	glTexCoord2f(1.0f, 1.0f); glVertex3f(-u, u, -u);
	glTexCoord2f(0.0f, 1.0f); glVertex3f(-u, u, u);
	glEnd();*/

	for (int i = 0; i < 6; i++) {
		glBindTexture(GL_TEXTURE_2D, envTexNames[i]);
		glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 0.0f); glVertex3fv(cubeFaces[i][0]);
		glTexCoord2f(1.0f, 0.0f); glVertex3fv(cubeFaces[i][1]);
		glTexCoord2f(1.0f, 1.0f); glVertex3fv(cubeFaces[i][2]);
		glTexCoord2f(0.0f, 1.0f); glVertex3fv(cubeFaces[i][3]);
		glEnd();
	}



	// TODO: call sphere-drawing display list.
/*	glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
	glTexGeni(GL_R, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_R);*/
	glPushMatrix();
	glRotatef(-90.0f, 1.0f, 0.0f, 0.0f);
	glCallList(sphere);
	glPopMatrix();
/*	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glDisable(GL_TEXTURE_GEN_R);*/

	glutSwapBuffers();
}




void reshape(int w, int h)
{
	/* Save new screen dimensions */
	screen_width = (GLdouble) w;
	screen_height = (GLdouble) h;

	/* Instruct Open GL to use the whole window for drawing */
	glViewport(0, 0, (GLsizei) w, (GLsizei) h);

	/* Set the perspective - later in this course */
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(55, screen_width/screen_height, 0.1, 25);
}


void keyb(unsigned char key, int x, int y)
{
	/* set the matrix mode to model-view */
	glMatrixMode(GL_MODELVIEW);

	/* Get info about Alt, Ctrl, Shift modifier keys */
	int mod;
	mod = glutGetModifiers();

	/* Removing the glLight functions after the modeling/viewing transforms
	 * will keep the light position and direction fixed in eye coordinates.
	 * Preserving them will make the light change position and direction
	 * with the transforms.
	 */


	switch((char)key) {

		case 'h':
		case '?':
			print_help();
			break;

		case 'x':
			if(mod & GLUT_ACTIVE_ALT) glRotatef(5,1,0,0);
			else glTranslatef(0.1,0, 0);
			glLightfv(GL_LIGHT0, GL_POSITION, position); 
			glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
			glutPostRedisplay();
			break;

		case 'X':
			if(mod & GLUT_ACTIVE_ALT) glRotatef(-5,1,0,0);
			else glTranslatef(-0.1,0,0);
			glLightfv(GL_LIGHT0, GL_POSITION, position); 
			glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
			glutPostRedisplay();
			break;

		case 'y':
			if(mod & GLUT_ACTIVE_ALT) glRotatef(5,0,1,0); 
			else glTranslatef(0,0.1, 0);
			glLightfv(GL_LIGHT0, GL_POSITION, position); 
			glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
			glutPostRedisplay();
			break;

		case 'Y':
			if(mod & GLUT_ACTIVE_ALT) glRotatef(-5,0,1,0); 
			else glTranslatef(0,-0.1,0);
			glLightfv(GL_LIGHT0, GL_POSITION, position); 
			glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
			glutPostRedisplay();
			break;

		case 'z':
			if(mod & GLUT_ACTIVE_ALT) glRotatef(5,0,0,1);
			else glTranslatef(0,0,0.1);
			glLightfv(GL_LIGHT0, GL_POSITION, position); 
			glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
			glutPostRedisplay();
			break;

		case 'Z':
			if(mod & GLUT_ACTIVE_ALT) glRotatef(-5,0,0,1);
			else glTranslatef(0,0,-0.1);
			glLightfv(GL_LIGHT0, GL_POSITION, position); 
			glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
			glutPostRedisplay();
			break;

		case '.':
			in_texture_mode = !in_texture_mode;
			if(in_texture_mode) {
				glDisable(GL_LIGHTING);
				glEnable(GL_TEXTURE_2D); 
			}
			else {
				glEnable(GL_LIGHTING);
				glDisable(GL_TEXTURE_2D);
			}
			glutPostRedisplay();
			break;


		case 'q':
		case 'Q':
		case 27:
			glutDestroyWindow(window_handle);
			break;
	}

}


void print_help(void)
{
	cerr << "\nEECS 487 F08 Lab 70: OpenGL Lighting\n";
	cerr << "----------------------------------------\n\n";
	cerr << "Navigate by translating and rotating along the axes" << endl;

	cerr << "x    : Move forward along the x-axis by a distance of 0.1\n";
	cerr << "X    : Move backward along the x-axis by a distance of 0.1\n";
	cerr << "ALT-x: Rotate about x-axis by 5 degrees\n";
	cerr << "ALT-X: Rotate about x-axis by -5 degrees\n";

	cerr << "y    : Move forward along the y-axis by a distance of 0.1\n";
	cerr << "Y    : Move backward along the y-axis by a distance of 0.1\n";
	cerr << "ALT-y: Rotate about y-axis by 5 degrees\n";
	cerr << "ALT-Y: Rotate about y-axis by -5 degrees\n";

	cerr << "z    : Move forward along the z-axis by a distance of 0.1\n";
	cerr << "Z    : Move backward along the z-axis by a distance of 0.1\n";
	cerr << "ALT-z: Rotate about z-axis by 5 degrees\n";
	cerr << "ALT-Z: Rotate about z-axis by -5 degrees\n\n";

	cerr << "  .  : Toggle between lighting and texture modes" << endl;
}

