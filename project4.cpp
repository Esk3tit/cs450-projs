#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <vector>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#endif

#include "glew.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include "glut.h"

//extern void Unit( float*, float* );

#ifndef POINT_H
#define POINT_H
struct point
{
	float x, y, z;		// coordinates
	float nx, ny, nz;	// surface normal
	float s, t;			// texture coords
};

inline
void
DrawPoint( struct point* p )
{
	glNormal3fv( &p->nx );
	glTexCoord2fv( &p->s );
	glVertex3fv( &p->x );
}
#endif

// milliseconds per cycle:
#define MS_PER_CYCLE  10000

#define ORBIT_RAD 7.f

#define AMPLITUDE 6.f
#define NUM_BACK_AND_FORTH_PER_CYCLE 3.f
#define X0 10.f

// delimiters for parsing the obj file:

#define OBJDELIMS		" \t"


//	This is a sample OpenGL / GLUT program
//
//	The objective is to draw a 3d object and change the color of the axes
//		with a glut menu
//
//	The left mouse button does rotation
//	The middle mouse button does scaling
//	The user interface allows:
//		1. The axes to be turned on and off
//		2. The color of the axes to be changed
//		3. Debugging to be turned on and off
//		4. Depth cueing to be turned on and off
//		5. The projection to be changed
//		6. The transformations to be reset
//		7. The program to quit
//
//	Author:			Joe Graphics

// title of these windows:

const char *WINDOWTITLE = "Project #4 -- Khai Phan";
const char *GLUITITLE   = "User Interface Window";

// what the glui package defines as true and false:

const int GLUITRUE  = true;
const int GLUIFALSE = false;

// the escape key:

const int ESCAPE = 0x1b;

// initial window size:

const int INIT_WINDOW_SIZE = 600;

// size of the 3d box to be drawn:

const float BOXSIZE = 2.f;

// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = 1.f;
const float SCLFACT = 0.005f;

// minimum allowable scale factor:

const float MINSCALE = 0.05f;

// scroll wheel button values:

const int SCROLL_WHEEL_UP   = 3;
const int SCROLL_WHEEL_DOWN = 4;

// equivalent mouse movement when we click the scroll wheel:

const float SCROLL_WHEEL_CLICK_FACTOR = 5.f;

// active mouse buttons (or them together):

const int LEFT   = 4;
const int MIDDLE = 2;
const int RIGHT  = 1;

// which projection:

enum Projections
{
	ORTHO,
	PERSP
};

// which button:

enum ButtonVals
{
	RESET,
	QUIT
};

// window background color (rgba):

const GLfloat BACKCOLOR[ ] = { 0., 0., 0., 1. };

// line width for the axes:

const GLfloat AXES_WIDTH   = 3.;

// the color numbers:
// this order must match the radio button order, which must match the order of the color names,
// 	which must match the order of the color RGB values

enum Colors
{
	RED,
	YELLOW,
	GREEN,
	CYAN,
	BLUE,
	MAGENTA,
	WHITE,
	BLACK
};

char * ColorNames[ ] =
{
	(char *)"Red",
	(char*)"Yellow",
	(char*)"Green",
	(char*)"Cyan",
	(char*)"Blue",
	(char*)"Magenta",
	(char*)"White",
	(char*)"Black"
};

enum Lights
{
	LIGHT0,
	LIGHT1,
	LIGHT2
};

// the color definitions:
// this order must match the menu order

const GLfloat Colors[ ][3] = 
{
	{ 1., 0., 0. },		// red
	{ 1., 1., 0. },		// yellow
	{ 0., 1., 0. },		// green
	{ 0., 1., 1. },		// cyan
	{ 0., 0., 1. },		// blue
	{ 1., 0., 1. },		// magenta
	{ 1., 1., 1. },		// white
	{ 0., 0., 0. },		// black
};

// fog parameters:

const GLfloat FOGCOLOR[4] = { .0f, .0f, .0f, 1.f };
const GLenum  FOGMODE     = GL_LINEAR;
const GLfloat FOGDENSITY  = 0.30f;
const GLfloat FOGSTART    = 1.5f;
const GLfloat FOGEND      = 4.f;

struct Vertex
{
	float x, y, z;
};


struct Normal
{
	float nx, ny, nz;
};


struct TextureCoord
{
	float s, t, p;
};


struct face
{
	int v, n, t;
};


// what options should we compile-in?
// in general, you don't need to worry about these
// i compile these in to show class examples of things going wrong

//#define DEMO_Z_FIGHTING
//#define DEMO_DEPTH_BUFFER


// non-constant global variables:

int		ActiveButton;			// current button that is down
GLuint	AxesList;				// list to hold the axes
int		AxesOn;					// != 0 means to draw the axes
int		DebugOn;				// != 0 means to print debugging info
int		DepthCueOn;				// != 0 means to use intensity depth cueing
int		DepthBufferOn;			// != 0 means to use the z-buffer
int		DepthFightingOn;		// != 0 means to force the creation of z-fighting
GLuint	EarthList;				// list to hold the Earth solid sphere
GLuint	BoxList;				// object display list
GLuint	GamerList;				// list for bing chilling big gamer
GLuint	SunList;				// list for sun light source (orange)
GLuint	HaloList;				// list for torus halo light source (yellow)
int		MainWindow;				// window id for main graphics window
float	Scale;					// scaling factor
int		ShadowsOn;				// != 0 means to turn shadows on
GLuint	StarList;				// list to hold star orbiting Earth
float	Time;					// fraction (0.-1.) of the time cycle
int		WhichColor;				// index into Colors[ ]
int		WhichProjection;		// ORTHO or PERSP
int		Xmouse, Ymouse;			// mouse values
float	Xrot, Yrot;				// rotation angles in degrees

int		ConeNumLngs, ConeNumLats;
struct  point* ConePts;

int		SphNumLngs, SphNumLats;
struct  point* SphPts;

bool	Frozen;

bool	Light0On, Light1On, Light2On;

unsigned char* Texture;		// the texels
unsigned int   WorldTex;	// the texture object

float White[ 3 ] = { 1., 1., 1. };

// function prototypes:

void	Animate( );
void	Display( );
void	DoAxesMenu( int );
void	DoColorMenu( int );
void	DoDepthBufferMenu( int );
void	DoDepthFightingMenu( int );
void	DoDepthMenu( int );
void	DoDebugMenu( int );
void	DoMainMenu( int );
void	DoProjectMenu( int );
void	DoLightMenu( int );
void	DoRasterString( float, float, float, char * );
void	DoStrokeString( float, float, float, float, char * );
float	ElapsedSeconds( );
void	InitGraphics( );
void	InitLists( );
void	InitMenus( );
void	Keyboard( unsigned char, int, int );
void	MouseButton( int, int, int, int );
void	MouseMotion( int, int );
void	OsuCone( float, float, float, int, int );
void	OsuSphere( float, int, int );
void	Reset( );
void	Resize( int, int );
void	Visibility( int );

void	SetMaterial( float, float, float, float );
void	SetPointLight( int, float, float, float, float, float, float, float, float, float );
void	SetSpotLight( int, float, float, float, float, float, float, float, float, float );

float * Array3( float, float, float );
float * Array4( float, float, float, float );
float * BlendArray3( float, float[ 3 ], float[ 3 ] );
float * MulArray3( float, float[ 3 ] );

void			Axes( float );

unsigned char *	BmpToTexture( char *, int *, int * );
int				ReadInt( FILE * );
short			ReadShort( FILE * );

void			HsvRgb( float[3], float [3] );

void			Cross(float[3], float[3], float[3]);
float			Dot(float [3], float [3]);
float			Unit(float [3], float [3]);
int				LoadObjFile( char* );
char *			ReadRestOfLine( FILE* );
void			ReadObjVTN( char*, int*, int*, int* );
float			Unit( float[ 3 ] );

inline
struct point*
ConePtsPointer( int lat, int lng )
{
	if ( lat < 0 )			lat += ( ConeNumLats - 1 );
	if ( lng < 0 )			lng += ( ConeNumLngs - 0 );
	if ( lat > ConeNumLats - 1 )	lat -= ( ConeNumLats - 1 );
	if ( lng > ConeNumLngs - 1 )	lng -= ( ConeNumLngs - 0 );
	return &ConePts[ ConeNumLngs * lat + lng ];
}

inline
struct point*
SphPtsPointer( int lat, int lng )
{
	if ( lat < 0 )			lat += ( SphNumLats - 1 );
	if ( lng < 0 )			lng += ( SphNumLngs - 0 );
	if ( lat > SphNumLats - 1 )	lat -= ( SphNumLats - 1 );
	if ( lng > SphNumLngs - 1 )	lng -= ( SphNumLngs - 0 );
	return &SphPts[ SphNumLngs * lat + lng ];
}


