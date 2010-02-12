
#include <iostream>
#include <cstdlib>
#include <cassert>

// Tell the C++ linker that the C functions in the headers below
// will not have mangled names.
// Name-mangling is necessary to implement function polymorphism.
extern "C" {
#include "GL/glew.h"

#ifdef __APPLE__
	#include <GLUT/glut.h>
#else
	#include <GL/glut.h>
#endif
	
}

#include "shaders.h"
#include "timer.h"
using namespace std;

// app initialization
void init(char *vpFile, char *fpFile);

// callback prototypes
void disp(void);
void reshape(int w, int h);
void keyb(unsigned char key, int x, int y);
void update(void);
void updateScreen(void);


/* Handle to the GLUT window */
int window_handle;

/* Prints the help when the 'h' key is pressed */
void print_help(void);

/* defined externally */
void initDrawItems(void);

Timer timer;
GLint time_id;


// Global variables
int screen_width=800, screen_height=600;

GLhandleARB spo = 0; // Handle to shader program object
bool usingShaders = false; // on-off flag

int main(int argc, char *argv[])
{
    if(argc != 3) {
        cerr << "Usage: lab5 vpname fpname" << endl;
        cerr << "       vpname = basename of vertex program file" << endl;
        cerr << "       fpname = basename of fragment program file" << endl;
        cerr << "  Vertex program file must have .vs extension. Fragment\n"
                "  program file must have .fs extension. Both files must\n"
                "  be specified without these extensions" << endl;
		return -1;
    }
    
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize((int)screen_width, (int)screen_height);
    window_handle = glutCreateWindow("Lab5: GLSL" /* title */ );
    
    // ** IMPORTANT ** Initialize GLEW
    GLenum err = glewInit();
    if (GLEW_OK != err)
        cerr << "Error: " << glewGetErrorString(err) << endl;
    else
        cout <<  "Status: Using GLEW " << glewGetString(GLEW_VERSION) << endl;
    
    // Initialize application - defined below
    init(argv[1], argv[2]);
    
    /* --- register callbacks for GUI events --- */
    glutDisplayFunc(disp);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyb);

	glutIdleFunc(updateScreen);
    
    /* Start the GLUT event loop */
    glutMainLoop();
    
    return 0;
}

void updateScreen()
{
	//cout << "Update is called\n";
	//Timer curtimer = timer;
	//curtimer.stop();
	//cout << curtimer.secs() << endl;
	glUniform1f(time_id, timer.secs());
	glutPostRedisplay();
}



void init(char *vpFile, char *fpFile)
{
  /* Lighting and material properties */
    
    GLfloat position[] = { 10.0, 10.0, 10.0, 1.0 };
    GLfloat dir[] = {-5.0, -5.0, -10.0};
    GLfloat diffuse[] = { 1.0, 1.0, 0.0, 1.0 };
    GLfloat specular[] = {0.0, 1.0, 0.0, 1.0 };
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT1, GL_POSITION, position);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
    glLightfv(GL_LIGHT1, GL_SPOT_DIRECTION, dir);
    glLightf(GL_LIGHT1, GL_SPOT_CUTOFF, 15);
    glLightf(GL_LIGHT1, GL_SPOT_EXPONENT, 2.0);
    
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT1);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);
    
  /* Background colour */
    glClearColor(0.0, 0.0, 0.0, 1.0);
    
  /* Initial camera location and orientation */
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(10, 10, 10, 5, 5, 0, 0, 1, 0);
    
  /* Initialize the scene */
    initDrawItems();
  
    // Load vertex and fragment programs - calls PA2 code
    assert(InitShaders(vpFile, fpFile, &spo));
	time_id = glGetUniformLocation(spo, "time");
	timer.start();
}


// display list ID
int drawID = 0;

void initDrawItems(void)
{
    drawID = glGenLists(1);
    glNewList(drawID, GL_COMPILE);
    glutSolidTeapot(3.0);
    glEndList();
}

void drawCanonicalAxes(void)
{
    // Note: got to disable lighting calculations here otherwise
    // we won't see the colours we've requested
    glDisable(GL_LIGHTING);
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glColor3f(1.0, 0.3, 0.3);
    glVertex3f(-100,0,0);
    glVertex3f(100,0,0);
    glColor3f(0.3, 1.0, 0.3);
    glVertex3f(0,-100,0);
    glVertex3f(0,100,0);
    glColor3f(0.3, 0.3, 1.0);
    glVertex3f(0,0,-100);
    glVertex3f(0,0,100);
    glEnd();
    glEnable(GL_LIGHTING);
}


