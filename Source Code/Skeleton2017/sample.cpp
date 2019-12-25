/*Running time*/

#include <stdio.h>
// yes, I know stdio.h is not good C++, but I like the *printf()
#include <stdlib.h>
#include <ctype.h>

#define _USE_MATH_DEFINES
#include <math.h>

#ifdef WIN32
#include <windows.h>
#pragma warning(disable:4996)
#endif


// You need to adjust the location of these header files according to your configuration

#include <windows.h>
//#include <freeglut.h>
#include "GL\include\GL.H"
#include "GL\include\glu.h"
#include "GL\include\glut.h"
#include "GL\include\glui.h"
#include <iostream>
#include "Skeleton.h"
#include <time.h>
//#include "glui.h"
#include <float.h>
using namespace std;

//
//
//	This is a sample OpenGL / GLUT / GLUI program
//
//	The objective is to draw a 3d object and change the color of the axes
//		with radio buttons
//
//	The left mouse button allows rotation
//	The middle mouse button allows scaling
//	The glui window allows:
//		1. The 3d object to be transformed
//		2. The projection to be changed
//		3. The color of the axes to be changed
//		4. The axes to be turned on and off
//		5. The transformations to be reset
//		6. The program to quit
//
//	Author: Joe Graphics
//


//
// constants:
//
// NOTE: There are a bunch of good reasons to use const variables instead
// of #define's.  However, Visual C++ does not allow a const variable
// to be used as an array size or as the case in a switch() statement.  So in
// the following, all constants are const variables except those which need to
// be array sizes or cases in switch() statements.  Those are #defines.
//
//


// This source code has been modified by Guoning Chen since its release


// title of these windows:

const char* WINDOWTITLE = { "Visualization -- Final Project" };
const char* GLUITITLE = { "User Interface Window" };


// what the glui package defines as true and false:

const int GLUITRUE = { true };
const int GLUIFALSE = { false };


// the escape key:

#define ESCAPE		0x1b


// initial window size:

const int INIT_WINDOW_SIZE = { 600 };


// size of the box:

const float BOXSIZE = { 2.f };



// multiplication factors for input interaction:
//  (these are known from previous experience)

const float ANGFACT = { 1. };
const float SCLFACT = { 0.005f };
float white_value;
float S_MIN, S_MAX;
float s_mid;
int color;


const int IMG_RES = 512;
unsigned char vec_img[IMG_RES][IMG_RES][3];


// able to use the left mouse for either rotation or scaling,
// in case have only a 2-button mouse:

enum LeftButton
{
	ROTATE,
	SCALE
};


// minimum allowable scale factor:

const float MINSCALE = { 0.05f };


// active mouse buttons (or them together):

const int LEFT = { 4 };
const int MIDDLE = { 2 };
const int RIGHT = { 1 };


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

const float BACKCOLOR[] = { 0., 0., 0., 0. };


// line width for the axes:

const GLfloat AXES_WIDTH = { 3. };


// the color numbers:
// this order must match the radio button order

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

enum COLOR_CODING
{
	RAINBOW,
	BLUE_WHITE_RED,
	HEAT_MAP,
	NON_LINEAR
};
// the object numbers:
// 
enum MODELS
{
	BNOISE,
	BRUNO3,
	CNOISE,
	DIPOLE,
	VNOISE
};

// the color definitions:
// this order must match the radio button order

const GLfloat Colors[8][3] =
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

const GLfloat FOGCOLOR[4] = { .0, .0, .0, 1. };
const GLenum  FOGMODE = { GL_LINEAR };
const GLfloat FOGDENSITY = { 0.30f };
const GLfloat FOGSTART = { 1.5 };
const GLfloat FOGEND = { 4. };



//
// non-constant global variables:
//

int	ActiveButton;		// current button that is down
GLuint	AxesList;		// list to hold the axes
int	AxesOn;			// != 0 means to draw the axes
int	DebugOn;			// != 0 means to print debugging info
int	DepthCueOn;		// != 0 means to use intensity depth cueing
int DiscreteOn;
int ArrowOn;
int LicOn;
int colorplot = 1;
int BlendOn;
GLUI* Glui;			// instance of glui window
int	GluiWindow;		// the glut id for the glui window
int	LeftButton;		// either ROTATE or SCALE
GLuint	BoxList;		// object display list
int	MainWindow;		// window id for main graphics window
GLfloat	RotMatrix[4][4];	// set by glui rotation widget
float	Scale, Scale2;		// scaling factors
int	WhichColor;		// index into Colors[]
int	WhichProjection;	// ORTHO or PERSP
int	Xmouse, Ymouse;		// mouse values
float	Xrot, Yrot;		// rotation angles in degrees
float	TransXYZ[3];		// set by glui translation widgets
float max_vx, min_vx, max_vy, min_vy;

//
// function prototypes:
//
void	Animate(void);
void	Buttons(int);
void	Display(void);
void	DoRasterString(float, float, float, char*);
void	DoStrokeString(float, float, float, float, char*);
float	ElapsedSeconds(void);
void	InitGlui(void);
void	InitGraphics(void);
void	InitLists(void);
void	Keyboard(unsigned char, int, int);
void	MouseButton(int, int, int, int);
void	MouseMotion(int, int);
void	Reset(void);
void	Resize(int, int);
void	Visibility(int);
void    draw_arrow_head(double head[2], float direct[2]);
void	Arrow(float[3], float[3]);
void	Cross(float[3], float[3], float[3]);
float	Dot(float[3], float[3]);
float	Unit(float[3], float[3]);
void	Axes(float);
void	HsvRgb(float[3], float[3]);
void    compute_lic();
void	CalculateFirstStreamLine();
void    Display_Model(void);
void    set_view(GLenum mode, Polyhedron* poly);
void    set_scene(GLenum mode, Polyhedron* poly);
void    display_shape(GLenum mode, Polyhedron* this_poly);
void    Choose_Object();
void    Choose_Object_Color();
void    get_s_min_and_s_max(Polyhedron* this_poly);
void    max_and_min(Polyhedron* this_poly);
double radius_factor = 0.9;
int     display_mode = 0;
int ObjectId = 0;
int ObjectColorId = 0;
int WhiteId = 0;
int VectorId = 0;
char object_name[128] = "bnoise";
void    render_vec_img(Polyhedron* this_poly);


Polyhedron* poly = NULL;

/**********************************************************************************************************************************************************/
typedef struct seedPoint {
	float x, y;
	int ii, jj;
};


void disp_streamlines();
void get_forward_seedpoints();
void get_backward_seedpoints();

vector <seedPoint> SeedPoints;
vector <seedPoint> InitialStreamLine;

vector <vector <seedPoint> > StreamLines;
vector <vector <seedPoint> > StreamLines1;
int all_points[512][512] = { 0 };

