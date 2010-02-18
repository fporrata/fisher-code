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

#include "x3reader.h"
#include "x3.h"
#include "shaders.h"

using namespace std;

// Global constants (image dimensions).
const int WIDTH = 640;
const int HEIGHT = 480;

X3Scene* scene = NULL;

enum ShaderModeEnum {
	NO_SHADERS,
	INTERESTING_SHADERS,
	PHONG_SHADERS
} shader_mode = NO_SHADERS;

bool lmb_down = false;
bool rmb_down = false;
float phi_old = 0.0f;
float psi_old = 0.0f;
XVec3f position_old = XVec3f(0.0f, 0.0f, 0.0f);
XVec2i down_lmb;

GLhandleARB g_hpo_interesting; // interesting program object handle
GLhandleARB g_hpo_phong; // Phong program object handle


// Function prototypes.
void DisplayCallback();
void KeyboardCallback(unsigned char key, int x, int y);
void ResizeCallback(int w, int h);
void MouseCallback(int button, int state, int x, int y);
void MotionCallback(int x, int y);

int main(int argc, char** argv)
{
	if(argc<2) {
		cerr << "Usage: ./proj2 scene.x3d" << endl;
		return -1;
	}
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

	InitShaders("interesting", &g_hpo_interesting);
	InitShaders("phong", &g_hpo_phong);

	ifstream input_stream(argv[1]);
	X3Reader x3reader;
	scene = x3reader.Read(input_stream);

	glutDisplayFunc(DisplayCallback);
	glutKeyboardFunc(KeyboardCallback);
	glutReshapeFunc(ResizeCallback);
	glutMouseFunc(MouseCallback);
	glutMotionFunc(MotionCallback);

	glutMainLoop();

	return 0;
}

// This function is called when window needs to be rendered.
void DisplayCallback() {
	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch(shader_mode) {
		case NO_SHADERS:
			glUseProgramObjectARB(0);
			break;
		case INTERESTING_SHADERS:
			glUseProgramObjectARB(g_hpo_interesting);
			break;
		case PHONG_SHADERS:
			glUseProgramObjectARB(g_hpo_phong);
			break;
	}

	if(scene) {
		// setup the viewpoint cam

		// Setup lights.
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		int count_lights = 0;

		scene->SetupLights(&count_lights);

		// cout << count_lights << " lights in the scene." << endl;

		// If there are no lights in the scene file, setup a default light.
		if(count_lights==0) {
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
			XVec4f light_pos(1, 1, 0, 1);
			glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
			glEnable(GL_LIGHT0);
		}

		// Render scene.
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		glEnable(GL_LIGHTING);
		glEnable(GL_CULL_FACE);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_NORMALIZE);

		glShadeModel(GL_SMOOTH);

		glColor3f(1.0f, 0.0f, 0.0f);

		scene->Render();
	}

	glUseProgramObjectARB(0);

	glutSwapBuffers();
}

// This function is called when user presses a key.
void KeyboardCallback(unsigned char key, int x, int y) {
	switch(key) {
		case 27: // ESC key
			exit(0);
			break;
		case 'i':
			if(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader) {
				cout << "Loading interesting shaders." << endl;
				shader_mode = INTERESTING_SHADERS;
			} else {
				cerr << "shaders not supported." << endl;
			}
			break;
		case 'p':
			if(GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader) {
				cout << "Loading on per-pixel Phong shaders." << endl;
				shader_mode = PHONG_SHADERS;
			} else {
				cerr << "shaders not supported." << endl;
			}
			break;
		case 'n':
			shader_mode = NO_SHADERS;
			cout << "Fixed-function pipeline mode (no programmable shaders)." << endl;
			break;
	}
	glutPostRedisplay();
}

void ResizeCallback(int w, int h) {
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if(scene) {
		float aspect = float(w) / h;
		gluPerspective(45.0f, aspect, 0.1f, 1000.0f);
	}
}

// YOUR CODE HERE: modify the two functions below to enable zooming in and out 
// towards the center of the scene (using the right mouse button on vertical motion),
// and the tilting of the view (using the left mouse button on vertical motion).
// You will also need to add methods to the X3Viewpoint class.

// What if both of the bottons are down?
// Currently right click has higher priority.
void MouseCallback(int button, int state, int x, int y) {
	if(!scene)
		return;
	switch(button) {
		case GLUT_LEFT_BUTTON:
			rmb_down = false;
			if(state==GLUT_DOWN) {
				lmb_down = true;
				phi_old = scene->viewpoint()->phi();
				psi_old = scene->viewpoint()->psi();
				down_lmb = XVec2i(x, y);
			} else if(state==GLUT_UP){
				lmb_down = false;
			}
			break;
		case GLUT_RIGHT_BUTTON:
			lmb_down = false;
			if (state==GLUT_DOWN) {
				rmb_down = true;
				position_old = scene->viewpoint()->position();
				down_lmb = XVec2i(x, y);
			} else if(state==GLUT_UP) {
				rmb_down = false;
			}	
			break;
	}
}

void MotionCallback(int x, int y) {
	if(lmb_down) {
		scene->viewpoint()->set_phi(phi_old + 0.1f * (x - down_lmb(0)));
		scene->viewpoint()->set_psi(psi_old + 0.1f * (y - down_lmb(1)));
	}
	if(rmb_down) {
		scene->viewpoint()->set_position(position_old + 
				XVec3f(0.0f, 0.0f, 0.1f * (y - down_lmb(1))));
	}
	glutPostRedisplay();
}
