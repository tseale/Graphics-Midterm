// Homework Assignment 4
// main.cpp
// Written by John Quinn

#ifdef __APPLE__
	#include <GLUT/glut.h>
	#include <OpenGL/gl.h>
	#include <OpenGL/glu.h>
#else
	#include <GL/glut.h>
	#include <GL/gl.h>
	#include <GL/glu.h>
#endif

#include <math.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <ctype.h>
#include <fstream>
#include "camera.h"
#include "controlpts.h"

using namespace std;

// declare window size global variables
int windowWidth = 512, windowHeight = 512;

// bezier curve points.
vector<controlpts> points; // holds points read in from file
int numPoints;   // holds total number of control points

// controls motion of animated sphere
float step=0.0;
int pcount=0;

// declare pi as a floating point global var
float pi = 3.14159;

// declare global variables to handles mouse interaction
GLint leftClick, rightClick;    // holds state of left and right mouse buttons.
float oldx=0, oldy=0;    // holds previous x and y position of the mouse

// declare global camera object
camera myCam(20.0, 2.00, 1.80,0,0,0);  // declares camera object

// draws axes for reference
void drawAxes() {

    glDisable(GL_LIGHTING);

	// draw our axes
    glBegin(GL_LINES); {
		// set color white.
        glColor3f(1,1,1);
        
        // set up the x axes;
        for (int i=-10; i<=10; i++)
         {
           glVertex3f(-10,0,i);
           glVertex3f(10,0,i);
         }
        
        // set up z axes
        for (int i=-10; i<=10; i++)
         {
           glVertex3f(i,0,-10);
           glVertex3f(i,0,10);
         }
        
    }; glEnd();

    glEnable(GL_LIGHTING);
}

// function that draws the spheres that reperesent each control point.  drawControlPoints();
void drawControlPoints()
{
    // set color to white for drawing spheres at control points
	glColor3f(1,1,1);
		
	// draw spheres at control points
	for (int i=0; i<points.size(); i++)
	{
		GLUquadricObj* temp=gluNewQuadric();  // new sphere
		glPushMatrix();{  // push matrix
			glTranslatef(points[i].getX(),points[i].getY(),points[i].getZ()); // translate to appriate point
			gluSphere(temp,.25,10,10);  // draw sphere
		}; glPopMatrix(); // pop matrix
	}

}

// draws yellow lines between control point spheres that form the control cage
void drawControlCage()
{
	// set color to yellow
    glColor3f(1,1,0);

	glBegin(GL_LINE_STRIP); {
		for (int i=0; i<points.size(); i++)
		{
			glVertex3f(points[i].getX(),points[i].getY(),points[i].getZ());
		}	
	}; glEnd();

}

// draws a bezier curve based on control points
void drawBezierCurve(controlpts p0, controlpts p1, controlpts p2, controlpts p3, double tstep)
{
	// set color to blue
	glColor3f(0,0,1);
	
	// calculate x coefficients
	float xa= -1 * p0.getX() +3*p1.getX() - 3*p2.getX() +p3.getX();
	float xb= 3*p0.getX() - 6*p1.getX() +3*p2.getX();
	float xc= -3*p0.getX() + 3*p1.getX();
	float xd= p0.getX();
	
	// calculate y coefficients
	float ya= -1 * p0.getY() +3*p1.getY() -3*p2.getY() +p3.getY();
	float yb= 3*p0.getY() - 6*p1.getY() +3*p2.getY();
	float yc= -3*p0.getY() + 3*p1.getY();
	float yd= p0.getY();
	
	// calculate z coefficients
	float za= -1 * p0.getZ() +3*p1.getZ() -3*p2.getZ() +p3.getZ();
	float zb= 3*p0.getZ() - 6*p1.getZ() +3*p2.getZ();
	float zc= -3*p0.getZ() + 3*p1.getZ();
	float zd= p0.getZ();
	
	glBegin(GL_LINE_STRIP);{
	  for (double t=0.0; t<1.0; t+=tstep)
	  {
		 float xpos=xa*t*t*t+xb*t*t+xc*t+xd;
		 float ypos=ya*t*t*t+yb*t*t+yc*t+yd;
		 float zpos=za*t*t*t+zb*t*t+zc*t+zd;
		 glVertex3f(xpos,ypos,zpos);
	  }
	}; glEnd(); 
}