float d_sep = 20;
int XLength, YLength;
int stream_count = 0;
int d_test = 10;
float init_i = rand() % 256;
float init_j = rand() % 256;
/**********************************************************************************************************************************************************/
// main program:
int main(int argc, char* argv[])
{
	// turn on the glut package:
	// (do this before checking argc and argv since it might
	// pull some command line arguments out)

	glutInit(&argc, argv);

	// Load the model and data here
	FILE* this_file = fopen("../models/bnoise.ply", "r");
	poly = new Polyhedron(this_file);
	fclose(this_file);
	//mat_ident( rotmat );	

	poly->initialize(); // initialize everything

	poly->calc_bounding_sphere();
	poly->calc_face_normals_and_area();
	poly->average_normals();

	// setup all the graphics stuff:
	InitGraphics();

	/**************************************************/

	render_vec_img(poly);
	StreamLines1.clear();
	XLength = YLength = sqrt(poly->nverts);
	//init_i = 256;
	//init_j = 256;
	CalculateFirstStreamLine();
	get_forward_seedpoints();
	//compute_lic();
	get_backward_seedpoints();
	compute_lic();
	//CalculateFirstStreamLine();
	float xi = init_i;
	float yi = init_j;
	for (int xyi = 0; xyi < 11; xyi++)
	{
		float x = init_i;
		float y = init_j;
		for (int c = 0; c < 171; c++)
		{

			//StreamLines.clear();
			init_i = init_i;
			init_j = init_j + (3 * d_test);
			CalculateFirstStreamLine();
			get_forward_seedpoints();
			//compute_lic();
			get_backward_seedpoints();
			compute_lic();
		}
		init_i = x;
		init_j = y;
		for (int c = 0; c < 171; c++)
		{
			//StreamLines.clear();
			init_i = init_i;
			init_j = init_j - (3 * d_test);

			CalculateFirstStreamLine();
			get_forward_seedpoints();
			//compute_lic();
			get_backward_seedpoints();
			compute_lic();
		}

		init_i = x + 50;
		init_j = y;
	}
	init_i = xi;
	init_j = yi;
	for (int xi = 0; xi < 11; xi++)
	{
		float x = init_i;
		float y = init_j;
		for (int c = 0; c < 171; c++)
		{

			//StreamLines.clear();
			init_i = init_i;
			init_j = init_j + (3 * d_test);
			CalculateFirstStreamLine();
			get_forward_seedpoints();
			//compute_lic();
			get_backward_seedpoints();
			compute_lic();
		}
		init_i = x;
		init_j = y;
		for (int c = 0; c < 171; c++)
		{
			//StreamLines.clear();
			init_i = init_i;
			init_j = init_j - (3 * d_test);

			CalculateFirstStreamLine();
			get_forward_seedpoints();
			//compute_lic();
			get_backward_seedpoints();
			compute_lic();
		}

		init_i = x - 50;
		init_j = y;
	}


	/**************************************************/


	// create the display structures that will not change:

	InitLists();


	// init all the global variables used by Display():
	// this will also post a redisplay
	// it is important to call this before InitGlui()
	// so that the variables that glui will control are correct
	// when each glui widget is created

	Reset();


	// setup all the user interface stuff:

	InitGlui();


	// draw the scene once and wait for some interaction:
	// (will never return)

	glutMainLoop();

	// finalize the object if loaded

	if (poly != NULL)
		poly->finalize();

	// this is here to make the compiler happy:

	return 0;
}



//
// this is where one would put code that is to be called
// everytime the glut main loop has nothing to do
//
// this is typically where animation parameters are set
//
// do not call Display() from here -- let glutMainLoop() do it
//

void
Animate(void)
{
	// put animation stuff in here -- change some global variables
	// for Display() to find:



	// force a call to Display() next time it is convenient:

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}




//
// glui buttons callback:
//

void Buttons(int id)
{
	switch (id)
	{
	case RESET:
		Reset();
		Glui->sync_live();
		glutSetWindow(MainWindow);
		glutPostRedisplay();
		break;

	case QUIT:
		// gracefully close the glui window:
		// gracefully close out the graphics:
		// gracefully close the graphics window:
		// gracefully exit the program:

		Glui->close();
		glutSetWindow(MainWindow);
		glFinish();
		glutDestroyWindow(MainWindow);
		exit(0);
		break;

	default:
		fprintf(stderr, "Don't know what to do with Button ID %d\n", id);
	}

}



//
// draw the complete scene:
//

void Display(void)
{
	glViewport(0, 0, (GLsizei)IMG_RES, (GLsizei)IMG_RES);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, 1, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);

	if (colorplot == 1) {
		//display_shape(GL_RENDER, poly);
		disp_streamlines();
	}

	if (LicOn == 1) {
	}

	if (ArrowOn == 1)
	{
		int i = 0;
		while (i < poly->ntris) {

			Triangle* temp_t = poly->tlist[i];


			for (int j = 0; j < 3; j++)
			{
				Vertex* temp_v = temp_t->verts[j];
				double head[2];
				head[0] = temp_v->x;
				head[1] = temp_v->y;
				float direct[2];
				direct[0] = temp_v->vx;
				direct[1] = temp_v->vy;
				draw_arrow_head(head, direct);
			}
			if (ObjectId == BRUNO3)
			{
				i = i + 1;
			}
			else if (ObjectId == CNOISE)
			{
				i = i + 15;
			}
			else
			{
				i = i + 6;
			}
		}
	}
	// swap the double-buffered framebuffers:
	//StreamLines1.clear();

	glutSwapBuffers();


	// be sure the graphics buffer has been sent:
	// note: be sure to use glFlush() here, not glFinish() !

	glFlush();
}



//
// use glut to display a string of characters using a raster font:
//

void
DoRasterString(float x, float y, float z, char* s)
{
	char c;			// one character to print

	glRasterPos3f((GLfloat)x, (GLfloat)y, (GLfloat)z);
	for (; (c = *s) != '\0'; s++)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, c);
	}
}



//
// use glut to display a string of characters using a stroke font:
//

void
DoStrokeString(float x, float y, float z, float ht, char* s)
{
	char c;			// one character to print
	float sf;		// the scale factor

	glPushMatrix();
	glTranslatef((GLfloat)x, (GLfloat)y, (GLfloat)z);
	sf = ht / (119.05 + 33.33);
	glScalef((GLfloat)sf, (GLfloat)sf, (GLfloat)sf);
	for (; (c = *s) != '\0'; s++)
	{
		glutStrokeCharacter(GLUT_STROKE_ROMAN, c);
	}
	glPopMatrix();
}



//
// return the number of seconds since the start of the program:
//

float ElapsedSeconds(void)
{
	// get # of milliseconds since the start of the program:

	int ms = glutGet(GLUT_ELAPSED_TIME);

	// convert it to seconds:

	return (float)ms / 1000.;
}



//
// initialize the glui window:
//

