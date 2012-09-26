// Homework Assignment 3
// main.cpp
// Written by John Quinn
// As specified in the assignment guidelines, the basic 3d setup program was used as a basis.


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
#include <stdio.h>
#include <stdlib.h>

//********************************************************************************
// GLOBAL VARIABLES 

// global variables to keep track of window width and height.
// set to initial values for convenience, but we need variables
// for later on in case the window gets resized.
int windowWidth = 512, windowHeight = 512;

// declare pi as a floating point
float pi = 3.14159;

GLint leftClick, rightClick;    // holds state of left and right mouse buttons.
float oldx=0, oldy=0;    // holds previous x and y position of the mouse
float cameraRadius = 5.0, cameraTheta = 2.00, cameraPhi = 1.80; //camera position in spherical coordinates
float camX,camY,camZ;                     // holds x,y,z camera position.
float offX=-1, offY=0.5, offZ=-.5;                     // offset object position in world coordinates
float objX=0,objY=0,objZ=0;                  // object position
float objTheta=0;                        //  angle of rotation of object
float wheelTheta=0;                      // handles rotation of wheels when object is moving.
GLUquadric* cobj;  // body cylinder


// END GLOBAL VARIABLES 
//********************************************************************************

//********************************************************************************
//  void drawAxes()
//      
//      Draw a set of unit vectors at the origin, showing the user
//      the positive and negative X, Y, and Z axes for perspective.
//********************************************************************************
void drawAxes() {

    glDisable(GL_LIGHTING);

	// draw our axes
    glBegin(GL_LINES); {
		// set color white.
        glColor3f(1,1,1);
        
        // set up the x axes;
        for (int i=-50; i<=50; i++)
         {
           glVertex3f(-50,0,i);
           glVertex3f(50,0,i);
         }
        
        // set up z axes
        for (int i=-50; i<=50; i++)
         {
           glVertex3f(i,0,-50);
           glVertex3f(i,0,50);
         }
        
    }; glEnd();

    glEnable(GL_LIGHTING);
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

void drawBody()
{
		// set color
       glColor3f(.1,.1,1);      
       // draws a cylinder body
        gluCylinder(cobj, 1, 1, 7, 10, 16);
}

void drawEnd()
{
   
   // draws a conical end.
   glutSolidCone(1,1,10,10);

}

void drawWheels()
{
// change color to draw wheels

glColor3f(.1,1,.1);

// draw front wheels
  glPushMatrix();
  {
    glTranslatef(0,.5,-2);
    glRotatef(wheelTheta,0,0,1);
	glutSolidTorus	(0.25, 0.5 ,6 , 10 );
  }; glPopMatrix();
  
  glPushMatrix();
  {
    glTranslatef(-5,.5,-2);
    glRotatef(wheelTheta,0,0,1);
	glutSolidTorus	(0.25, 0.5 ,6, 10 );
  }; glPopMatrix();
  
//draw back wheels

      glPushMatrix();
  {
    glTranslatef(0,.5,0);
    glRotatef(wheelTheta,0,0,1);
	glutSolidTorus	(0.25, 0.5 ,6 , 10 );
  }; glPopMatrix();
  
// draws train smokestack using a tetrahedron
   glPushMatrix();
  {
    glTranslatef(-5,.5,0);
    glRotatef(wheelTheta,0,0,1);
	glutSolidTorus	(0.25, 0.5 ,6 , 10 );
  }; glPopMatrix();


}

// draws the whole train kart
void drawKart()
{

// draws our kart vehicle

// draw the body
  glPushMatrix();{
    glTranslatef(-6,1,-1);
    glRotatef(90,0,1,0);
	drawBody();
  }; glPopMatrix();

// draw all the wheels
  glPushMatrix();{
	drawWheels();
  }; glPopMatrix();

// draw front
  glPushMatrix();{
     glColor3f(1,.1,.1);
    glTranslatef(1,1,-1);
    glRotatef(90,0,1,0);
	drawEnd();
  }; glPopMatrix();

// draws back
  glPushMatrix();{
       glColor3f(.1,.1,1);
    glTranslatef(-6,1,-1);
    glRotatef(270,0,1,0);
	drawEnd();
  }; glPopMatrix();
  
// draw top piece - tetrahedron
  glPushMatrix();{
    glColor3f(1,1,1);
    glTranslatef(0,2.5,-1);
    glRotatef(45,0,1,0);
    glutSolidTetrahedron();
  }; glPopMatrix();

}

// draws several dodecahedrons for scenery.
void drawScenery()
{
   for (int i = -5; i<=5; i++)
     {
      for (int j=-5; j<=5; j++)
      {
       if (j!=0 || i!=0)
       {
       glPushMatrix();{
         glColor3f(.5,.5,.5);
         glTranslatef(j*10,2,i*10);
         glutSolidDodecahedron();
  	   } glPopMatrix();
  	   }  // draw a dodecahedron every 10 units, skipping the starting point of the train
      }
    }
}

//********************************************************************************
//  void renderScene()
//
//  GLUT callback for scene rendering. Sets up the modelview matrix, renders
//      a dodecahedron to the back buffer, and switches the back buffer with the
//      front buffer (what the user sees).
//********************************************************************************
void renderScene(void)  {
    //update the modelview matrix based on the camera's position
    glMatrixMode(GL_MODELVIEW);             //make sure we aren't changing the projection matrix!
    glLoadIdentity();

    /* TODO: Set up the Camera */
	gluLookAt( camX+objX, camY, camZ+objZ, 
			   objX+offX, objY+offY, objZ+offZ,
			   0, 1, 0 );
	
    //clear the render buffer and depth buffer
	glDrawBuffer( GL_BACK );
    glClearColor(0,0,0,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glPushMatrix(); {
        drawAxes();				// draw our axes so we know how we are oriented
    }; glPopMatrix();
	
	glPushMatrix();{
	    glTranslatef(objX,objY,objZ);
	    glScalef(.5,.5,.5);
	    glRotatef((float)objTheta *(180/pi),0,1,0);  /// rotate base on input
        drawKart();
     }; glPopMatrix();  // draw the kart

    glPushMatrix();{
       drawScenery();  // draw the scenery
    };glPopMatrix();
    //push the back buffer to the screen
    glutSwapBuffers();
}

//********************************************************************************
//  void normalKeys(unsigned char key, int x, int y)
//
//      GLUT keyboard callback for regular characters when pressed down.
//********************************************************************************
void normalKeys(unsigned char key, int x, int y)  {
    //kindly quit if the user requests!
        if (key == 27)
			exit(0);
		else if (key == 'w')
		{
		   objX += cos(objTheta) * .5;
		   objZ -= sin(objTheta) * .5;
		   wheelTheta -=4;
		}  // move forward
		else if (key == 's')
		{
		   objX -= cos(objTheta) * .5;
		   objZ += sin(objTheta) * .5;
		   wheelTheta +=2;
		} // move backward
		else if (key == 'd')
		{
		   objTheta-=.04;
		}  // rotate right
		else if (key == 'a')
		{
		   objTheta+=.04;
		}  // rotate left
		
		// handle z out of bounds
		if (objZ<=-50)
		  objZ=-49.99;
		else if (objZ >=50)
		  objZ=49.99;
		  
		// handle x out of bounds
		if (objX <=-50)
		  objX=-49.99;
		else if (objX>=50)
		  objX=49.99;
		  
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
	/* TODO: and make all that stuff move */
	
	// tell GLUT to update the display
    glutPostRedisplay();
	// and register our timer again
    glutTimerFunc((unsigned int)(1000.0 / 60.0), myTimer, 0);
}

void cameraControl( int x, int y)
{
   if ( leftClick==GLUT_DOWN)
   {
   // update camera angles
    cameraTheta+=(x-oldx)*.01;
    cameraPhi+=(y-oldy)*.01;
    //update camera coordinates using spherical coordinates      
    camX = cameraRadius * sin(cameraTheta)*sin(cameraPhi);
    camY = cameraRadius * -cos(cameraPhi);
    camZ = cameraRadius * -cos(cameraTheta)*sin(cameraPhi);
    glutPostRedisplay();
  }
  
  else if (rightClick==GLUT_DOWN)
  {
     cameraRadius += (y-oldy) * 0.03;  // change the radius based on movement
     // don't allow the camera radius to go below a set minimum or above a set maximum
     if(cameraRadius < 3.0) 
        cameraRadius = 3.0;
     if(cameraRadius > 12.0) 
        cameraRadius = 12.0;
    //update camera coordinates using spherical coordinates      
    camX = cameraRadius * sin(cameraTheta)*sin(cameraPhi);
    camY = cameraRadius * -cos(cameraPhi);
    camZ = cameraRadius * -cos(cameraTheta)*sin(cameraPhi);
  	glutPostRedisplay();
  } 
  // update old x and y positions
  oldx=x;
  oldy=y;
}

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

//********************************************************************************
//  int main(int argc, char **argv)
//
//      No offense but you should probably know what the main function does.
//      argc is the number of arguments to the program (program name included),
//      and argv is an array of strings; argv[i] is the ith command line
//      argument, where 0 <= i <= argc
//********************************************************************************
int main(int argc, char* argv[]) {
    // create a double-buffered GLUT window with depth buffer at (50,50) with 
	// predefined windowsize
    glutInit( &argc, argv );
    glutInitDisplayMode( GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGB );
    glutInitWindowPosition( 50, 50 );
    glutInitWindowSize( windowWidth, windowHeight );
    glutCreateWindow( "CHOO CHOO!!!!" );
    
    // set up cylinder object
    cobj = gluNewQuadric();
    gluQuadricNormals(cobj, GLU_SMOOTH);
     
    // set up initial camera position
    camX = cameraRadius * sin(cameraTheta)*sin(cameraPhi);
    camY = cameraRadius * -cos(cameraPhi);
    camZ = cameraRadius * -cos(cameraTheta)*sin(cameraPhi);


	// register all of our callbacks with GLUT
	registerCallbacks();
	
    // do some basic OpenGL setup
    initScene();

    // and enter the GLUT loop, never to exit.
    glutMainLoop();

    return(0);
}