// animates a red sphere to follow the bezier curve.
void animateBezier()
{
    // get bezier points
	controlpts p0 =points[pcount];
	controlpts p1 =points[pcount+1];
	controlpts p2 =points[pcount+2];
	controlpts p3 =points[pcount+3];

  	// set color to red
  	glColor3f(1,0,0);
  
    // calculate x coefficients
	float xa= -1 * p0.getX() +3*p1.getX() - 3*p2.getX() +p3.getX();
	float xb= 3*p0.getX() - 6*p1.getX() +3*p2.getX();
	float xc= -3*p0.getX() + 3*p1.getX();
	float xd= p0.getX();
	
    // calculate y coefficients
	float ya= -1 * p0.getY() +3*p1.getY() -3*p2.getY() +p3.getY();
	float yb= 3*p0.getY() - 6*p1.getY() +3*p2.getY();
	float yc= -3*p0.getY() + 3*p1.getY();
	float yd= p0.getY();
	
	// calculate z coefficients
	float za= -1 * p0.getZ() +3*p1.getZ() -3*p2.getZ() +p3.getZ();
	float zb= 3*p0.getZ() - 6*p1.getZ() +3*p2.getZ();
	float zc= -3*p0.getZ() + 3*p1.getZ();
	float zd= p0.getZ();
	
	// calculate points
	float xpos=xa*step*step*step+xb*step*step+xc*step+xd;
	float ypos=ya*step*step*step+yb*step*step+yc*step+yd;
	float zpos=za*step*step*step+zb*step*step+zc*step+zd;

    // declare sphere object that will follow curve.
	GLUquadricObj* sobj=gluNewQuadric();  // new sphere

     glPushMatrix(); {
    	glTranslatef(xpos,ypos,zpos); // translate to appriate point
		gluSphere(sobj,.25,10,10);  // draw sphere
    }; glPopMatrix(); 
    
}

//render scene function.  draws everything to scene.
void renderScene(void)  {
    //update the modelview matrix based on the camera's position
    glMatrixMode(GL_MODELVIEW);             //make sure we aren't changing the projection matrix!
    glLoadIdentity();

    // set camera position
	myCam.update_pos();
	
    //clear the render buffer and depth buffer
	glDrawBuffer( GL_BACK );
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix(); {
        drawAxes();				// draw our axes so we know how we are oriented
    }; glPopMatrix();
    
    glPushMatrix(); {
        drawControlPoints();				// draw our control points for the bezier curve as spheres 
    	drawControlCage();					// draws cage for control points
    	// draws a bezier curve for each set of four control points.  assuming there will always be N=1+3m control points.
    	for(int j=0; j<numPoints-3; j+=3)
    	{
    		drawBezierCurve(points[j],points[j+1],points[j+2],points[j+3],0.01);
    	}
    }; glPopMatrix();
    
    glPushMatrix(); {
      animateBezier();     // animates curve along bezier
    }; glPopMatrix();
	
    //push the back buffer to the screen
    glutSwapBuffers();
}

void normalKeys(unsigned char key, int x, int y)  
{
    //kindly quit if the user requests!
        if (key == 27)
			exit(0);
}

// controls camera with mouse movement
void cameraControl( int x, int y)
{
   if ( leftClick==GLUT_DOWN)
   {
    // update camera angles
    myCam.set_theta(myCam.get_theta() + (x-oldx)*.01);
    myCam.set_phi(myCam.get_phi() + (y-oldy)*.01);
    // restrict phi
     if(myCam.get_phi() <= 0)
		myCam.set_phi(0+0.001);
	 if(myCam.get_phi() >= pi)
		myCam.set_phi(pi-0.001);
      
    glutPostRedisplay();
  }
  
  else if (rightClick==GLUT_DOWN)
  {
     myCam.set_radius(myCam.get_radius() + (y-oldy) * 0.03);  // change the radius based on movement
     // don't allow the camera radius to go below a set minimum or above a set maximum
     if(myCam.get_radius() < 10.0) 
        myCam.set_radius (10.0);
     if(myCam.get_radius() > 40.0) 
        myCam.set_radius (40.0);
  	glutPostRedisplay();
  } 
  // update old x and y positions
  oldx=x;
  oldy=y;
}

// controls the state of mouse based on usere clicking.
void clickControl(int button, int state, int x, int y)
{
    if(button == GLUT_LEFT_BUTTON)
        leftClick= state;  // save info if left click
    else if(button == GLUT_RIGHT_BUTTON)
        rightClick = state; // save info if right click 
    
    // update old x and y position
    oldx = x;
    oldy = y;
}

// function to handle menu attached to middle button
void myMenu(int value)
{
	if (value==1)
	{
		exit(0);
	}
}

//********************************************************************************
// void myTimer(int value)
//
//  We have to take an integer as input per GLUT's timer function prototype;
//  but we don't use that value so just ignore it. We'll register this function
//  once at the start of the program, and then every time it gets called it
//  will perpetually re-register itself and tell GLUT that the screen needs
//  be redrawn. yes, I said "needs be."
//********************************************************************************
void myTimer(int value) {
    
    // handle animation of sphere along curve
    if(step<1&&pcount<numPoints-3)
      {
      	step+=.004;
      }
    else if(step>1&&pcount<numPoints-3)
      {
    	step=0;
    	pcount+=3;
      }
      
    if (pcount>numPoints-3)
      {
        pcount=0;
        step=0.0;
	  }  // if sphere reaches the end of the curve, send it back to the beginning.
	  
	// tell GLUT to update the display
    glutPostRedisplay();
	// and register our timer again
    glutTimerFunc((unsigned int)(1000.0 / 60.0), myTimer, 0);
}