void InitGlui(void)
{
	GLUI_Panel* panel;
	GLUI_RadioGroup* group;
	GLUI_Rotation* rot;
	GLUI_Translation* trans, * scale;
	GLUI_Spinner* spinner;

	// setup the glui window:

	glutInitWindowPosition(INIT_WINDOW_SIZE + 50, 0);
	Glui = GLUI_Master.create_glui((char*)GLUITITLE);


	Glui->add_statictext((char*)GLUITITLE);
	Glui->add_separator();


	// Add a list for the different models
	//rollout = Glui->add_rollout(" Models ", 0);
	panel = Glui->add_panel("Choose object to open ");
	GLUI_Listbox* obj_list = Glui->add_listbox_to_panel(panel, "Objects", &ObjectId, -1, (GLUI_Update_CB)Choose_Object);
	obj_list->add_item(0, "bnoise");
	obj_list->add_item(1, "bruno3");
	obj_list->add_item(2, "cnoise");
	obj_list->add_item(3, "dipole");
	obj_list->add_item(4, "vnoise");

	spinner = Glui->add_spinner("Distance:", GLUI_SPINNER_INT, &d_test, -2, (GLUI_Update_CB)Choose_Object);

	spinner->set_int_limits(1, 50, GLUI_LIMIT_CLAMP);


	panel = Glui->add_panel("Object Transformation");

	rot = Glui->add_rotation_to_panel(panel, "Rotation", (float*)RotMatrix);

	// allow the object to be spun via the glui rotation widget:

	rot->set_spin(1.0);


	Glui->add_column_to_panel(panel, GLUIFALSE);
	scale = Glui->add_translation_to_panel(panel, "Scale", GLUI_TRANSLATION_Y, &Scale2);
	scale->set_speed(0.005f);

	Glui->add_column_to_panel(panel, GLUIFALSE);
	trans = Glui->add_translation_to_panel(panel, "Trans XY", GLUI_TRANSLATION_XY, &TransXYZ[0]);
	trans->set_speed(0.05f);

	Glui->add_column_to_panel(panel, GLUIFALSE);
	trans = Glui->add_translation_to_panel(panel, "Trans Z", GLUI_TRANSLATION_Z, &TransXYZ[2]);
	trans->set_speed(0.05f);



	Glui->add_checkbox("Arrow Head", &ArrowOn);

	//Glui->add_checkbox("Blend with Magnitude", &BlendOn);

	Glui->add_checkbox("Debug", &DebugOn);


	panel = Glui->add_panel("", GLUIFALSE);

	Glui->add_button_to_panel(panel, "Reset", RESET, (GLUI_Update_CB)Buttons);

	Glui->add_column_to_panel(panel, GLUIFALSE);

	Glui->add_button_to_panel(panel, "Quit", QUIT, (GLUI_Update_CB)Buttons);


	// tell glui what graphics window it needs to post a redisplay to:

	Glui->set_main_gfx_window(MainWindow);


	// set the graphics window's idle function:

	GLUI_Master.set_glutIdleFunc(NULL);
}



//
// initialize the glut and OpenGL libraries:
//	also setup display lists and callback functions
//

void
InitGraphics(void)
{
	// setup the display mode:
	// ( *must* be done before call to glutCreateWindow() )
	// ask for color, double-buffering, and z-buffering:

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);


	// set the initial window configuration:

	glutInitWindowPosition(0, 0);
	glutInitWindowSize(INIT_WINDOW_SIZE, INIT_WINDOW_SIZE);


	// open the window and set its title:

	MainWindow = glutCreateWindow(WINDOWTITLE);
	glutSetWindowTitle(WINDOWTITLE);


	// setup the clear values:

	glClearColor(BACKCOLOR[0], BACKCOLOR[1], BACKCOLOR[2], BACKCOLOR[3]);


	glutSetWindow(MainWindow);
	glutDisplayFunc(Display);
	//glutDisplayFunc( Display_Model );
	glutReshapeFunc(Resize);
	glutKeyboardFunc(Keyboard);
	glutMouseFunc(MouseButton);
	glutMotionFunc(MouseMotion);
	glutPassiveMotionFunc(NULL);
	glutVisibilityFunc(Visibility);
	glutEntryFunc(NULL);
	glutSpecialFunc(NULL);
	glutSpaceballMotionFunc(NULL);
	glutSpaceballRotateFunc(NULL);
	glutSpaceballButtonFunc(NULL);
	glutButtonBoxFunc(NULL);
	glutDialsFunc(NULL);
	glutTabletMotionFunc(NULL);
	glutTabletButtonFunc(NULL);
	glutMenuStateFunc(NULL);
	glutTimerFunc(0, NULL, 0);

	// DO NOT SET THE GLUT IDLE FUNCTION HERE !!
	// glutIdleFunc( NULL );
	// let glui take care of it in InitGlui()
}




//
// initialize the display lists that will not change:
//

void
InitLists(void)
{
	float dx = BOXSIZE / 2.;
	float dy = BOXSIZE / 2.;
	float dz = BOXSIZE / 2.;

	// create the object:

	BoxList = glGenLists(1);
	glNewList(BoxList, GL_COMPILE);

	glBegin(GL_QUADS);

	glColor3f(0., 0., 1.);
	glNormal3f(0., 0., 1.);
	glVertex3f(-dx, -dy, dz);
	glVertex3f(dx, -dy, dz);
	glVertex3f(dx, dy, dz);
	glVertex3f(-dx, dy, dz);

	glNormal3f(0., 0., -1.);
	glTexCoord2f(0., 0.);
	glVertex3f(-dx, -dy, -dz);
	glTexCoord2f(0., 1.);
	glVertex3f(-dx, dy, -dz);
	glTexCoord2f(1., 1.);
	glVertex3f(dx, dy, -dz);
	glTexCoord2f(1., 0.);
	glVertex3f(dx, -dy, -dz);

	glColor3f(1., 0., 0.);
	glNormal3f(1., 0., 0.);
	glVertex3f(dx, -dy, dz);
	glVertex3f(dx, -dy, -dz);
	glVertex3f(dx, dy, -dz);
	glVertex3f(dx, dy, dz);

	glNormal3f(-1., 0., 0.);
	glVertex3f(-dx, -dy, dz);
	glVertex3f(-dx, dy, dz);
	glVertex3f(-dx, dy, -dz);
	glVertex3f(-dx, -dy, -dz);

	glColor3f(0., 1., 0.);
	glNormal3f(0., 1., 0.);
	glVertex3f(-dx, dy, dz);
	glVertex3f(dx, dy, dz);
	glVertex3f(dx, dy, -dz);
	glVertex3f(-dx, dy, -dz);

	glNormal3f(0., -1., 0.);
	glVertex3f(-dx, -dy, dz);
	glVertex3f(-dx, -dy, -dz);
	glVertex3f(dx, -dy, -dz);
	glVertex3f(dx, -dy, dz);

	glEnd();

	glEndList();


	// create the axes:

	AxesList = glGenLists(1);
	glNewList(AxesList, GL_COMPILE);
	glLineWidth(AXES_WIDTH);
	Axes(1.5);
	glLineWidth(1.);
	glEndList();
}



//
// the keyboard callback:
//

void
Keyboard(unsigned char c, int x, int y)
{
	if (DebugOn != 0)
		fprintf(stderr, "Keyboard: '%c' (0x%0x)\n", c, c);

	switch (c)
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
		Buttons(QUIT);	// will not return here
		break;			// happy compiler

	case 'r':
	case 'R':
		LeftButton = ROTATE;
		break;

	case 's':
	case 'S':
		LeftButton = SCALE;
		break;

	default:
		fprintf(stderr, "Don't know what to do with keyboard hit: '%c' (0x%0x)\n", c, c);
	}


	// synchronize the GLUI display with the variables:

	Glui->sync_live();


	// force a call to Display():

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}