/* Display callback */

void disp(void)
{
  /* Clear the background, clear the depth buffer  */
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    glMatrixMode(GL_MODELVIEW);

  /* Draw the teapot */
    glCallList(drawID);
    
	glUseProgramObjectARB(0);
  /* Draw the canonical axes */
    drawCanonicalAxes();
	if (usingShaders)
		glUseProgramObjectARB(spo);
    
  /* Force OpenGL to start drawing */
    glutSwapBuffers();
}




/* Keyboard callback */
void keyb(unsigned char key, int x, int y)
{
    glMatrixMode(GL_MODELVIEW);
    
  /* Get info about Alt, Ctrl, Shift modifier keys */
    int mod;
    mod = glutGetModifiers();
    
  /* TODO: implement key bindings
   *       x, X, ALT-x, ALT-X, y, Y, ALT-y, ALT-Y, z, Z, ALT-z, ALT-Z
   *
   *    (mod & GLUT_ACTIVE_ALT) is true if ALT was pressed when this function was called
   *    for a keypress.
   */
    
    switch((char)key) {
        
    case 'h':
        print_help();
        break;
        
    case 'x':
        if(mod & GLUT_ACTIVE_ALT) glRotatef(5,1,0,0);
        else glTranslatef(0.1,0, 0);
        glutPostRedisplay();
        break;
        
    case 'X':
        if(mod & GLUT_ACTIVE_ALT) glRotatef(-5,1,0,0);
        else glTranslatef(-0.1,0,0);
        glutPostRedisplay();
        break;
        
    case 'y':
        if(mod & GLUT_ACTIVE_ALT) glRotatef(5,0,1,0);
        else glTranslatef(0,0.1, 0);
        glutPostRedisplay();
        break;
        
    case 'Y':
        if(mod & GLUT_ACTIVE_ALT) glRotatef(-5,0,1,0);
        else glTranslatef(0,-0.1,0);
        glutPostRedisplay();
        break;
        
    case 'z':
        if(mod & GLUT_ACTIVE_ALT) glRotatef(5,0,0,1);
        else glTranslatef(0,0,0.1);
        glutPostRedisplay();
        break;
        
    case 'Z':
        if(mod & GLUT_ACTIVE_ALT) glRotatef(-5,0,0,1);
        else glTranslatef(0,0,-0.1);
        glutPostRedisplay();
        break;
        
    case 's':
    case 'S':
        usingShaders = !usingShaders;
        if(usingShaders)
            glUseProgramObjectARB(spo);
        else
            glUseProgramObjectARB(0);
        glutPostRedisplay();
        break;
        
    case 'q':
    case 27:
        glutDestroyWindow(window_handle);
        break;
    }
    
}


/* OpenGL resize callback */

void reshape(int w, int h)
{
  /* Save new screen dimensions */
    screen_width = w;
    screen_height = h;
    
  /* Instruct Open GL to use the whole window for drawing */
    glViewport(0, 0, (GLsizei) w, (GLsizei) h);
    
  /* Set the perspective - later in this course */
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(55, (GLdouble)w/(GLdouble)h, 0.1, 25);
}


void print_help(void)
{
    fprintf(stderr, "EECS 487 f08 Lab 6: Porcelain-geist (GLSL)\n");
    fprintf(stderr, "----------------------------------------\n\n");
    fprintf(stderr, "Put a ghost into the teapot\n");
    
    fprintf(stderr, "x: Move forward along the x-axis by a distance of 0.1\n");
    fprintf(stderr, "X: Move backward along the x-axis by a distance of 0.1\n");
    fprintf(stderr, "ALT-x: Rotate about x-axis by 5 degrees\n");
    fprintf(stderr, "ALT-X: Rotate about x-axis by -5 degrees\n");
    
    fprintf(stderr, "y: Move forward along the y-axis by a distance of 0.1\n");
    fprintf(stderr, "Y: Move backward along the y-axis by a distance of 0.1\n");
    fprintf(stderr, "ALT-y: Rotate about y-axis by 5 degrees\n");
    fprintf(stderr, "ALT-Y: Rotate about y-axis by -5 degrees\n");
    
    fprintf(stderr, "z: Move forward along the z-axis by a distance of 0.1\n");
    fprintf(stderr, "Z: Move backward along the z-axis by a distance of 0.1\n");
    fprintf(stderr, "ALT-z: Rotate about z-axis by 5 degrees\n");
    fprintf(stderr, "ALT-Z: Rotate about z-axis by -5 degrees\n");
    
    fprintf(stderr, "s: Toggle shaders\n");
}

