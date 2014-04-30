/*
FILE: main.cpp
ABOUT: GLUTGameOfLife3D
AUTHOR: kevin
EMAIL: keglme00@hs-esslingen.de
DATE: 02.01.2012
 */

#include <GL/glut.h>    // Header File For The GLUT Library
#include <GL/gl.h>	// Header File For The OpenGL32 Library
#include <GL/glu.h>	// Header File For The GLu32 Library
#include <unistd.h>     // Header file for sleeping.
#include <stdio.h>      // Header file for standard file i/o.
#include <stdlib.h>     // Header file for malloc/free.

#include <pthread.h>


#include "GOL3D.h"

/* ascii codes for various special keys */
#define ESCAPE 27
#define PAGE_UP 73
#define PAGE_DOWN 81
#define UP_ARROW 72
#define DOWN_ARROW 80
#define LEFT_ARROW 75
#define RIGHT_ARROW 77

/* The number of our GLUT window */
int window;

/* lighting on/off (1 = on, 0 = off) */
int light;

/* L pressed (1 = yes, 0 = no) */
int lp;

/* F pressed (1 = yes, 0 = no) */
int fp;



gliewe::GOL3D* gol;

int oldMouseX = 0;
int oldMouseY = 0;

float rotx = 0;
float roty = 0;
float zoom = 0.05;
bool run = true;


/* A general OpenGL initialization function.  Sets all of the initial parameters. */
GLvoid InitGL(GLsizei Width, GLsizei Height)	// We call this right after our OpenGL window is created.
{

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);						// Black Background
	glClearDepth(1.0f);											// Depth Buffer Setup
	glDepthFunc(GL_LEQUAL);										// Type Of Depth Testing
	glEnable(GL_DEPTH_TEST);									// Enable Depth Testing
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// Enable Alpha Blending (disable alpha testing)
	glEnable(GL_BLEND);											// Enable Blending       (disable alpha testing)
//	glAlphaFunc(GL_GREATER,0.1f);								// Set Alpha Testing     (disable blending)
//	glEnable(GL_ALPHA_TEST);									// Enable Alpha Testing  (disable blending)
	glEnable(GL_TEXTURE_2D);									// Enable Texture Mapping
	glEnable(GL_CULL_FACE);
	//glEnable(GL_LIGHTING);


    //// track material ambient and diffuse from surface color, call it before glEnable(GL_COLOR_MATERIAL)
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_COLOR_MATERIAL);


    // set up light colors (ambient, diffuse, specular)
    GLfloat lightKa[] = {.2f, .2f, .2f, 1.0f};  // ambient light
    GLfloat lightKd[] = {.7f, .7f, .7f, 1.0f};  // diffuse light
    GLfloat lightKs[] = {1, 1, 1, 1};           // specular light
    glLightfv(GL_LIGHT0, GL_AMBIENT, lightKa);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightKd);
    glLightfv(GL_LIGHT0, GL_SPECULAR, lightKs);

    //// position the light
    float lightPos[4] = {0, 0, 20, 1}; // positional light
	//float lightPos[4] = {0, 0, -1, -8.5f}; // positional light
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos);

    //glEnable(GL_LIGHT0);                        // MUST enable each light source after configuration
}

/* The function called when our window is resized (which shouldn't happen, because we're fullscreen) */
GLvoid ReSizeGLScene(GLsizei Width, GLsizei Height)
{
    if (Height==0)				// Prevent A Divide By Zero If The Window Is Too Small
	Height=1;

    glViewport(0, 0, Width, Height);		// Reset The Current Viewport And Perspective Transformation

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(45.0f,(GLfloat)Width/(GLfloat)Height,0.1f,100.0f);
    glMatrixMode(GL_MODELVIEW);
}

/* The main drawing function. */
void DrawGLScene(void)
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);		// Clear The Screen And The Depth Buffer
    glLoadIdentity();				// Reset The View

    glTranslatef(0.0f,0.0f,-5);                  // move z units out from the screen.

    glRotatef(roty,1.0f,0.0f,0.0f);		// Rotate On The X Axis
    glRotatef(rotx,0.0f,1.0f,0.0f);		// Rotate On The Y Axis

    glColor3f(1,0,0);
    glScalef(zoom,zoom,zoom);

    gol->Draw();

    // since this is double buffered, swap the buffers to display what just got drawn.
    glutSwapBuffers();
}


/* The function called whenever a normal key is pressed. */
void keyPressed(unsigned char key, int x, int y)
{
    /* avoid thrashing this procedure */
    usleep(100);

    //printf("Key : %d\n",key);

    switch (key) {
    case ESCAPE: // kill everything.
	/* shut down our window */
	glutDestroyWindow(window);

	/* exit the program...normal termination. */
	exit(1);
	break; // redundant.

    case 76:
    case 108: // switch the lighting.
	printf("L/l pressed; light is: %d\n", light);
	light = light ? 0 : 1;              // switch the current value of light, between 0 and 1.
	printf("Light is now: %d\n", light);
	if (!light) {
	    glDisable(GL_LIGHTING);
	} else {
	    glEnable(GL_LIGHTING);
	}
	break;

    case 70:
    case 102: // switch the filter.
    	gol->Clear();
	break;

    default:
	break;
    }
}

/* The function called whenever a normal key is pressed. */
void specialKeyPressed(int key, int x, int y)
{
    /* avoid thrashing this procedure */
    usleep(100);

    //printf("Special Key : %d\n",key);

    switch (key) {
    case GLUT_KEY_PAGE_UP: // move the cube into the distance.
	break;

    case GLUT_KEY_PAGE_DOWN: // move the cube closer.;
	break;

    case GLUT_KEY_UP: // decrease x rotation speed;
    	zoom *= 1.2f;
	break;

    case GLUT_KEY_DOWN: // increase x rotation speed;
    	zoom /= 1.2f;
	break;

    case GLUT_KEY_LEFT: // decrease y rotation speed;
	break;

    case GLUT_KEY_RIGHT: // increase y rotation speed;
	break;

    default:
	break;
    }
}

void MouseMove(int x, int y)
{
	//printf("Mouse Move : X=%d Y=%d\n",x,y);

	int dx = x - oldMouseX;
	int dy = y - oldMouseY;

	oldMouseX = x;
	oldMouseY = y;

	rotx += 0.5f * dx;
	roty += 0.5f * dy;
}
void MouseMovePassive(int x, int y)
{
	//printf("Mouse Move Passive: X=%d Y=%d\n",x,y);
	oldMouseX = x;
	oldMouseY = y;
}

void* calcLoop(void* v)
{
	while(run)
	{
		gol->Step();
		sleepx(300);
	}
	return 0;
}

int main(int argc, char **argv)
{

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
    glutInitWindowSize(1000, 800);
    glutInitWindowPosition(0, 0);
    window = glutCreateWindow("3D Game of Live");

    glutDisplayFunc(&DrawGLScene);
    //glutFullScreen();
    glutIdleFunc(&DrawGLScene);
    glutReshapeFunc(&ReSizeGLScene);
    glutKeyboardFunc(&keyPressed);
    glutSpecialFunc(&specialKeyPressed);

    glutMotionFunc(&MouseMove);
    glutPassiveMotionFunc(&MouseMovePassive);

    InitGL(800, 600);

    gol = new gliewe::GOL3D(50,50,50);
    gol->Clear();
    printf("Dump Field:\n");
    gol->DumpFild();

    pthread_t thread1;
    pthread_create( &thread1, NULL, calcLoop, 0);

    glutMainLoop();

    pthread_join( thread1, NULL);

    return 1;
}