//
// called when the mouse button transitions down or up:
//

void
MouseButton(int button, int state, int x, int y)
{
	int b;			// LEFT, MIDDLE, or RIGHT

	if (DebugOn != 0)
		fprintf(stderr, "MouseButton: %d, %d, %d, %d\n", button, state, x, y);


	// get the proper button bit mask:

	switch (button)
	{
	case GLUT_LEFT_BUTTON:
		b = LEFT;		break;

	case GLUT_MIDDLE_BUTTON:
		b = MIDDLE;		break;

	case GLUT_RIGHT_BUTTON:
		b = RIGHT;		break;

	default:
		b = 0;
		fprintf(stderr, "Unknown mouse button: %d\n", button);
	}


	// button down sets the bit, up clears the bit:

	if (state == GLUT_DOWN)
	{
		Xmouse = x;
		Ymouse = y;
		ActiveButton |= b;		// set the proper bit
	}
	else
	{
		ActiveButton &= ~b;		// clear the proper bit
	}
}



//
// called when the mouse moves while a button is down:
//

void
MouseMotion(int x, int y)
{
	int dx, dy;		// change in mouse coordinates

	if (DebugOn != 0)
		fprintf(stderr, "MouseMotion: %d, %d\n", x, y);


	dx = x - Xmouse;		// change in mouse coords
	dy = y - Ymouse;

	if ((ActiveButton & LEFT) != 0)
	{
		switch (LeftButton)
		{
		case ROTATE:
			Xrot += (ANGFACT * dy);
			Yrot += (ANGFACT * dx);
			break;

		case SCALE:
			Scale += SCLFACT * (float)(dx - dy);
			if (Scale < MINSCALE)
				Scale = MINSCALE;
			break;
		}
	}


	if ((ActiveButton & MIDDLE) != 0)
	{
		Scale += SCLFACT * (float)(dx - dy);

		// keep object from turning inside-out or disappearing:

		if (Scale < MINSCALE)
			Scale = MINSCALE;
	}

	Xmouse = x;			// new current position
	Ymouse = y;

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}



//
// reset the transformations and the colors:
//
// this only sets the global variables --
// the glut main loop is responsible for redrawing the scene
//

void
Reset(void)
{
	ActiveButton = 0;
	AxesOn = GLUITRUE;
	DebugOn = GLUIFALSE;
	DiscreteOn = GLUIFALSE;
	DepthCueOn = GLUIFALSE;
	LeftButton = ROTATE;
	Scale = 1.0;
	Scale2 = 0.0;		// because we add 1. to it in Display()
	WhichColor = WHITE;
	WhichProjection = PERSP;
	Xrot = Yrot = 0.;
	TransXYZ[0] = TransXYZ[1] = TransXYZ[2] = 0.;

	RotMatrix[0][1] = RotMatrix[0][2] = RotMatrix[0][3] = 0.;
	RotMatrix[1][0] = RotMatrix[1][2] = RotMatrix[1][3] = 0.;
	RotMatrix[2][0] = RotMatrix[2][1] = RotMatrix[2][3] = 0.;
	RotMatrix[3][0] = RotMatrix[3][1] = RotMatrix[3][3] = 0.;
	RotMatrix[0][0] = RotMatrix[1][1] = RotMatrix[2][2] = RotMatrix[3][3] = 1.;
}



//
// called when user resizes the window:
//

void
Resize(int width, int height)
{
	if (DebugOn != 0)
		fprintf(stderr, "ReSize: %d, %d\n", width, height);

	// don't really need to do anything since window size is
	// checked each time in Display():

	glutSetWindow(MainWindow);
	glutPostRedisplay();
}


//
// handle a change to the window's visibility:
//

void
Visibility(int state)
{
	if (DebugOn != 0)
		fprintf(stderr, "Visibility: %d\n", state);

	if (state == GLUT_VISIBLE)
	{
		glutSetWindow(MainWindow);
		glutPostRedisplay();
	}
	else
	{
		// could optimize by keeping track of the fact
		// that the window is not visible and avoid
		// animating or redrawing it ...
	}
}




//////////////////////////////////////////  EXTRA HANDY UTILITIES:  /////////////////////////////

// size of wings as fraction of length:

#define WINGS	0.10


// axes:

#define X	1
#define Y	2
#define Z	3


// x, y, z, axes:

static float axx[3] = { 1., 0., 0. };
static float ayy[3] = { 0., 1., 0. };
static float azz[3] = { 0., 0., 1. };


void
Arrow(float tail[3], float head[3])
{
	float u[3], v[3], w[3];		// arrow coordinate system
	float d;			// wing distance
	float x, y, z;			// point to plot
	float mag;			// magnitude of major direction
	float f;			// fabs of magnitude
	int axis;			// which axis is the major


	// set w direction in u-v-w coordinate system:

	w[0] = head[0] - tail[0];
	w[1] = head[1] - tail[1];
	w[2] = head[2] - tail[2];


	// determine major direction:

	axis = X;
	mag = fabs(w[0]);
	if ((f = fabs(w[1])) > mag)
	{
		axis = Y;
		mag = f;
	}
	if ((f = fabs(w[2])) > mag)
	{
		axis = Z;
		mag = f;
	}


	// set size of wings and turn w into a Unit vector:

	d = WINGS * Unit(w, w);


	// draw the shaft of the arrow:

	glBegin(GL_LINE_STRIP);
	glVertex3fv(tail);
	glVertex3fv(head);
	glEnd();

	// draw two sets of wings in the non-major directions:

	if (axis != X)
	{
		Cross(w, axx, v);
		(void)Unit(v, v);
		Cross(v, w, u);
		x = head[0] + d * (u[0] - w[0]);
		y = head[1] + d * (u[1] - w[1]);
		z = head[2] + d * (u[2] - w[2]);
		glBegin(GL_LINE_STRIP);
		glVertex3fv(head);
		glVertex3f(x, y, z);
		glEnd();
		x = head[0] + d * (-u[0] - w[0]);
		y = head[1] + d * (-u[1] - w[1]);
		z = head[2] + d * (-u[2] - w[2]);
		glBegin(GL_LINE_STRIP);
		glVertex3fv(head);
		glVertex3f(x, y, z);
		glEnd();
	}


	if (axis != Y)
	{
		Cross(w, ayy, v);
		(void)Unit(v, v);
		Cross(v, w, u);
		x = head[0] + d * (u[0] - w[0]);
		y = head[1] + d * (u[1] - w[1]);
		z = head[2] + d * (u[2] - w[2]);
		glBegin(GL_LINE_STRIP);
		glVertex3fv(head);
		glVertex3f(x, y, z);
		glEnd();
		x = head[0] + d * (-u[0] - w[0]);
		y = head[1] + d * (-u[1] - w[1]);
		z = head[2] + d * (-u[2] - w[2]);
		glBegin(GL_LINE_STRIP);
		glVertex3fv(head);
		glVertex3f(x, y, z);
		glEnd();
	}



	if (axis != Z)
	{
		Cross(w, azz, v);
		(void)Unit(v, v);
		Cross(v, w, u);
		x = head[0] + d * (u[0] - w[0]);
		y = head[1] + d * (u[1] - w[1]);
		z = head[2] + d * (u[2] - w[2]);
		glBegin(GL_LINE_STRIP);
		glVertex3fv(head);
		glVertex3f(x, y, z);
		glEnd();
		x = head[0] + d * (-u[0] - w[0]);
		y = head[1] + d * (-u[1] - w[1]);
		z = head[2] + d * (-u[2] - w[2]);
		glBegin(GL_LINE_STRIP);
		glVertex3fv(head);
		glVertex3f(x, y, z);
		glEnd();
	}
}