//********************************************************************************
//  void resizeWindow(int w, int h)
//
//      We will register this function as GLUT's reshape callback.
//   When the window is resized, the OS will tell GLUT and GLUT will tell
//   us by calling this function - GLUT will give us the new window width
//   and height as 'w' and 'h,' respectively - so save those to our global vars.
//********************************************************************************
void resizeWindow(int w, int h) {
    float aspectRatio = w / (float)h;
	
    windowWidth = w;
    windowHeight = h;

    //update the viewport to fill the window
    glViewport( 0, 0, w, h );

    //update the projection matrix with the new window properties
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity();

    /* TODO: set up the proper projection */
	gluPerspective( 45.0f, aspectRatio, 0.1, 1000 );
}

//********************************************************************************
//	registerCallbacks()
//
//	Register all of our callbacks with GLUT.  Just a way to clean up our 
//		intialization process by breaking each section into its own function.
//********************************************************************************
void registerCallbacks() {
	// ASCII keydown callback
    glutKeyboardFunc( normalKeys );
	// display to screen callback
    glutDisplayFunc( renderScene );
	// window reshape callback
    glutReshapeFunc( resizeWindow );
	// register our timer with GLUT
	glutTimerFunc( (unsigned int)(1000.0 / 60.0), myTimer, 0 );
    // clicking control that helps aid the arcball
	glutMouseFunc(clickControl);
	// control for the arcball camera
	glutMotionFunc( cameraControl);
}

//********************************************************************************
//  void initScene()
//
//  A basic scene initialization function; should be called once after the
//      OpenGL context has been created. Doesn't need to be called further.
//      Just sets up a few function calls so that our main() is cleaner.
//********************************************************************************
void initScene() {
	// tell OpenGL to perform depth testing with the Z-Buffer to perform hidden
	//		surface removal.  We will discuss this more very soon.
    glEnable( GL_DEPTH_TEST );
	
	//******************************************************************
    // this is some code to enable a default light for the scene;
    // feel free to play around with this, but we won't talk about
    // lighting in OpenGL for another couple of weeks yet.
    float lightCol[4] = { 1, 1, 1, 1};
    float ambientCol[4] = { 0.0, 0.0, 0.0, 1.0 };
    float lPosition[4] = { 10, 10, 10, 1 };
    glLightfv( GL_LIGHT0, GL_POSITION,lPosition );
    glLightfv( GL_LIGHT0, GL_DIFFUSE,lightCol );
    glLightfv( GL_LIGHT0, GL_AMBIENT, ambientCol );
    glEnable( GL_LIGHTING );
    glEnable( GL_LIGHT0 );
    
    // setup menu to be used for middle clicking
    int id = glutCreateMenu(myMenu);
    glutAddMenuEntry("Quit", 1);
    glutAttachMenu(GLUT_MIDDLE_BUTTON);
   	
    // tell OpenGL not to use the material system; just use whatever we 
	// pass with glColor*()
    glEnable( GL_COLOR_MATERIAL );
    glColorMaterial( GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE );
	//******************************************************************
	
    // tells OpenGL to blend colors across triangles. Once lighting is
    // enabled, this means that objects will appear smoother - if your object
    // is rounded or has a smooth surface, this is probably a good idea;
    // if your object has a blocky surface, you probably want to disable this.
    glShadeModel( GL_SMOOTH );
}

int main(int argc, char* argv[]) {
    // create a double-buffered GLUT window with depth buffer at (50,50) with 
	// predefined windowsize
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB );
    glutInitWindowPosition( 50, 50 );
    glutInitWindowSize( windowWidth, windowHeight );
    glutCreateWindow( "Bezier Curve" );
    
    // handle file
	FILE *dataFile;		// used to open data file
    string hold;    // buffer that holds data read in from file
    char c;         // character buffer
    
    vector<float> rawpoints;
    
    if (!(dataFile = fopen (argv[1], "r")))  // open file
	{
	  cout << "File read failed." << endl;	// if file can't be read, give appropriate error message.
	}
     else
	{
  	  while (!feof (dataFile))  // for the whole file
  	  {
  	    c=fgetc(dataFile);  // get next character
  	    if ( c!= ','  && c!='\n')
  	    {
  	    	hold.push_back(c);
  	    }  // put all characters of a particular float into a buffer.
  	    else 
  	    {
  	    	rawpoints.push_back(atof(hold.c_str()));  // put floating number in vector
  	    	hold.clear(); // clear buffer
  	    }
  	  }
  	      fclose (dataFile);  // close file
	}
	
	// store points in vector of control points
	numPoints=rawpoints[0];
	for(int i=1; i<=numPoints*3; i+=3)
	{
		controlpts temp(rawpoints[i],rawpoints[i+1],rawpoints[i+2]);
		points.push_back(temp);
	}
	
	// register all of our callbacks with GLUT
	registerCallbacks();
	
    // do some basic OpenGL setup
    initScene();

    // and enter the GLUT loop, never to exit.
    glutMainLoop();

    return(0);
}
