/*  robot.c*/
/*  modified by Chaolun Wang */
/*
	keyboard controls:
	s/S:	Positive/negative shoulder rotation
	e/E:	Positive/negative elbow rotation
	O/C:	Open/Close the hand -- YOU GET TO IMPLEMENT THIS
	
	x/X:	Positive/negative X-axis shift of the model
	y/Y:	Positive/negative Y-axis shift of the model
	UP/DOWN ARROWS:	(zoom) Z-axis shift of the model
	
	LEFT/RIGHT ARROWS:	Y-axis rotations
	PAGE UP/DOWN:	X-axis rotations
	ESC:	exit
*/
#include<stdio.h>
#include<GL/gl.h>
#include<GL/glu.h>
#include<GL/glut.h>

#define ESC 27
#define MAXANGLE 120                  /*maximum elbow angle was setted as 120*/
#define MAXFIGANGLE 60				  /*maximum fingure angle was setted as 120(between two fingures)*/

static GLfloat red[]={1,0,0},green[]={0,1,0},blue[]={0,0,1}; 
static int shoulder=0,elbow=0,fingure=0;
static int xrot=0,yrot=0,zrot=0;
static GLfloat xshift=-1.0,yshift=0.0,zoom=-3.0;

/* some basic GL initialization */
void init()
{
	glClearColor(0.0,0.0,0.0,0.0);
	glShadeModel(GL_FLAT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
}

/* The display callback */
void draw_stuff()
{
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	glPushMatrix();
	glTranslatef(xshift,yshift,zoom);
	glRotatef((GLfloat)xrot,1.0,0.0,0.0);
	glRotatef((GLfloat)yrot,0.0,1.0,0.0);
	glRotatef((GLfloat)zrot,0.0,0.0,1.0);
	
	glColor3fv(red);
	glPushMatrix();
	glTranslatef(-1.0,0.0,0.0);
	glRotatef((GLfloat)shoulder,0.0,0.0,1.0);
	glTranslatef(1.0,0.0,0.0);
	glPushMatrix();
	glScalef(2.0,0.4,1.0);
	glutSolidCube(1.0);
	glPopMatrix();

	glColor3fv(green);
	glTranslatef(1.0,0.0,0.0);
	glRotatef((GLfloat)elbow,0.0,0.0,1.0);
	glTranslatef(1.0,0.0,0.0);
	glPushMatrix();
	glScalef(2.0,0.4,1.0);
	glutSolidCube(1.0);
	glPopMatrix();
	
	/* Put the "hand" code here...*/
	glColor3fv(blue);                            /*upper fingure*/
	glTranslatef(1.0,0.0,0.0);
	glTranslatef(0.0,0.1,0.0);
	glRotatef((GLfloat)fingure,0.0,0.0,1.0);
	glTranslatef(0.52,0.0,0.0);
	glPushMatrix();
	glScalef(1,0.2,0.2);
	glutSolidCube(1.0);
	glPopMatrix();
	
	glColor3fv(blue);                             /*lower fingure*/
	glTranslatef(-0.52,0.0,0);
	glRotatef(360-(GLfloat)fingure,0.0,0.0,1.0);
	glTranslatef(0.0,-0.2,0.0);
	glRotatef(360-(GLfloat)fingure,0.0,0.0,1.0);
	glTranslatef(0.52,0.0,0.0);
	glPushMatrix();
	glScalef(1,0.2,0.2);
	glutSolidCube(1.0);
	glPopMatrix();
	
	

	glPopMatrix();
	glPopMatrix();
	glutSwapBuffers();
}

/* the window-reshape callback */
void reshape(int w, int h)
{
	glViewport(0,0,(GLsizei)w,(GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(65.0,(GLfloat)w/(GLfloat)h,1.0,20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0,0.0,-5.0);
}

/* The basic keyboard controls, callback for glutKeyboardFunc() */
void kb(unsigned char key, int x, int y)
{
	switch(key)
	{
		case 's':
			if(shoulder<MAXANGLE )
				shoulder=(shoulder+5)%360;
			glutPostRedisplay();
			break;
		case 'S':
			if(shoulder>0-MAXANGLE)
				shoulder=(shoulder-5)%360;
			glutPostRedisplay();
			break;
		case 'e':
			if(elbow<MAXANGLE )
				elbow=(elbow+5)%360;
			glutPostRedisplay();
			break;
		case 'E':
			if(elbow>0-MAXANGLE)
				elbow=(elbow-5)%360;
			glutPostRedisplay();
			break;
		case 'C':
			/* close the "hand" */
			if(fingure>0 )
				fingure=(fingure-5)%360;
			glutPostRedisplay();		
			break;
		case 'O':
			/* open the "hand" */
			if(fingure<MAXFIGANGLE )
				fingure=(fingure+5)%360;
			glutPostRedisplay();

			break;
		case 'x':
			xshift+=.25;
			glutPostRedisplay();
			break;
		case 'X':
			xshift-=.25;
			glutPostRedisplay();
			break;
		case 'y':
			yshift+=.25;
			glutPostRedisplay();
			break;
		case 'Y':
			yshift-=.25;
			glutPostRedisplay();
			break;
		case 'r':
		case 'R':
			xrot=yrot=zrot=shoulder=elbow=0;
			xshift=-1.0;
			yshift=0.0;
			zoom=-3.0;
			glutPostRedisplay();
			break;
		case 'z':
			zrot=(zrot-1)%360;
			glutPostRedisplay();
			break;
		case 'Z':
			zrot=(zrot+1)%360;
			glutPostRedisplay();
			break;
		case ESC:
			exit(0);
			break;
		default:
			break;
	}
}

/* The "special" keyboard controls, callback for glutSpecialFunc() */
void skb(int key, int x, int y)
{
	switch(key)
	{
		case GLUT_KEY_UP:
			zoom+=1;
			glutPostRedisplay();
			break;
		case GLUT_KEY_DOWN:
			zoom-=1;
			glutPostRedisplay();
			break;
		case GLUT_KEY_RIGHT:
			yrot=(yrot-1)%360;
			glutPostRedisplay();
			break;
		case GLUT_KEY_LEFT:
			yrot=(yrot+1)%360;
			glutPostRedisplay();
			break;
		case GLUT_KEY_PAGE_UP:
			xrot=(xrot-1)%360;
			glutPostRedisplay();
			break;
		case GLUT_KEY_PAGE_DOWN:
			xrot=(xrot+1)%360;
			glutPostRedisplay();
			break;
		default:
			break;
	}
}

int main(int argc, char **argv)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
	glutInitWindowSize(700,300);
	glutInitWindowPosition(100,50);
	glutCreateWindow("foo");
	init();
	glutDisplayFunc(draw_stuff);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(kb);
	glutSpecialFunc(skb);
	glutMainLoop();
	return 0;
}