float
Dot(float v1[3], float v2[3])
{
	return v1[0] * v2[0] + v1[1] * v2[1] + v1[2] * v2[2];
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
Unit(float vin[3], float vout[3])
{
	float dist, f;

	dist = vin[0] * vin[0] + vin[1] * vin[1] + vin[2] * vin[2];

	if (dist > 0.0)
	{
		dist = sqrt(dist);
		f = 1. / dist;
		vout[0] = f * vin[0];
		vout[1] = f * vin[1];
		vout[2] = f * vin[2];
	}
	else
	{
		vout[0] = vin[0];
		vout[1] = vin[1];
		vout[2] = vin[2];
	}

	return dist;
}



// the stroke characters 'X' 'Y' 'Z' :

static float xx[] = {
		0.f, 1.f, 0.f, 1.f
};

static float xy[] = {
		-.5f, .5f, .5f, -.5f
};

static int xorder[] = {
		1, 2, -3, 4
};


static float yx[] = {
		0.f, 0.f, -.5f, .5f
};

static float yy[] = {
		0.f, .6f, 1.f, 1.f
};

static int yorder[] = {
		1, 2, 3, -2, 4
};


static float zx[] = {
		1.f, 0.f, 1.f, 0.f, .25f, .75f
};

static float zy[] = {
		.5f, .5f, -.5f, -.5f, 0.f, 0.f
};

static int zorder[] = {
		1, 2, 3, 4, -5, 6
};


// fraction of the length to use as height of the characters:

const float LENFRAC = 0.10f;


// fraction of length to use as start location of the characters:

const float BASEFRAC = 1.10f;


//
//	Draw a set of 3D axes:
//	(length is the axis length in world coordinates)
//

void
Axes(float length)
{
	int i, j;			// counters
	float fact;			// character scale factor
	float base;			// character start location


	glBegin(GL_LINE_STRIP);
	glVertex3f(length, 0., 0.);
	glVertex3f(0., 0., 0.);
	glVertex3f(0., length, 0.);
	glEnd();
	glBegin(GL_LINE_STRIP);
	glVertex3f(0., 0., 0.);
	glVertex3f(0., 0., length);
	glEnd();

	fact = LENFRAC * length;
	base = BASEFRAC * length;

	glBegin(GL_LINE_STRIP);
	for (i = 0; i < 4; i++)
	{
		j = xorder[i];
		if (j < 0)
		{

			glEnd();
			glBegin(GL_LINE_STRIP);
			j = -j;
		}
		j--;
		glVertex3f(base + fact * xx[j], fact * xy[j], 0.0);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (i = 0; i < 5; i++)
	{
		j = yorder[i];
		if (j < 0)
		{

			glEnd();
			glBegin(GL_LINE_STRIP);
			j = -j;
		}
		j--;
		glVertex3f(fact * yx[j], base + fact * yy[j], 0.0);
	}
	glEnd();

	glBegin(GL_LINE_STRIP);
	for (i = 0; i < 6; i++)
	{
		j = zorder[i];
		if (j < 0)
		{

			glEnd();
			glBegin(GL_LINE_STRIP);
			j = -j;
		}
		j--;
		glVertex3f(0.0, fact * zy[j], base + fact * zx[j]);
	}
	glEnd();

}




//
// routine to convert HSV to RGB
//
// Reference:  Foley, van Dam, Feiner, Hughes,
//		"Computer Graphics Principles and Practices,"
//		Additon-Wesley, 1990, pp592-593.


void HsvRgb(float hsv[3], float rgb[3])
{
	float h, s, v;			// hue, sat, value
	float r = 1., g = 1., b = 1.;			// red, green, blue
	float i, f, p, q, t;		// interim values


	// guarantee valid input:

	h = hsv[0] / 60.;
	while (h >= 6.)	h -= 6.;
	while (h < 0.) 	h += 6.;

	s = hsv[1];
	if (s < 0.)
		s = 0.;
	if (s > 1.)
		s = 1.;

	v = hsv[2];
	if (v < 0.)
		v = 0.;
	if (v > 1.)
		v = 1.;


	// if sat==0, then is a gray:

	if (s == 0.0)
	{
		rgb[0] = rgb[1] = rgb[2] = v;
		return;
	}


	// get an rgb from the hue itself:

	i = floor(h);
	f = h - i;
	p = v * (1. - s);
	q = v * (1. - s * f);
	t = v * (1. - (s * (1. - f)));

	switch ((int)i)
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

// Set the view and the lighting properties
//

void set_view(GLenum mode, Polyhedron* poly)
{
	icVector3 up, ray, view;
	GLfloat light_ambient0[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat light_diffuse0[] = { 0.7, 0.7, 0.7, 1.0 };
	GLfloat light_specular0[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_ambient1[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_diffuse1[] = { 0.5, 0.5, 0.5, 1.0 };
	GLfloat light_specular1[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat light_ambient2[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_diffuse2[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular2[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_position[] = { 0.0, 0.0, 0.0, 1.0 };

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular0);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular1);


	glMatrixMode(GL_PROJECTION);
	if (mode == GL_RENDER)
		glLoadIdentity();

	if (WhichProjection == ORTHO)
		glOrtho(-radius_factor, radius_factor, -radius_factor, radius_factor, 0.0, 40.0);
	else
		gluPerspective(45.0, 1.0, 0.1, 40.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	light_position[0] = 5.5;
	light_position[1] = 0.0;
	light_position[2] = 0.0;
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	light_position[0] = -0.1;
	light_position[1] = 0.0;
	light_position[2] = 0.0;
	glLightfv(GL_LIGHT2, GL_POSITION, light_position);
}

//
// Set the scene for the object based on the center of the object

void set_scene(GLenum mode, Polyhedron* poly)
{
	glTranslatef(0.0, 0.0, -3.0);

	glScalef(1.0 / poly->radius, 1.0 / poly->radius, 1.0 / poly->radius);
	glTranslatef(-poly->center.entry[0], -poly->center.entry[1], -poly->center.entry[2]);
}

//
// OpenGL rendering of the object

void display_shape(GLenum mode, Polyhedron* this_poly)
{

}


void Display_Model(void) {
	GLint viewport[4];
	int jitter;

	glClearColor(1.0, 1.0, 1.0, 1.0);  // background for rendering color coding and lighting
	glGetIntegerv(GL_VIEWPORT, viewport);

	set_view(GL_RENDER, poly);
	set_scene(GL_RENDER, poly);
	display_shape(GL_RENDER, poly);
	glFlush();
	glutSwapBuffers();
	glFinish();
}


void Choose_Object() {
	int w, h;
	switch (ObjectId) {
	case BNOISE:
		strcpy(object_name, "bnoise");
		break;

	case BRUNO3:
		strcpy(object_name, "bruno3");
		break;

	case CNOISE:
		strcpy(object_name, "cnoise");
		break;

	case DIPOLE:
		strcpy(object_name, "dipole");
		break;

	case VNOISE:
		strcpy(object_name, "vnoise");
		break;

	}

	poly->finalize();

	Reset();

	char tmp_str[512];

	sprintf(tmp_str, "../models/%s.ply", object_name);

	FILE* this_file = fopen(tmp_str, "r");
	poly = new Polyhedron(this_file);
	fclose(this_file);
	////Following codes build the edge information
	clock_t start, finish; //Used to show the time assumed
	start = clock(); //Get the first time

	poly->initialize(); // initialize everything

	poly->calc_bounding_sphere();
	poly->calc_face_normals_and_area();
	poly->average_normals();

	/*****************************************************************/
	render_vec_img(poly);
	XLength = YLength = sqrt(poly->nverts);
	StreamLines1.clear();
	StreamLines.clear();
	for (int i1 = 0; i1 < 512; i1++)
	{
		for (int j1 = 0; j1 < 512; j1++)
		{
			all_points[i1][j1] = 0;
		}
	}
	init_i = rand() % 256;
	init_j = rand() % 256;
	stream_count = 0;
	CalculateFirstStreamLine();
	get_forward_seedpoints();
	//compute_lic();
	get_backward_seedpoints();
	compute_lic();
	//CalculateFirstStreamLine();
	int xi = init_i;
	int yi = init_j;
	for (int xi = 0; xi < 11; xi++)
	{
		float x = init_i;
		float y = init_j;
		for (int c = 0; c < 171; c++)
		{

			//StreamLines.clear();
			init_i = init_i;
			init_j = init_j + (3 * d_test);
			CalculateFirstStreamLine();
			get_forward_seedpoints();
			//compute_lic();
			get_backward_seedpoints();
			compute_lic();
		}
		init_i = x;
		init_j = y;
		for (int c = 0; c < 171; c++)
		{
			//StreamLines.clear();
			init_i = init_i;
			init_j = init_j - (3 * d_test);

			CalculateFirstStreamLine();
			get_forward_seedpoints();
			//compute_lic();
			get_backward_seedpoints();
			compute_lic();
		}

		init_i = x + 50;
		init_j = y;
	}
	init_i = xi;
	init_j = yi;
	for (int xi = 0; xi < 11; xi++)
	{
		float x = init_i;
		float y = init_j;
		for (int c = 0; c < 500; c++)
		{

			//StreamLines.clear();
			init_i = init_i;
			init_j = init_j + (3 * d_test);
			CalculateFirstStreamLine();
			get_forward_seedpoints();
			//compute_lic();
			get_backward_seedpoints();
			compute_lic();
		}
		init_i = x;
		init_j = y;
		for (int c = 0; c < 171; c++)
		{
			//StreamLines.clear();
			init_i = init_i;
			init_j = init_j - (3 * d_test);

			CalculateFirstStreamLine();
			get_forward_seedpoints();
			//compute_lic();
			get_backward_seedpoints();
			compute_lic();
		}

		init_i = x - 50;
		init_j = y;
	}

	/*get_forward_seedpoints();
	compute_lic();
	get_backward_seedpoints();
	compute_lic();*/
	/*****************************************************************/

	finish = clock(); //Get the current time after finished
	double t = (double)(finish - start) / CLOCKS_PER_SEC;

	/*printf("\n");
	printf("The number of the edges of the object %s is %d \n", object_name, poly->nedges);
	printf("The Euler Characteristics of the object %s is %d \n", object_name, (poly->nverts - poly->nedges + poly->ntris));

	printf("Time to building the edge link is %f seconds\n", t);*/

	Glui->sync_live();
	glutSetWindow(MainWindow);
	glutPostRedisplay();
}
void Choose_Object_Color() {
	int w, h;
	switch (ObjectColorId) {
	case RAINBOW:
		color = 0;
		display_shape(GL_RENDER, poly);
		break;

	case BLUE_WHITE_RED:
		color = 1;
		display_shape(GL_RENDER, poly);
		break;

	case HEAT_MAP:
		color = 2;
		display_shape(GL_RENDER, poly);
		break;

	case NON_LINEAR:
		color = 3;
		display_shape(GL_RENDER, poly);
		break;
	}
}


void get_s_min_and_s_max(Polyhedron* this_poly)
{
	float x, y, z;
	if (VectorId == 0)
	{
		for (int i = 0; i < this_poly->nverts; i++)
		{
			x = (this_poly->vlist[i]->vx);
			y = (this_poly->vlist[i]->vy);
			z = (this_poly->vlist[i]->vz);
			float g = sqrt(((x * x) + (y * y) + (z * z)));
			this_poly->vlist[i]->s = g;
			this_poly->vlist[i]->magni = g;
		}
	}
	else if (VectorId == 1)
	{
		for (int i = 0; i < this_poly->nverts; i++)
		{
			x = this_poly->vlist[i]->vx;
			y = this_poly->vlist[i]->vy;
			this_poly->vlist[i]->s = atan(y / x);
		}
	}
	else if (VectorId == 2)
	{
		for (int i = 0; i < this_poly->nverts; i++)
		{
			x = this_poly->vlist[i]->vx;
			this_poly->vlist[i]->s = x;
		}
	}
	else if (VectorId == 3)
	{
		for (int i = 0; i < this_poly->nverts; i++)
		{
			y = this_poly->vlist[i]->vy;
			this_poly->vlist[i]->s = y;
		}
	}
	S_MIN = this_poly->vlist[0]->s;
	S_MAX = this_poly->vlist[0]->s;
	for (int i = 0; i < this_poly->nverts; i++)
	{
		if (S_MIN > this_poly->vlist[i]->s) {
			S_MIN = this_poly->vlist[i]->s;
		}
		if (S_MAX < this_poly->vlist[i]->s) {
			S_MAX = this_poly->vlist[i]->s;
		}
	}
}

void max_and_min(Polyhedron* this_poly) {
	for (int i = 0; i < this_poly->nverts; i++)
	{
		if (min_vx > this_poly->vlist[i]->vx) {
			min_vx = this_poly->vlist[i]->vx;
		}
		if (max_vx < this_poly->vlist[i]->vx) {
			max_vx = this_poly->vlist[i]->vx;
		}
		if (min_vy > this_poly->vlist[i]->vy) {
			min_vy = this_poly->vlist[i]->vy;
		}
		if (max_vy < this_poly->vlist[i]->vy) {
			max_vy = this_poly->vlist[i]->vy;
		}
	}
}

void draw_arrow_head(double head[2], float direct[2]) {
	glPushMatrix();
	glTranslatef(head[0], head[1], 0.005);
	glRotatef(atan2(direct[1], direct[0]) * 360 / (2 * M_PI), 0, 0, 1);
	glScalef(0.04, 0.04, 1);
	glColor3f(0., 0., 1.);
	glBegin(GL_TRIANGLES);
	glVertex2f(0, 0);
	glVertex2f(-0.35, 0.12);
	glVertex2f(-0.35, -0.12);
	glEnd();
	glBegin(GL_LINES);
	glVertex2f(-0.35, 0.);
	glVertex2f(-0.65, 0.);
	glEnd();
	glPopMatrix();
}

void render_vec_img(Polyhedron* this_poly) {
	glViewport(0, 0, (GLsizei)512, (GLsizei)512);
	glMatrixMode(GL_PROJECTION);
	//glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glLoadIdentity();
	gluOrtho2D(0, 1, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawBuffer(GL_BACK);
	//glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	int i, j;
	// first search the max_vx, min_vx, max_vy, min_vy through the entire field
		// render the mesh
	max_and_min(poly);
	for (i = 0; i < this_poly->ntris; i++) {
		float rgb[3];
		rgb[2] = 0.5;
		Triangle* temp_t = this_poly->tlist[i];
		glBegin(GL_TRIANGLES);
		for (j = 0; j < 3; j++) {

			Vertex* temp_v = temp_t->verts[j];
			//determine the color for this vertex based on its vector value
			rgb[0] = (temp_v->vx - min_vx) / (max_vx - min_vx); // red channel
			rgb[1] = (temp_v->vy - min_vy) / (max_vy - min_vy); // green channel
			rgb[2] = 0.;
			glColor3fv(rgb);
			//glNormal3d(temp_v->normal.entry[0], temp_v->normal.entry[1], temp_v->normal.entry[2]);
			glVertex2d(temp_v->x, temp_v->y);

		}
		glEnd();
	}
	// save the rendered image into the vec_img
	glReadBuffer(GL_BACK);
	glReadPixels(0, 0, 512, 512, GL_RGB, GL_UNSIGNED_BYTE, vec_img);

}

void CalculateFirstStreamLine()
{
	float tvx, tvy;
	float sumr = 0., sumg = 0.;

	int i = init_i, j = init_j;

	//StreamLines.clear();

	float y = i + .5;
	float x = j + .5;
	int next_i = i;
	int next_j = j;
	int count = 0;
	sumr = 0.;
	if ((next_i < 512) && (next_j < 512) && (next_i >= 0) && (next_j >= 0))
	{
		vector <seedPoint> temp_sl;
		while (count < 1000) {
			tvx = min_vx + (max_vx - min_vx) * vec_img[next_i][next_j][0] / 255.0;
			tvy = min_vy + (max_vy - min_vy) * vec_img[next_i][next_j][1] / 255.0;
			float mag = sqrt((tvx * tvx) + (tvy * tvy));

			if (mag < 1.e-6)
				break;

			seedPoint temp;
			temp.ii = next_i;
			temp.jj = next_j;

			tvx /= mag;
			tvy /= mag;
			x = x + tvx;
			y = y + tvy;
			next_i = int(y);
			next_j = int(x);


			temp.x = x;
			temp.y = y;

			if ((next_i < 0) || (next_j < 0) || (next_i >= 512) || (next_j >= 512))
			{
				break;
			}
			else
			{
				//StreamLines[strm_cnt].push_back(temp);
				int flag = 0;
				for (int m = 0; m < d_test; m++) {

					if ((all_points[next_i][next_j + m] == 1) || (all_points[next_i][next_j - m] == 1) || (all_points[next_i + m][next_j] == 1) || (all_points[next_i - m][next_j] == 1) ||
						(all_points[next_i + m][next_j + m] == 1) || (all_points[next_i - m][next_j + m] == 1) || (all_points[next_i + m][next_j - m] == 1) || (all_points[next_i - m][next_j - m] == 1)) {
						flag = 1;
						break;
					}
				}
				if (flag == 1)
					break;
				else{
					temp_sl.push_back(temp);
				}
			}//temp_sl.insert(temp_sl.begin(), temp);
			count++;
		}

		y = i + .5;
		x = j + .5;
		next_i = i;
		next_j = j;
		count = 0;
		while (count < 1000) {
			tvx = min_vx + (max_vx - min_vx) * vec_img[next_i][next_j][0] / 255.0;
			tvy = min_vy + (max_vy - min_vy) * vec_img[next_i][next_j][1] / 255.0;
			float mag = sqrt((tvx * tvx) + (tvy * tvy));
			if (mag < 1.e-6)
				break;
			seedPoint temp;
			temp.ii = next_i;
			temp.jj = next_j;

			tvx /= mag;
			tvy /= mag;
			x = x - tvx;
			y = y - tvy;

			temp.x = x;
			temp.y = y;

			next_i = int(y);
			next_j = int(x);
			if ((next_i < 0) || (next_j < 0) || (next_i >= 512) || (next_j >= 512)) {
				break;
			}
			else
			{//temp_sl.push_back(temp);
				//StreamLines[strm_cnt].push_back(temp);
				int flag = 0;
				for (int m = 0; m < d_test; m++) {

					if ((all_points[next_i][next_j + m] == 1) || (all_points[next_i][next_j - m] == 1) || (all_points[next_i + m][next_j] == 1) || (all_points[next_i - m][next_j] == 1) ||
						(all_points[next_i + m][next_j + m] == 1) || (all_points[next_i - m][next_j + m] == 1) || (all_points[next_i + m][next_j - m] == 1) || (all_points[next_i - m][next_j - m] == 1)) {

						flag = 1;
						break;
					}
				}
				if (flag == 1)
					break;
				else
				{
					temp_sl.insert(temp_sl.begin(), temp);
				}

			}
			count++;
		}
		StreamLines1.push_back(temp_sl);
		for (int m = 0; m < temp_sl.size(); m++) {
			
			all_points[(int)temp_sl[m].y][(int)temp_sl[m].x] = 1;
		}
		StreamLines.push_back(temp_sl);
		//cout << StreamLines[stream_count].size() << "\n";
		/*
		for (int i = 0; i < temp_sl.size(); i++) {
			printf("%f\t%f\n", temp_sl[i].x, temp_sl[i].y);
		}


		printf("%d\n", StreamLines.size());
		for (int j = 0; j < StreamLines.size(); j++) {
			//printf("%d\n", StreamLines[j].size());
			for (int i = 0; i < StreamLines[j].size();i++) {
				printf("%d\t%d\n", StreamLines[j][i].ii, StreamLines[j][i].jj);
			}
		}
		*/
	}
}

void compute_lic() {
	float tvx, tvy;
	float sumr = 0., sumg = 0.;
	int strm_cnt = 0;
	//StreamLines1.clear();

	//int i = 200, j = 300;
	for (int i = 0; i < SeedPoints.size(); i++) {
		//for (int j = 0; j < 512; j+=20)	{


		float y = SeedPoints[i].x;
		float x = SeedPoints[i].y;
		int next_i = SeedPoints[i].x;
		int next_j = SeedPoints[i].y;
		int count = 0;
		sumr = 0.;
		if ((next_i >= 0) && (next_j < 512) && (next_i < 512) && (next_j >= 0))
		{
			vector <seedPoint> temp_sl;
			while (count < 1000)
			{
				tvx = min_vx + (max_vx - min_vx) * vec_img[next_i][next_j][0] / 255.0;
				tvy = min_vy + (max_vy - min_vy) * vec_img[next_i][next_j][1] / 255.0;
				float mag = sqrt((tvx * tvx) + (tvy * tvy));
				if (mag < 1.e-6)
					break;

				seedPoint temp;
				temp.ii = next_i;
				temp.jj = next_j;

				tvx /= mag;
				tvy /= mag;
				x = x + tvx;
				y = y + tvy;
				next_i = int(y);
				next_j = int(x);


				temp.x = x;
				temp.y = y;

				if ((next_i < 0) || (next_j < 0) || (next_i >= 512) || (next_j >= 512))
				{
					break;
				}
				else
				{//StreamLines[strm_cnt].push_back(temp);
					int flag = 0;
					for (int m = 0; m < d_test; m++) {

						if ((all_points[next_i][next_j + m] == 1) || (all_points[next_i][next_j - m] == 1) || (all_points[next_i + m][next_j] == 1) || (all_points[next_i - m][next_j] == 1) ||
							(all_points[next_i + m][next_j + m] == 1) || (all_points[next_i - m][next_j + m] == 1) || (all_points[next_i + m][next_j - m] == 1) || (all_points[next_i - m][next_j - m] == 1)) {

							flag = 1;
							break;
						}
					}
					if (flag == 1)
						break;
					else
					{
						temp_sl.push_back(temp);
					}
				}
				count++;
			}

			strm_cnt++;
			y = SeedPoints[i].x;
			x = SeedPoints[i].y;
			next_i = SeedPoints[i].x;
			next_j = SeedPoints[i].y;
			count = 0;

			while (count < 1000) {
				tvx = min_vx + (max_vx - min_vx) * vec_img[next_i][next_j][0] / 255.0;
				tvy = min_vy + (max_vy - min_vy) * vec_img[next_i][next_j][1] / 255.0;
				float mag = sqrt((tvx * tvx) + (tvy * tvy));
				if (mag < 1.e-6)
					break;
				seedPoint temp;

				temp.ii = next_i;
				temp.jj = next_j;

				tvx /= mag;
				tvy /= mag;

				x = x - tvx;
				y = y - tvy;

				temp.x = x;
				temp.y = y;

				next_i = int(y);
				next_j = int(x);
				if ((next_i < 0) || (next_j < 0) || (next_i >= 512) || (next_j >= 512)) {
					break;
				}
				else
				{//StreamLines[strm_cnt].push_back(temp);
					int flag = 0;
					for(int m =0; m < d_test; m++){
						
						if ((all_points[next_i][next_j + m] == 1) || (all_points[next_i][next_j - m] == 1) || (all_points[next_i + m][next_j] == 1) || (all_points[next_i - m][next_j] == 1) ||
							(all_points[next_i + m][next_j + m] == 1) || (all_points[next_i - m][next_j + m] == 1) || (all_points[next_i + m][next_j - m] == 1) || (all_points[next_i - m][next_j - m] == 1)) {
							
							flag = 1;
							break;
						}
}
					if (flag == 1)
						break;
					else
					{
						temp_sl.insert(temp_sl.begin(), temp);
					}
				}

				count++;

			}
			
		}
		//}
	}
}


void disp_streamlines() {
	//printf("StreamLines vec: %d\n", StreamLines.size());

	for (int i = 0; i < StreamLines.size(); i++) {
		//for (auto i = StreamLines[j].begin(); i < StreamLines[j].end(); i++) {
		//printf("StreamLines sub vec: %d\n", StreamLines[i].size());
		glColor3f(1., 1., 1.);
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < StreamLines[i].size(); j++) {
			glVertex2f(StreamLines[i][j].x / 512, StreamLines[i][j].y / 512);
			//printf("%f\t%f\n", StreamLines[i][j].x, StreamLines[i][j].y);
		}
		glEnd();

		/*glColor3f(0.5, 1., 0.5);
		for (int j = 0; j < StreamLines[i].size(); j += 10) {
			glBegin(GL_LINE_STRIP);
			float y1 = (StreamLines[i][j].y / 512) - d_sep;
			float y2 = (StreamLines[i][j].y / 512) + d_sep;
			//printf("%f\t%f\n", StreamLines[i][j].y / 512 - 0.01, StreamLines[i][j].y / 512 + 0.01);
			glVertex2f(StreamLines[i][j].x / 512, y1);
			glVertex2f(StreamLines[i][j].x / 512, y2);
			glEnd();
		}*/


	}

	/*
	//cout << StreamLines1.size();
	//printf("StreamLines vec: %d\n", StreamLines1.size());
	for (int i = 0; i < StreamLines1.size(); i++) {
		int s = 0;
		//for (auto i = StreamLines[j].begin(); i < StreamLines[j].end(); i++) {
		//printf("StreamLines sub vec: %d\n", StreamLines1[i].size() );
		glColor3f(0.5, 0.5, 0.5);
		glBegin(GL_LINE_STRIP);
		for (int j = 0; j < StreamLines1[i].size(); j++) {
			//printf("j = %d\n", j);
			glVertex2f(StreamLines1[i][j].x / 512, StreamLines1[i][j].y / 512);
			//printf("%f\t%f\n", StreamLines1[i][j].x, StreamLines1[i][j].y);
			//cout << s;
		}
		//printf("**************************");
		glEnd();
		
	}
	*/


}


void get_forward_seedpoints() {
	int j = 0;
	SeedPoints.clear();
	//for ( j = 0; j < StreamLines[stream_count].size(); j = j + d_test) 
	//{
	seedPoint temp1;
	temp1.x = init_i;
	temp1.y = init_j + d_sep;
	temp1.ii = temp1.x + d_sep;
	temp1.jj = temp1.x + d_sep;
	SeedPoints.push_back(temp1);
	//printf("%f\t%f\t\t%f\t%f\t", StreamLines[0][j].x, StreamLines[0][j].y,temp1.x,temp1.y);
//}
}

void get_backward_seedpoints() {
	int j = 0;
	//SeedPoints.clear();

	//for (j = 0; j < StreamLines[stream_count].size(); j=j+d_test)
	//{
	seedPoint temp2;
	temp2.x = init_i;
	temp2.y = init_j - d_sep;
	temp2.ii = temp2.x + d_sep;
	temp2.jj = temp2.y + d_sep;
	SeedPoints.insert(SeedPoints.begin(), temp2);
	//printf("%f\t%f\t\t%f\t%f\n", StreamLines[0][j].ii, StreamLines[0][j].jj, temp2.x, temp2.y);

//}
}