// main program:

int
main( int argc, char *argv[ ] )
{
	// turn on the glut package:
	// (do this before checking argc and argv since it might
	// pull some command line arguments out)

	glutInit( &argc, argv );

	// setup all the graphics stuff:

	InitGraphics( );

	// create the display structures that will not change:

	InitLists( );

	// init all the global variables used by Display( ):
	// this will also post a redisplay

	Reset( );

	// setup all the user interface stuff:

	InitMenus( );

	// draw the scene once and wait for some interaction:
	// (this will never return)

	glutSetWindow( MainWindow );
	glutMainLoop( );

	// glutMainLoop( ) never actually returns
	// the following line is here to make the compiler happy:

	return 0;
}


// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display( ) from here -- let glutPostRedisplay( ) do it

void
Animate( )
{
	// put animation stuff in here -- change some global variables
	// for Display( ) to find:

	int ms = glutGet( GLUT_ELAPSED_TIME );
	ms %= MS_PER_CYCLE;
	Time = ( float )ms / ( float )MS_PER_CYCLE;		// [0.,1.)

	// force a call to Display( ) next time it is convenient:

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// draw the complete scene:

void
Display( )
{
	// set which window we want to do the graphics into:

	glutSetWindow( MainWindow );


	// erase the background:

	glDrawBuffer( GL_BACK );
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	glEnable( GL_DEPTH_TEST );
#ifdef DEMO_DEPTH_BUFFER
	if( DepthBufferOn == 0 )
		glDisable( GL_DEPTH_TEST );
#endif


	// specify shading to be flat:

	glShadeModel( GL_FLAT );


	// set the viewport to a square centered in the window:

	GLsizei vx = glutGet( GLUT_WINDOW_WIDTH );
	GLsizei vy = glutGet( GLUT_WINDOW_HEIGHT );
	GLsizei v = vx < vy ? vx : vy;			// minimum dimension
	GLint xl = ( vx - v ) / 2;
	GLint yb = ( vy - v ) / 2;
	glViewport( xl, yb,  v, v );


	// set the viewing volume:
	// remember that the Z clipping  values are actually
	// given as DISTANCES IN FRONT OF THE EYE
	// USE gluOrtho2D( ) IF YOU ARE DOING 2D !

	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	if( WhichProjection == ORTHO )
		glOrtho( -2.f, 2.f,     -2.f, 2.f,     0.1f, 1000.f );
	else
		gluPerspective( 70.f, 1.f,	0.1f, 1000.f );


	// place the objects into the scene:

	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );


	// set the eye position, look-at position, and up-vector:

	gluLookAt( 0.f, 0.f, 3.f,     0.f, 0.f, 0.f,     0.f, 1.f, 0.f );


	// rotate the scene:

	glRotatef( (GLfloat)Yrot, 0.f, 1.f, 0.f );
	glRotatef( (GLfloat)Xrot, 1.f, 0.f, 0.f );


	// uniformly scale the scene:

	if( Scale < MINSCALE )
		Scale = MINSCALE;
	glScalef( (GLfloat)Scale, (GLfloat)Scale, (GLfloat)Scale );


	// set the fog parameters:

	if( DepthCueOn != 0 )
	{
		glFogi( GL_FOG_MODE, FOGMODE );
		glFogfv( GL_FOG_COLOR, FOGCOLOR );
		glFogf( GL_FOG_DENSITY, FOGDENSITY );
		glFogf( GL_FOG_START, FOGSTART );
		glFogf( GL_FOG_END, FOGEND );
		glEnable( GL_FOG );
	}
	else
	{
		glDisable( GL_FOG );
	}


	// possibly draw the axes:

	if( AxesOn != 0 )
	{
		glColor3fv( &Colors[WhichColor][0] );
		glCallList( AxesList );
	}


	// since we are using glScalef( ), be sure the normals get unitized:

	glEnable( GL_NORMALIZE );

	// Global ambient lighting
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, MulArray3( .3f, White ) );
	
	// Enable two sided lighting
	glLightModeli( GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE );

	// OpenGL lighting enabled (different from enabling individual lights)
	glEnable( GL_LIGHTING );

	if ( Light0On )
		glEnable( GL_LIGHT0 );
	else
		glDisable( GL_LIGHT0 );

	if ( Light1On )
		glEnable( GL_LIGHT1 );
	else
		glDisable( GL_LIGHT1 );

	if ( Light2On )
		glEnable( GL_LIGHT2 );
	else
		glDisable( GL_LIGHT2 );

	// SCENE OBJECTS
	glEnable( GL_TEXTURE_2D );
	glBindTexture( GL_TEXTURE_2D, WorldTex );
	glTexEnvf( GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE );

	glPushMatrix( );
		SetMaterial( 1., 1., 1., 3. );
		glShadeModel( GL_SMOOTH );
		glCallList( EarthList );
	glPopMatrix( );

	glDisable( GL_TEXTURE_2D );

	glPushMatrix( );
		SetMaterial( 0., 1., 1., 23. );
		glTranslatef( ( float )( X0 + AMPLITUDE * sin( 2. * M_PI * NUM_BACK_AND_FORTH_PER_CYCLE * Time ) ), 6., 0. );
		glShadeModel( GL_FLAT );
		glutSolidDodecahedron( );
	glPopMatrix( );

	glPushMatrix( );
		SetMaterial( 1., 1., 1., 0. );
		glTranslatef( 0., -10., -25. );
		glShadeModel( GL_SMOOTH );
		glCallList( GamerList );
	glPopMatrix( );

	float theta = ( float )( 2. * M_PI ) * Time;

	// LIGHT SOURCES

	// Star orbiting Earth
	glPushMatrix( );
		// Set the point light
		SetPointLight( GL_LIGHT0, ( float )( ORBIT_RAD * cosf( theta ) ), 0., ( float )( ORBIT_RAD * sinf( theta ) ), 1., 1., 1., 0., 0., 1. );
		
		// Disable lighting here to re-enable color3f
		glDisable( GL_LIGHTING );
		glColor3f( 1., 1., 1. );

		glTranslatef( ( float )( ORBIT_RAD * cosf( theta ) ), 0., ( float )( ORBIT_RAD * sinf( theta ) ) );
		glCallList( StarList );

		glEnable( GL_LIGHTING );
	glPopMatrix( );

	// Sun
	glPushMatrix( );

		SetPointLight( GL_LIGHT1, 15., 15., -50., 1., 0.5, 0., 0., 1., 0. );

		// Disable lighting here to re-enable color3f
		glDisable( GL_LIGHTING );
		glColor3f( 1., 0.5, 0. );

		glTranslatef( 15., 15., -50. );
		glCallList( SunList );

		glEnable( GL_LIGHTING );
	glPopMatrix( );

	glPushMatrix( );

		SetSpotLight( GL_LIGHT2, 0., 13., -25., 0., -1., 0., 1., 1, 0. );

		// Disable lighting here to re-enable color3f
		glDisable( GL_LIGHTING );
		glColor3f( 1., 1., 0. );

		glPushMatrix( );
			glTranslatef( 0., 13., -25. );
			glutSolidSphere( 0.2, 10., 10. );
		glPopMatrix( );

		glTranslatef( 0., 13., -25. );
		glRotatef( 90., 1., 0., 0. );
		glScalef( 0.25, 0.25, 0.1 );
		glCallList( HaloList );

		glEnable( GL_LIGHTING );
	glPopMatrix( );

#ifdef DEMO_Z_FIGHTING
	if( DepthFightingOn != 0 )
	{
		glPushMatrix( );
			glRotatef( 90.f,   0.f, 1.f, 0.f );
			glCallList( BoxList );
		glPopMatrix( );
	}
#endif


	// draw some gratuitous text that just rotates on top of the scene:
	// i commented out the actual text-drawing calls -- put them back in if you have a use for them
	// a good use for thefirst one might be to have your name on the screen
	// a good use for the second one might be to have vertex numbers on the screen alongside each vertex

	glDisable( GL_DEPTH_TEST );
	glColor3f( 0.f, 1.f, 1.f );
	//DoRasterString( 0.f, 1.f, 0.f, (char *)"Text That Moves" );


	// draw some gratuitous text that is fixed on the screen:
	//
	// the projection matrix is reset to define a scene whose
	// world coordinate system goes from 0-100 in each axis
	//
	// this is called "percent units", and is just a convenience
	//
	// the modelview matrix is reset to identity as we don't
	// want to transform these coordinates

	glDisable( GL_DEPTH_TEST );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity( );
	gluOrtho2D( 0.f, 100.f,     0.f, 100.f );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity( );
	glColor3f( 1.f, 1.f, 1.f );
	//DoRasterString( 5.f, 5.f, 0.f, (char *)"Text That Doesn't" );

	// swap the double-buffered framebuffers:

	glutSwapBuffers( );

	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush( ) here, not glFinish( ) !

	glFlush( );
}


void
DoAxesMenu( int id )
{
	AxesOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoColorMenu( int id )
{
	WhichColor = id - RED;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDebugMenu( int id )
{
	DebugOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthBufferMenu( int id )
{
	DepthBufferOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthFightingMenu( int id )
{
	DepthFightingOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoDepthMenu( int id )
{
	DepthCueOn = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// main menu callback:

void
DoMainMenu( int id )
{
	switch( id )
	{
		case RESET:
			Reset( );
			break;

		case QUIT:
			// gracefully close out the graphics:
			// gracefully close the graphics window:
			// gracefully exit the program:
			glutSetWindow( MainWindow );
			glFinish( );
			glutDestroyWindow( MainWindow );
			exit( 0 );
			break;

		default:
			fprintf( stderr, "Don't know what to do with Main Menu ID %d\n", id );
	}

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


void
DoProjectMenu( int id )
{
	WhichProjection = id;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}

void
DoLightMenu( int id )
{
	switch ( id )
	{
		case LIGHT0:
			Light0On = !Light0On;
			break;

		case LIGHT1:
			Light1On = !Light1On;
			break;

		case LIGHT2:
			Light2On = !Light2On;
			break;

		default:
			break;
	}

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// use glut to display a string of characters using a raster font:

void
DoRasterString( float x, float y, float z, char *s )
{
	glRasterPos3f( (GLfloat)x, (GLfloat)y, (GLfloat)z );

	char c;			// one character to print
	for( ; ( c = *s ) != '\0'; s++ )
	{
		glutBitmapCharacter( GLUT_BITMAP_TIMES_ROMAN_24, c );
	}
}


// use glut to display a string of characters using a stroke font:

void
DoStrokeString( float x, float y, float z, float ht, char *s )
{
	glPushMatrix( );
		glTranslatef( (GLfloat)x, (GLfloat)y, (GLfloat)z );
		float sf = ht / ( 119.05f + 33.33f );
		glScalef( (GLfloat)sf, (GLfloat)sf, (GLfloat)sf );
		char c;			// one character to print
		for( ; ( c = *s ) != '\0'; s++ )
		{
			glutStrokeCharacter( GLUT_STROKE_ROMAN, c );
		}
	glPopMatrix( );
}


// return the number of seconds since the start of the program:

float
ElapsedSeconds( )
{
	// get # of milliseconds since the start of the program:

	int ms = glutGet( GLUT_ELAPSED_TIME );

	// convert it to seconds:

	return (float)ms / 1000.f;
}


// initialize the glui window:

void
InitMenus( )
{
	glutSetWindow( MainWindow );

	int numColors = sizeof( Colors ) / ( 3*sizeof(int) );
	int colormenu = glutCreateMenu( DoColorMenu );
	for( int i = 0; i < numColors; i++ )
	{
		glutAddMenuEntry( ColorNames[i], i );
	}

	int axesmenu = glutCreateMenu( DoAxesMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthcuemenu = glutCreateMenu( DoDepthMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthbuffermenu = glutCreateMenu( DoDepthBufferMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int depthfightingmenu = glutCreateMenu( DoDepthFightingMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int debugmenu = glutCreateMenu( DoDebugMenu );
	glutAddMenuEntry( "Off",  0 );
	glutAddMenuEntry( "On",   1 );

	int projmenu = glutCreateMenu( DoProjectMenu );
	glutAddMenuEntry( "Orthographic",  ORTHO );
	glutAddMenuEntry( "Perspective",   PERSP );

	int lightmenu = glutCreateMenu( DoLightMenu );
	glutAddMenuEntry( "Light 0", LIGHT0 );
	glutAddMenuEntry( "Light 1", LIGHT1 );
	glutAddMenuEntry( "Light 2", LIGHT2 );

	int mainmenu = glutCreateMenu( DoMainMenu );
	glutAddSubMenu(   "Axes",          axesmenu);
	glutAddSubMenu(   "Axis Colors",   colormenu);
	glutAddSubMenu(	  "Lights",		   lightmenu );

#ifdef DEMO_DEPTH_BUFFER
	glutAddSubMenu(   "Depth Buffer",  depthbuffermenu);
#endif

#ifdef DEMO_Z_FIGHTING
	glutAddSubMenu(   "Depth Fighting",depthfightingmenu);
#endif

	glutAddSubMenu(   "Depth Cue",     depthcuemenu);
	glutAddSubMenu(   "Projection",    projmenu );
	glutAddMenuEntry( "Reset",         RESET );
	glutAddSubMenu(   "Debug",         debugmenu);
	glutAddMenuEntry( "Quit",          QUIT );

// attach the pop-up menu to the right mouse button:

	glutAttachMenu( GLUT_RIGHT_BUTTON );
}



// initialize the glut and OpenGL libraries:
//	also setup callback functions

void
InitGraphics( )
{
	// request the display modes:
	// ask for red-green-blue-alpha color, double-buffering, and z-buffering:

	glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );

	// set the initial window configuration:

	glutInitWindowPosition( 0, 0 );
	glutInitWindowSize( INIT_WINDOW_SIZE, INIT_WINDOW_SIZE );

	// open the window and set its title:

	MainWindow = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );

	// set the framebuffer clear values:

	glClearColor( BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3] );

	// setup the callback functions:
	// DisplayFunc -- redraw the window
	// ReshapeFunc -- handle the user resizing the window
	// KeyboardFunc -- handle a keyboard input
	// MouseFunc -- handle the mouse button going down or up
	// MotionFunc -- handle the mouse moving with a button down
	// PassiveMotionFunc -- handle the mouse moving with a button up
	// VisibilityFunc -- handle a change in window visibility
	// EntryFunc	-- handle the cursor entering or leaving the window
	// SpecialFunc -- handle special keys on the keyboard
	// SpaceballMotionFunc -- handle spaceball translation
	// SpaceballRotateFunc -- handle spaceball rotation
	// SpaceballButtonFunc -- handle spaceball button hits
	// ButtonBoxFunc -- handle button box hits
	// DialsFunc -- handle dial rotations
	// TabletMotionFunc -- handle digitizing tablet motion
	// TabletButtonFunc -- handle digitizing tablet button hits
	// MenuStateFunc -- declare when a pop-up menu is in use
	// TimerFunc -- trigger something to happen a certain time from now
	// IdleFunc -- what to do when nothing else is going on

	glutSetWindow( MainWindow );
	glutDisplayFunc( Display );
	glutReshapeFunc( Resize );
	glutKeyboardFunc( Keyboard );
	glutMouseFunc( MouseButton );
	glutMotionFunc( MouseMotion );
	glutPassiveMotionFunc(MouseMotion);
	//glutPassiveMotionFunc( NULL );
	glutVisibilityFunc( Visibility );
	glutEntryFunc( NULL );
	glutSpecialFunc( NULL );
	glutSpaceballMotionFunc( NULL );
	glutSpaceballRotateFunc( NULL );
	glutSpaceballButtonFunc( NULL );
	glutButtonBoxFunc( NULL );
	glutDialsFunc( NULL );
	glutTabletMotionFunc( NULL );
	glutTabletButtonFunc( NULL );
	glutMenuStateFunc( NULL );
	glutTimerFunc( -1, NULL, 0 );

	// setup glut to call Animate( ) every time it has
	// 	nothing it needs to respond to (which is most of the time)
	// we don't need to do this for this program, and really should set the argument to NULL
	// but, this sets us up nicely for doing animation

	glutIdleFunc( Animate );

	// Init textures
	int width, height;
	Texture = BmpToTexture( ( char* )"worldtex.bmp", &width, &height );
	if ( Texture == NULL )
		fprintf( stderr, "Cannot open texture '%s'\n", "worldtex.bmp" );
	else
		fprintf( stderr, "Width = %d ; Height = %d\n", width, height );

	glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
	glGenTextures( 1, &WorldTex );
	glBindTexture( GL_TEXTURE_2D, WorldTex );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexImage2D( GL_TEXTURE_2D, 0, 3, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, Texture );

	// init the glew package (a window must be open to do this):

#ifdef WIN32
	GLenum err = glewInit( );
	if( err != GLEW_OK )
	{
		fprintf( stderr, "glewInit Error\n" );
	}
	else
		fprintf( stderr, "GLEW initialized OK\n" );
	fprintf( stderr, "Status: Using GLEW %s\n", glewGetString(GLEW_VERSION));
#endif

}


// initialize the display lists that will not change:
// (a display list is a way to store opengl commands in
//  memory so that they can be played back efficiently at a later time
//  with a call to glCallList( )

void
InitLists( )
{
	float dx = BOXSIZE / 2.f;
	float dy = BOXSIZE / 2.f;
	float dz = BOXSIZE / 2.f;
	glutSetWindow( MainWindow );

	// create the object:

	BoxList = glGenLists( 1 );
	glNewList( BoxList, GL_COMPILE );

		glBegin( GL_QUADS );

			glColor3f( 1., 0., 0. );

				glNormal3f( 1., 0., 0. );
					glVertex3f(  dx, -dy,  dz );
					glVertex3f(  dx, -dy, -dz );
					glVertex3f(  dx,  dy, -dz );
					glVertex3f(  dx,  dy,  dz );

				glNormal3f(-1., 0., 0.);
					glVertex3f( -dx, -dy,  dz);
					glVertex3f( -dx,  dy,  dz );
					glVertex3f( -dx,  dy, -dz );
					glVertex3f( -dx, -dy, -dz );

			glColor3f( 0., 1., 0. );

				glNormal3f(0., 1., 0.);
					glVertex3f( -dx,  dy,  dz );
					glVertex3f(  dx,  dy,  dz );
					glVertex3f(  dx,  dy, -dz );
					glVertex3f( -dx,  dy, -dz );

				glNormal3f(0., -1., 0.);
					glVertex3f( -dx, -dy,  dz);
					glVertex3f( -dx, -dy, -dz );
					glVertex3f(  dx, -dy, -dz );
					glVertex3f(  dx, -dy,  dz );

			glColor3f(0., 0., 1.);

				glNormal3f(0., 0., 1.);
					glVertex3f(-dx, -dy, dz);
					glVertex3f( dx, -dy, dz);
					glVertex3f( dx,  dy, dz);
					glVertex3f(-dx,  dy, dz);

				glNormal3f(0., 0., -1.);
					glVertex3f(-dx, -dy, -dz);
					glVertex3f(-dx,  dy, -dz);
					glVertex3f( dx,  dy, -dz);
					glVertex3f( dx, -dy, -dz);

		glEnd( );

	glEndList( );


	// create the axes:

	AxesList = glGenLists( 1 );
	glNewList( AxesList, GL_COMPILE );
		glLineWidth( AXES_WIDTH );
			Axes( 1.5 );
		glLineWidth( 1. );
	glEndList( );

	EarthList = glGenLists( 1 );
	glNewList( EarthList, GL_COMPILE );
		OsuSphere( 3., 50, 50 );
	glEndList( );

	StarList = glGenLists( 1 );
	glNewList( StarList, GL_COMPILE );
		glutSolidSphere( 1., 30, 30 );
	glEndList( );

	GamerList = glGenLists( 1 );
	glNewList( GamerList, GL_COMPILE );
		LoadObjFile( "FinalBaseMesh.obj" );
	glEndList( );

	SunList = glGenLists( 1 );
	glNewList( SunList, GL_COMPILE );
		glutSolidSphere( 5., 50, 50 );
	glEndList( );

	HaloList = glGenLists( 1 );
	glNewList( HaloList, GL_COMPILE );
		glutSolidTorus( 1., 6., 70, 70 );
	glEndList( );

}


// the keyboard callback:

void
Keyboard( unsigned char c, int x, int y )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Keyboard: '%c' (0x%0x)\n", c, c );

	switch( c )
	{
		case 'o':
		case 'O':
			WhichProjection = ORTHO;
			break;

		case 'p':
		case 'P':
			WhichProjection = PERSP;
			break;

		case 'q':
		case 'Q':
		case ESCAPE:
			DoMainMenu( QUIT );	// will not return here
			break;				// happy compiler

		case 'f':
		case 'F':
			Frozen = !Frozen;
			if ( Frozen )
				glutIdleFunc( NULL );
			else
				glutIdleFunc( Animate );
			break;

		case '0':
			Light0On = !Light0On;
			break;

		case '1':
			Light1On = !Light1On;
			break;

		case '2':
			Light2On = !Light2On;
			break;

		default:
			fprintf( stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c );
	}

	// force a call to Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// called when the mouse button transitions down or up:

void
MouseButton( int button, int state, int x, int y )
{
	int b = 0;			// LEFT, MIDDLE, or RIGHT

	if( DebugOn != 0 )
		fprintf( stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y );

	
	// get the proper button bit mask:

	switch( button )
	{
		case GLUT_LEFT_BUTTON:
			b = LEFT;		break;

		case GLUT_MIDDLE_BUTTON:
			b = MIDDLE;		break;

		case GLUT_RIGHT_BUTTON:
			b = RIGHT;		break;

		case SCROLL_WHEEL_UP:
			Scale += SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
			// keep object from turning inside-out or disappearing:
			if (Scale < MINSCALE)
				Scale = MINSCALE;
			break;

		case SCROLL_WHEEL_DOWN:
			Scale -= SCLFACT * SCROLL_WHEEL_CLICK_FACTOR;
			// keep object from turning inside-out or disappearing:
			if (Scale < MINSCALE)
				Scale = MINSCALE;
			break;

		default:
			b = 0;
			fprintf( stderr, "Unknown mouse button: %d\n", button );
	}

	// button down sets the bit, up clears the bit:

	if( state == GLUT_DOWN )
	{
		Xmouse = x;
		Ymouse = y;
		ActiveButton |= b;		// set the proper bit
	}
	else
	{
		ActiveButton &= ~b;		// clear the proper bit
	}

	glutSetWindow(MainWindow);
	glutPostRedisplay();

}


// called when the mouse moves while a button is down:

void
MouseMotion( int x, int y )
{
	int dx = x - Xmouse;		// change in mouse coords
	int dy = y - Ymouse;

	if( ( ActiveButton & LEFT ) != 0 )
	{
		Xrot += ( ANGFACT*dy );
		Yrot += ( ANGFACT*dx );
	}

	if( ( ActiveButton & MIDDLE ) != 0 )
	{
		Scale += SCLFACT * (float) ( dx - dy );

		// keep object from turning inside-out or disappearing:

		if( Scale < MINSCALE )
			Scale = MINSCALE;
	}

	Xmouse = x;			// new current position
	Ymouse = y;

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// reset the transformations and the colors:
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene

void
Reset( )
{
	ActiveButton = 0;
	AxesOn = 1;
	DebugOn = 0;
	DepthBufferOn = 1;
	DepthFightingOn = 0;
	DepthCueOn = 0;
	Frozen = false;
	Light0On = Light1On = Light2On = false;
	Scale  = 1.0;
	ShadowsOn = 0;
	WhichColor = WHITE;
	WhichProjection = PERSP;
	Xrot = Yrot = 0.;
}


// called when user resizes the window:

void
Resize( int width, int height )
{
	// don't really need to do anything since window size is
	// checked each time in Display( ):

	glutSetWindow( MainWindow );
	glutPostRedisplay( );
}


// handle a change to the window's visibility:

void
Visibility ( int state )
{
	if( DebugOn != 0 )
		fprintf( stderr, "Visibility: %d\n", state );

	if( state == GLUT_VISIBLE )
	{
		glutSetWindow( MainWindow );
		glutPostRedisplay( );
	}
	else
	{
		// could optimize by keeping track of the fact
		// that the window is not visible and avoid
		// animating or redrawing it ...
	}
}



///////////////////////////////////////   HANDY UTILITIES:  //////////////////////////


// the stroke characters 'X' 'Y' 'Z' :

static float xx[ ] = { 0.f, 1.f, 0.f, 1.f };

static float xy[ ] = { -.5f, .5f, .5f, -.5f };

static int xorder[ ] = { 1, 2, -3, 4 };

static float yx[ ] = { 0.f, 0.f, -.5f, .5f };

static float yy[ ] = { 0.f, .6f, 1.f, 1.f };

static int yorder[ ] = { 1, 2, 3, -2, 4 };

static float zx[ ] = { 1.f, 0.f, 1.f, 0.f, .25f, .75f };

static float zy[ ] = { .5f, .5f, -.5f, -.5f, 0.f, 0.f };

static int zorder[ ] = { 1, 2, 3, 4, -5, 6 };

// fraction of the length to use as height of the characters:
const float LENFRAC = 0.10f;

// fraction of length to use as start location of the characters:
const float BASEFRAC = 1.10f;

//	Draw a set of 3D axes:
//	(length is the axis length in world coordinates)

void
Axes( float length )
{
	glBegin( GL_LINE_STRIP );
		glVertex3f( length, 0., 0. );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., length, 0. );
	glEnd( );
	glBegin( GL_LINE_STRIP );
		glVertex3f( 0., 0., 0. );
		glVertex3f( 0., 0., length );
	glEnd( );

	float fact = LENFRAC * length;
	float base = BASEFRAC * length;

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 4; i++ )
		{
			int j = xorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( base + fact*xx[j], fact*xy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 5; i++ )
		{
			int j = yorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( fact*yx[j], base + fact*yy[j], 0.0 );
		}
	glEnd( );

	glBegin( GL_LINE_STRIP );
		for( int i = 0; i < 6; i++ )
		{
			int j = zorder[i];
			if( j < 0 )
			{
				
				glEnd( );
				glBegin( GL_LINE_STRIP );
				j = -j;
			}
			j--;
			glVertex3f( 0.0, fact*zy[j], base + fact*zx[j] );
		}
	glEnd( );

}

void
OsuCone( float radBot, float radTop, float height, int slices, int stacks )
{
	// gracefully handle degenerate case:

	if ( radBot == 0. && radTop == 0. )
	{
		glBegin( GL_LINES );
		glNormal3f( 0., -1., 0. );
		glTexCoord2f( 0., 0. );
		glVertex3f( 0., 0., 0. );

		glNormal3f( 0., 1., 0. );
		glTexCoord2f( 0., 1. );
		glVertex3f( 0., height, 0. );
		glEnd( );
		return;
	}


	radBot = fabs( radBot );
	radTop = fabs( radTop );
	slices = fabs( slices );
	stacks = fabs( stacks );
	//fprintf( stderr, "%8.3f, %8.3f, %8.3f,  %3d, %3d\n", radBot, radTop, height, slices, stacks );

	ConeNumLngs = slices;
	if ( ConeNumLngs < 3 )
		ConeNumLngs = 3;

	ConeNumLats = stacks;
	if ( ConeNumLats < 3 )
		ConeNumLats = 3;

	// allocate the point data structure:

	ConePts = new struct point[ ConeNumLngs * ConeNumLats ];

	// fill the ConePts structure:

	for ( int ilat = 0; ilat < ConeNumLats; ilat++ )
	{
		float t = ( float )ilat / ( float )( ConeNumLats - 1 );
		float y = t * height;
		float rad = t * radTop + ( 1. - t ) * radBot;
		for ( int ilng = 0; ilng < ConeNumLngs; ilng++ )
		{
			float lng = -M_PI + 2. * M_PI * ( float )ilng / ( float )( ConeNumLngs - 1 );
			float x = cos( lng );
			float z = -sin( lng );
			struct point* p = ConePtsPointer( ilat, ilng );
			p->x = rad * x;
			p->y = y;
			p->z = rad * z;
			p->nx = height * x;
			p->ny = radBot - radTop;
			p->nz = height * z;
			Unit( &p->nx, &p->nx );
			p->s = ( float )ilng / ( float )( ConeNumLngs - 1 );
			p->t = ( float )ilat / ( float )( ConeNumLats - 1 );
		}
	}


	// draw the sides:

	for ( int ilat = 0; ilat < ConeNumLats - 1; ilat++ )
	{
		glBegin( GL_TRIANGLE_STRIP );

		struct point* p;
		p = ConePtsPointer( ilat, 0 );
		DrawPoint( p );

		p = ConePtsPointer( ilat + 1, 0 );
		DrawPoint( p );

		for ( int ilng = 1; ilng < ConeNumLngs; ilng++ )
		{
			p = ConePtsPointer( ilat, ilng );
			DrawPoint( p );

			p = ConePtsPointer( ilat + 1, ilng );
			DrawPoint( p );
		}

		glEnd( );
	}

	// draw the bottom circle:

	if ( radBot != 0. )
	{
		struct point* bot = new struct point[ ConeNumLngs ];
		for ( int ilng = 0; ilng < ConeNumLngs; ilng++ )
		{
			bot[ ilng ].x = 0.;
			bot[ ilng ].y = 0.;
			bot[ ilng ].z = 0.;
			bot[ ilng ].nx = 0.;
			bot[ ilng ].ny = -1.;
			bot[ ilng ].nz = 0.;
			bot[ ilng ].s = ( float )ilng / ( float )( ConeNumLngs - 1 );
			bot[ ilng ].t = 0.;
		}

		glBegin( GL_TRIANGLES );
		for ( int ilng = ConeNumLngs - 1; ilng >= 0; ilng-- )
		{
			struct point* p;
			p = ConePtsPointer( 0, ilng + 1 );
			DrawPoint( p );

			p = ConePtsPointer( 0, ilng );
			DrawPoint( p );

			DrawPoint( &bot[ ilng ] );
		}
		glEnd( );
		delete[ ] bot;
	}


	// draw the top circle:

	if ( radTop != 0. )
	{
		struct point* top = new struct point[ ConeNumLngs ];
		for ( int ilng = 0; ilng < ConeNumLngs; ilng++ )
		{
			top[ ilng ].x = 0.;
			top[ ilng ].y = height;
			top[ ilng ].z = 0.;
			top[ ilng ].nx = 0.;
			top[ ilng ].ny = 1.;
			top[ ilng ].nz = 0.;
			top[ ilng ].s = ( float )ilng / ( float )( ConeNumLngs - 1 );
			top[ ilng ].t = 1.;
		}

		glBegin( GL_TRIANGLES );
		for ( int ilng = 0; ilng < ConeNumLngs - 1; ilng++ )
		{
			struct point* p;
			p = ConePtsPointer( ConeNumLats - 1, ilng );
			DrawPoint( p );

			p = ConePtsPointer( ConeNumLats - 1, ilng + 1 );
			DrawPoint( p );

			DrawPoint( &top[ ilng ] );
		}
		glEnd( );
		delete[ ] top;
	}

	delete[ ] ConePts;
}

void
OsuSphere( float radius, int slices, int stacks )
{
	// set the globals:

	SphNumLngs = slices;
	SphNumLats = stacks;
	if ( SphNumLngs < 3 )
		SphNumLngs = 3;
	if ( SphNumLats < 3 )
		SphNumLats = 3;

	// allocate the point data structure:

	SphPts = new struct point[ SphNumLngs * SphNumLats ];

	// fill the SphPts structure:

	for ( int ilat = 0; ilat < SphNumLats; ilat++ )
	{
		float lat = -M_PI / 2. + M_PI * ( float )ilat / ( float )( SphNumLats - 1 );	// ilat=0/lat=0. is the south pole
		// ilat=SphNumLats-1, lat=+M_PI/2. is the north pole
		float xz = cosf( lat );
		float  y = sinf( lat );
		for ( int ilng = 0; ilng < SphNumLngs; ilng++ )				// ilng=0, lng=-M_PI and
			// ilng=SphNumLngs-1, lng=+M_PI are the same meridian
		{
			float lng = -M_PI + 2. * M_PI * ( float )ilng / ( float )( SphNumLngs - 1 );
			float x = xz * cosf( lng );
			float z = -xz * sinf( lng );
			struct point* p = SphPtsPointer( ilat, ilng );
			p->x = radius * x;
			p->y = radius * y;
			p->z = radius * z;
			p->nx = x;
			p->ny = y;
			p->nz = z;
			p->s = ( lng + M_PI ) / ( 2. * M_PI );
			p->t = ( lat + M_PI / 2. ) / M_PI;
		}
	}

	struct point top, bot;		// top, bottom points

	top.x = 0.;		top.y = radius;		top.z = 0.;
	top.nx = 0.;	top.ny = 1.;		top.nz = 0.;
	top.s = 0.;		top.t = 1.;

	bot.x = 0.;		bot.y = -radius;	bot.z = 0.;
	bot.nx = 0.;	bot.ny = -1.;		bot.nz = 0.;
	bot.s = 0.;		bot.t = 0.;

	// connect the north pole to the latitude SphNumLats-2:

	glBegin( GL_TRIANGLE_STRIP );
	for ( int ilng = 0; ilng < SphNumLngs; ilng++ )
	{
		float lng = -M_PI + 2. * M_PI * ( float )ilng / ( float )( SphNumLngs - 1 );
		top.s = ( lng + M_PI ) / ( 2. * M_PI );
		DrawPoint( &top );
		struct point* p = SphPtsPointer( SphNumLats - 2, ilng );	// ilat=SphNumLats-1 is the north pole
		DrawPoint( p );
	}
	glEnd( );

	// connect the south pole to the latitude 1:

	glBegin( GL_TRIANGLE_STRIP );
	for ( int ilng = SphNumLngs - 1; ilng >= 0; ilng-- )
	{
		float lng = -M_PI + 2. * M_PI * ( float )ilng / ( float )( SphNumLngs - 1 );
		bot.s = ( lng + M_PI ) / ( 2. * M_PI );
		DrawPoint( &bot );
		struct point* p = SphPtsPointer( 1, ilng );					// ilat=0 is the south pole
		DrawPoint( p );
	}
	glEnd( );

	// connect the horizontal strips:

	for ( int ilat = 2; ilat < SphNumLats - 1; ilat++ )
	{
		struct point* p;
		glBegin( GL_TRIANGLE_STRIP );
		for ( int ilng = 0; ilng < SphNumLngs; ilng++ )
		{
			p = SphPtsPointer( ilat, ilng );
			DrawPoint( p );
			p = SphPtsPointer( ilat - 1, ilng );
			DrawPoint( p );
		}
		glEnd( );
	}

	// clean-up:

	delete[ ] SphPts;
	SphPts = NULL;
}

// read a BMP file into a Texture:

#define VERBOSE				false
#define BMP_MAGIC_NUMBER	0x4d42
#ifndef BI_RGB
#define BI_RGB				0
#define BI_RLE8				1
#define BI_RLE4				2
#endif


// bmp file header:
struct bmfh
{
	short bfType;		// BMP_MAGIC_NUMBER = "BM"
	int bfSize;		// size of this file in bytes
	short bfReserved1;
	short bfReserved2;
	int bfOffBytes;		// # bytes to get to the start of the per-pixel data
} FileHeader;

// bmp info header:
struct bmih
{
	int biSize;		// info header size, should be 40
	int biWidth;		// image width
	int biHeight;		// image height
	short biPlanes;		// #color planes, should be 1
	short biBitCount;	// #bits/pixel, should be 1, 4, 8, 16, 24, 32
	int biCompression;	// BI_RGB, BI_RLE4, BI_RLE8
	int biSizeImage;
	int biXPixelsPerMeter;
	int biYPixelsPerMeter;
	int biClrUsed;		// # colors in the palette
	int biClrImportant;
} InfoHeader;



// read a BMP file into a Texture:

unsigned char *
BmpToTexture( char *filename, int *width, int *height )
{
	FILE *fp;
#ifdef _WIN32
        errno_t err = fopen_s( &fp, filename, "rb" );
        if( err != 0 )
        {
		fprintf( stderr, "Cannot open Bmp file '%s'\n", filename );
		return NULL;
        }
#else
		fp = fopen( filename, "rb" );
		if( fp == NULL )
		{
			fprintf( stderr, "Cannot open Bmp file '%s'\n", filename );
			return NULL;
		}
#endif

	FileHeader.bfType = ReadShort( fp );


	// if bfType is not BMP_MAGIC_NUMBER, the file is not a bmp:

	if( VERBOSE ) fprintf( stderr, "FileHeader.bfType = 0x%0x = \"%c%c\"\n",
			FileHeader.bfType, FileHeader.bfType&0xff, (FileHeader.bfType>>8)&0xff );
	if( FileHeader.bfType != BMP_MAGIC_NUMBER )
	{
		fprintf( stderr, "Wrong type of file: 0x%0x\n", FileHeader.bfType );
		fclose( fp );
		return NULL;
	}


	FileHeader.bfSize = ReadInt( fp );
	if( VERBOSE )	fprintf( stderr, "FileHeader.bfSize = %d\n", FileHeader.bfSize );

	FileHeader.bfReserved1 = ReadShort( fp );
	FileHeader.bfReserved2 = ReadShort( fp );

	FileHeader.bfOffBytes = ReadInt( fp );


	InfoHeader.biSize = ReadInt( fp );
	InfoHeader.biWidth = ReadInt( fp );
	InfoHeader.biHeight = ReadInt( fp );

	const int nums = InfoHeader.biWidth;
	const int numt = InfoHeader.biHeight;

	InfoHeader.biPlanes = ReadShort( fp );

	InfoHeader.biBitCount = ReadShort( fp );
	if( VERBOSE )	fprintf( stderr, "InfoHeader.biBitCount = %d\n", InfoHeader.biBitCount );

	InfoHeader.biCompression = ReadInt( fp );
	if( VERBOSE )	fprintf( stderr, "InfoHeader.biCompression = %d\n", InfoHeader.biCompression );

	InfoHeader.biSizeImage = ReadInt( fp );
	if( VERBOSE )	fprintf( stderr, "InfoHeader.biSizeImage = %d\n", InfoHeader.biSizeImage );

	InfoHeader.biXPixelsPerMeter = ReadInt( fp );
	InfoHeader.biYPixelsPerMeter = ReadInt( fp );

	InfoHeader.biClrUsed = ReadInt( fp );
	if( VERBOSE )	fprintf( stderr, "InfoHeader.biClrUsed = %d\n", InfoHeader.biClrUsed );

	InfoHeader.biClrImportant = ReadInt( fp );

	// fprintf( stderr, "Image size found: %d x %d\n", ImageWidth, ImageHeight );

	// pixels will be stored bottom-to-top, left-to-right:
	unsigned char *texture = new unsigned char[ 3 * nums * numt ];
	if( texture == NULL )
	{
		fprintf( stderr, "Cannot allocate the texture array!\n" );
		return NULL;
	}

	// extra padding bytes:

	int requiredRowSizeInBytes = 4 * ( ( InfoHeader.biBitCount*InfoHeader.biWidth + 31 ) / 32 );
	if( VERBOSE )	fprintf( stderr, "requiredRowSizeInBytes = %d\n", requiredRowSizeInBytes );

	int myRowSizeInBytes = ( InfoHeader.biBitCount*InfoHeader.biWidth + 7 ) / 8;
	if( VERBOSE )	fprintf( stderr, "myRowSizeInBytes = %d\n", myRowSizeInBytes );

	int numExtra = requiredRowSizeInBytes - myRowSizeInBytes;
	if( VERBOSE )	fprintf( stderr, "New NumExtra padding = %d\n", numExtra );


	// this function does not support compression:

	if( InfoHeader.biCompression != 0 )
	{
		fprintf( stderr, "Wrong type of image compression: %d\n", InfoHeader.biCompression );
		fclose( fp );
		return NULL;
	}
	
	// we can handle 24 bits of direct color:
	if( InfoHeader.biBitCount == 24 )
	{
		rewind( fp );
		fseek( fp, FileHeader.bfOffBytes, SEEK_SET );
		int t;
		unsigned char *tp;
		for( t = 0, tp = texture; t < numt; t++ )
		{
			for( int s = 0; s < nums; s++, tp += 3 )
			{
				*(tp+2) = fgetc( fp );		// b
				*(tp+1) = fgetc( fp );		// g
				*(tp+0) = fgetc( fp );		// r
			}

			for( int e = 0; e < numExtra; e++ )
			{
				fgetc( fp );
			}
		}
	}

	// we can also handle 8 bits of indirect color:
	if( InfoHeader.biBitCount == 8 && InfoHeader.biClrUsed == 256 )
	{
		struct rgba32
		{
			unsigned char r, g, b, a;
		};
		struct rgba32 *colorTable = new struct rgba32[ InfoHeader.biClrUsed ];

		rewind( fp );
		fseek( fp, sizeof(struct bmfh) + InfoHeader.biSize - 2, SEEK_SET );
		for( int c = 0; c < InfoHeader.biClrUsed; c++ )
		{
			colorTable[c].r = fgetc( fp );
			colorTable[c].g = fgetc( fp );
			colorTable[c].b = fgetc( fp );
			colorTable[c].a = fgetc( fp );
			if( VERBOSE )	fprintf( stderr, "%4d:\t0x%02x\t0x%02x\t0x%02x\t0x%02x\n",
				c, colorTable[c].r, colorTable[c].g, colorTable[c].b, colorTable[c].a );
		}

		rewind( fp );
		fseek( fp, FileHeader.bfOffBytes, SEEK_SET );
		int t;
		unsigned char *tp;
		for( t = 0, tp = texture; t < numt; t++ )
		{
			for( int s = 0; s < nums; s++, tp += 3 )
			{
				int index = fgetc( fp );
				*(tp+0) = colorTable[index].r;	// r
				*(tp+1) = colorTable[index].g;	// g
				*(tp+2) = colorTable[index].b;	// b
			}

			for( int e = 0; e < numExtra; e++ )
			{
				fgetc( fp );
			}
		}

		delete[ ] colorTable;
	}

	fclose( fp );

	*width = nums;
	*height = numt;
	return texture;
}

int
ReadInt( FILE *fp )
{
	const unsigned char b0 = fgetc( fp );
	const unsigned char b1 = fgetc( fp );
	const unsigned char b2 = fgetc( fp );
	const unsigned char b3 = fgetc( fp );
	return ( b3 << 24 )  |  ( b2 << 16 )  |  ( b1 << 8 )  |  b0;
}

short
ReadShort( FILE *fp )
{
	const unsigned char b0 = fgetc( fp );
	const unsigned char b1 = fgetc( fp );
	return ( b1 << 8 )  |  b0;
}

int
LoadObjFile( char* name )
{
	char* cmd;		// the command string
	char* str;		// argument string

	std::vector <struct Vertex> Vertices( 10000 );
	std::vector <struct Normal> Normals( 10000 );
	std::vector <struct TextureCoord> TextureCoords( 10000 );

	Vertices.clear( );
	Normals.clear( );
	TextureCoords.clear( );

	struct Vertex sv;
	struct Normal sn;
	struct TextureCoord st;


	// open the input file:

	FILE* fp = fopen( name, "r" );
	if ( fp == NULL )
	{
		fprintf( stderr, "Cannot open .obj file '%s'\n", name );
		return 1;
	}


	float xmin = 1.e+37f;
	float ymin = 1.e+37f;
	float zmin = 1.e+37f;
	float xmax = -xmin;
	float ymax = -ymin;
	float zmax = -zmin;

	glBegin( GL_TRIANGLES );

	for ( ; ; )
	{
		char* line = ReadRestOfLine( fp );
		if ( line == NULL )
			break;


		// skip this line if it is a comment:

		if ( line[ 0 ] == '#' )
			continue;


		// skip this line if it is something we don't feel like handling today:

		if ( line[ 0 ] == 'g' )
			continue;

		if ( line[ 0 ] == 'm' )
			continue;

		if ( line[ 0 ] == 's' )
			continue;

		if ( line[ 0 ] == 'u' )
			continue;


		// get the command string:

		cmd = strtok( line, OBJDELIMS );


		// skip this line if it is empty:

		if ( cmd == NULL )
			continue;


		if ( strcmp( cmd, "v" ) == 0 )
		{
			str = strtok( NULL, OBJDELIMS );
			sv.x = atof( str );

			str = strtok( NULL, OBJDELIMS );
			sv.y = atof( str );

			str = strtok( NULL, OBJDELIMS );
			sv.z = atof( str );

			Vertices.push_back( sv );

			if ( sv.x < xmin )	xmin = sv.x;
			if ( sv.x > xmax )	xmax = sv.x;
			if ( sv.y < ymin )	ymin = sv.y;
			if ( sv.y > ymax )	ymax = sv.y;
			if ( sv.z < zmin )	zmin = sv.z;
			if ( sv.z > zmax )	zmax = sv.z;

			continue;
		}


		if ( strcmp( cmd, "vn" ) == 0 )
		{
			str = strtok( NULL, OBJDELIMS );
			sn.nx = atof( str );

			str = strtok( NULL, OBJDELIMS );
			sn.ny = atof( str );

			str = strtok( NULL, OBJDELIMS );
			sn.nz = atof( str );

			Normals.push_back( sn );

			continue;
		}


		if ( strcmp( cmd, "vt" ) == 0 )
		{
			st.s = st.t = st.p = 0.;

			str = strtok( NULL, OBJDELIMS );
			st.s = atof( str );

			str = strtok( NULL, OBJDELIMS );
			if ( str != NULL )
				st.t = atof( str );

			str = strtok( NULL, OBJDELIMS );
			if ( str != NULL )
				st.p = atof( str );

			TextureCoords.push_back( st );

			continue;
		}


		if ( strcmp( cmd, "f" ) == 0 )
		{
			struct face vertices[ 10 ];
			for ( int i = 0; i < 10; i++ )
			{
				vertices[ i ].v = 0;
				vertices[ i ].n = 0;
				vertices[ i ].t = 0;
			}

			int sizev = ( int )Vertices.size( );
			int sizen = ( int )Normals.size( );
			int sizet = ( int )TextureCoords.size( );

			int numVertices = 0;
			bool valid = true;
			int vtx = 0;
			char* str;
			while ( ( str = strtok( NULL, OBJDELIMS ) ) != NULL )
			{
				int v, n, t;
				ReadObjVTN( str, &v, &t, &n );

				// if v, n, or t are negative, they are wrt the end of their respective list:

				if ( v < 0 )
					v += ( sizev + 1 );

				if ( n < 0 )
					n += ( sizen + 1 );

				if ( t < 0 )
					t += ( sizet + 1 );


				// be sure we are not out-of-bounds (<vector> will abort):

				if ( t > sizet )
				{
					if ( t != 0 )
						fprintf( stderr, "Read texture coord %d, but only have %d so far\n", t, sizet );
					t = 0;
				}

				if ( n > sizen )
				{
					if ( n != 0 )
						fprintf( stderr, "Read normal %d, but only have %d so far\n", n, sizen );
					n = 0;
				}

				if ( v > sizev )
				{
					if ( v != 0 )
						fprintf( stderr, "Read vertex coord %d, but only have %d so far\n", v, sizev );
					v = 0;
					valid = false;
				}

				vertices[ vtx ].v = v;
				vertices[ vtx ].n = n;
				vertices[ vtx ].t = t;
				vtx++;

				if ( vtx >= 10 )
					break;

				numVertices++;
			}


			// if vertices are invalid, don't draw anything this time:

			if ( !valid )
				continue;

			if ( numVertices < 3 )
				continue;


			// list the vertices:

			int numTriangles = numVertices - 2;

			for ( int it = 0; it < numTriangles; it++ )
			{
				int vv[ 3 ];
				vv[ 0 ] = 0;
				vv[ 1 ] = it + 1;
				vv[ 2 ] = it + 2;

				// get the planar normal, in case vertex normals are not defined:

				struct Vertex* v0 = &Vertices[ vertices[ vv[ 0 ] ].v - 1 ];
				struct Vertex* v1 = &Vertices[ vertices[ vv[ 1 ] ].v - 1 ];
				struct Vertex* v2 = &Vertices[ vertices[ vv[ 2 ] ].v - 1 ];

				float v01[ 3 ], v02[ 3 ], norm[ 3 ];
				v01[ 0 ] = v1->x - v0->x;
				v01[ 1 ] = v1->y - v0->y;
				v01[ 2 ] = v1->z - v0->z;
				v02[ 0 ] = v2->x - v0->x;
				v02[ 1 ] = v2->y - v0->y;
				v02[ 2 ] = v2->z - v0->z;
				Cross( v01, v02, norm );
				Unit( norm, norm );
				glNormal3fv( norm );

				for ( int vtx = 0; vtx < 3; vtx++ )
				{
					if ( vertices[ vv[ vtx ] ].t != 0 )
					{
						struct TextureCoord* tp = &TextureCoords[ vertices[ vv[ vtx ] ].t - 1 ];
						glTexCoord2f( tp->s, tp->t );
					}

					if ( vertices[ vv[ vtx ] ].n != 0 )
					{
						struct Normal* np = &Normals[ vertices[ vv[ vtx ] ].n - 1 ];
						glNormal3f( np->nx, np->ny, np->nz );
					}

					struct Vertex* vp = &Vertices[ vertices[ vv[ vtx ] ].v - 1 ];
					glVertex3f( vp->x, vp->y, vp->z );
				}
			}
			continue;
		}


		if ( strcmp( cmd, "s" ) == 0 )
		{
			continue;
		}

	}

	glEnd( );
	fclose( fp );

	fprintf( stderr, "Obj file range: [%8.3f,%8.3f,%8.3f] -> [%8.3f,%8.3f,%8.3f]\n",
		xmin, ymin, zmin, xmax, ymax, zmax );
	fprintf( stderr, "Obj file center = (%8.3f,%8.3f,%8.3f)\n",
		( xmin + xmax ) / 2., ( ymin + ymax ) / 2., ( zmin + zmax ) / 2. );
	fprintf( stderr, "Obj file  span = (%8.3f,%8.3f,%8.3f)\n",
		xmax - xmin, ymax - ymin, zmax - zmin );

	return 0;
}


// function to convert HSV to RGB
// 0.  <=  s, v, r, g, b  <=  1.
// 0.  <= h  <=  360.
// when this returns, call:
//		glColor3fv( rgb );

void
HsvRgb( float hsv[3], float rgb[3] )
{
	// guarantee valid input:

	float h = hsv[0] / 60.f;
	while( h >= 6. )	h -= 6.;
	while( h <  0. ) 	h += 6.;

	float s = hsv[1];
	if( s < 0. )
		s = 0.;
	if( s > 1. )
		s = 1.;

	float v = hsv[2];
	if( v < 0. )
		v = 0.;
	if( v > 1. )
		v = 1.;

	// if sat==0, then is a gray:

	if( s == 0.0 )
	{
		rgb[0] = rgb[1] = rgb[2] = v;
		return;
	}

	// get an rgb from the hue itself:
	
	float i = (float)floor( h );
	float f = h - i;
	float p = v * ( 1.f - s );
	float q = v * ( 1.f - s*f );
	float t = v * ( 1.f - ( s * (1.f-f) ) );

	float r=0., g=0., b=0.;			// red, green, blue
	switch( (int) i )
	{
		case 0:
			r = v;	g = t;	b = p;
			break;
	
		case 1:
			r = q;	g = v;	b = p;
			break;
	
		case 2:
			r = p;	g = v;	b = t;
			break;
	
		case 3:
			r = p;	g = q;	b = v;
			break;
	
		case 4:
			r = t;	g = p;	b = v;
			break;
	
		case 5:
			r = v;	g = p;	b = q;
			break;
	}


	rgb[0] = r;
	rgb[1] = g;
	rgb[2] = b;
}

void
Cross(float v1[3], float v2[3], float vout[3])
{
	float tmp[3];
	tmp[0] = v1[1] * v2[2] - v2[1] * v1[2];
	tmp[1] = v2[0] * v1[2] - v1[0] * v2[2];
	tmp[2] = v1[0] * v2[1] - v2[0] * v1[1];
	vout[0] = tmp[0];
	vout[1] = tmp[1];
	vout[2] = tmp[2];
}

float
Dot(float v1[3], float v2[3])
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
}

float
Unit( float v[ 3 ] )
{
	float dist;

	dist = v[ 0 ] * v[ 0 ] + v[ 1 ] * v[ 1 ] + v[ 2 ] * v[ 2 ];

	if ( dist > 0.0 )
	{
		dist = sqrt( dist );
		v[ 0 ] /= dist;
		v[ 1 ] /= dist;
		v[ 2 ] /= dist;
	}

	return dist;
}


float
Unit(float vin[3], float vout[3])
{
	float dist = vin[0] * vin[0] + vin[1] * vin[1] + vin[2] * vin[2];
	if (dist > 0.0)
	{
		dist = sqrtf(dist);
		vout[0] = vin[0] / dist;
		vout[1] = vin[1] / dist;
		vout[2] = vin[2] / dist;
	}
	else
	{
		vout[0] = vin[0];
		vout[1] = vin[1];
		vout[2] = vin[2];
	}
	return dist;
}

void
SetMaterial( float r, float g, float b, float shininess )
{
	glMaterialfv( GL_BACK, GL_EMISSION, Array3( 0., 0., 0. ) );
	glMaterialfv( GL_BACK, GL_AMBIENT, MulArray3( .4f, White ) );
	glMaterialfv( GL_BACK, GL_DIFFUSE, MulArray3( 1., White ) );
	glMaterialfv( GL_BACK, GL_SPECULAR, Array3( 0., 0., 0. ) );
	glMaterialf( GL_BACK, GL_SHININESS, 5.f );

	glMaterialfv( GL_FRONT, GL_EMISSION, Array3( 0., 0., 0. ) );
	glMaterialfv( GL_FRONT, GL_AMBIENT, Array3( r, g, b ) );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, Array3( r, g, b ) );
	glMaterialfv( GL_FRONT, GL_SPECULAR, MulArray3( .8f, White ) );
	glMaterialf( GL_FRONT, GL_SHININESS, shininess );
}


void
SetPointLight( int ilight, float x, float y, float z, float r, float g, float b, float const_attn, float lin_attn, float quad_attn )
{
	glLightfv( ilight, GL_POSITION, Array3( x, y, z ) );
	glLightfv( ilight, GL_AMBIENT, Array3( 0., 0., 0. ) );
	glLightfv( ilight, GL_DIFFUSE, Array3( r, g, b ) );
	glLightfv( ilight, GL_SPECULAR, Array3( r, g, b ) );
	glLightf( ilight, GL_CONSTANT_ATTENUATION, const_attn );
	glLightf( ilight, GL_LINEAR_ATTENUATION, lin_attn );
	glLightf( ilight, GL_QUADRATIC_ATTENUATION, quad_attn );
	glEnable( ilight );
}


void
SetSpotLight( int ilight, float x, float y, float z, float xdir, float ydir, float zdir, float r, float g, float b )
{
	glLightfv( ilight, GL_POSITION, Array3( x, y, z ) );
	glLightfv( ilight, GL_SPOT_DIRECTION, Array3( xdir, ydir, zdir ) );
	glLightf( ilight, GL_SPOT_EXPONENT, 1. );
	glLightf( ilight, GL_SPOT_CUTOFF, 45. );
	glLightfv( ilight, GL_AMBIENT, Array3( 0., 0., 0. ) );
	glLightfv( ilight, GL_DIFFUSE, Array3( r, g, b ) );
	glLightfv( ilight, GL_SPECULAR, Array3( r, g, b ) );
	glLightf( ilight, GL_CONSTANT_ATTENUATION, 1. );
	glLightf( ilight, GL_LINEAR_ATTENUATION, 0. );
	glLightf( ilight, GL_QUADRATIC_ATTENUATION, 0. );
	glEnable( ilight );
}

float *
Array3( float a, float b, float c )
{
	static float array[ 4 ];

	array[ 0 ] = a;
	array[ 1 ] = b;
	array[ 2 ] = c;
	array[ 3 ] = 1.;
	return array;
}

float *
Array4( float a, float b, float c, float d )
{
	static float array[ 4 ];

	array[ 0 ] = a;
	array[ 1 ] = b;
	array[ 2 ] = c;
	array[ 3 ] = d;
	return array;
}

float *
BlendArray3( float factor, float array0[ 3 ], float array1[ 3 ] )
{
	static float array[ 4 ];

	array[ 0 ] = factor * array0[ 0 ] + ( 1.f - factor ) * array1[ 0 ];
	array[ 1 ] = factor * array0[ 1 ] + ( 1.f - factor ) * array1[ 1 ];
	array[ 2 ] = factor * array0[ 2 ] + ( 1.f - factor ) * array1[ 2 ];
	array[ 3 ] = 1.;
	return array;
}

float *
MulArray3( float factor, float array0[ 3 ] )
{
	static float array[ 4 ];

	array[ 0 ] = factor * array0[ 0 ];
	array[ 1 ] = factor * array0[ 1 ];
	array[ 2 ] = factor * array0[ 2 ];
	array[ 3 ] = 1.;
	return array;
}

char*
ReadRestOfLine( FILE* fp )
{
	static char* line;
	std::vector<char> tmp( 1000 );
	tmp.clear( );

	for ( ; ; )
	{
		int c = getc( fp );

		if ( c == EOF && tmp.size( ) == 0 )
		{
			return NULL;
		}

		if ( c == EOF || c == '\n' )
		{
			delete[ ] line;
			line = new char[ tmp.size( ) + 1 ];
			for ( int i = 0; i < ( int )tmp.size( ); i++ )
			{
				line[ i ] = tmp[ i ];
			}
			line[ tmp.size( ) ] = '\0';	// terminating null
			return line;
		}
		else
		{
			tmp.push_back( c );
		}
	}

	return "";
}


void
ReadObjVTN( char* str, int* v, int* t, int* n )
{
	// can be one of v, v//n, v/t, v/t/n:

	if ( strstr( str, "//" ) )				// v//n
	{
		*t = 0;
		sscanf( str, "%d//%d", v, n );
		return;
	}
	else if ( sscanf( str, "%d/%d/%d", v, t, n ) == 3 )	// v/t/n
	{
		return;
	}
	else
	{
		*n = 0;
		if ( sscanf( str, "%d/%d", v, t ) == 2 )		// v/t
		{
			return;
		}
		else						// v
		{
			*n = *t = 0;
			sscanf( str, "%d", v );
		}
	}
}