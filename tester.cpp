////////////////////////////////////////
// tester.cpp
////////////////////////////////////////

#include "tester.h"
#include <iostream>
#define WINDOWTITLE	"EL PSY CONGROO"

////////////////////////////////////////////////////////////////////////////////

static Tester *TESTER;

int main(int argc, char **argv) {
	glutInit(&argc, argv);
	TESTER = new Tester(argc,argv);
	glutMainLoop();	
	return 0;
}

////////////////////////////////////////////////////////////////////////////////

// These are really HACKS to ' glut call member functions instead of static functions
static void display()									{TESTER->Draw();}
static void idle()										{TESTER->Update();}
static void resize(int x,int y)							{TESTER->Resize(x,y);}
static void keyboard(unsigned char key,int x,int y)		{TESTER->Keyboard(key,x,y);}
static void keyboard2(int key,int x,int y) { TESTER->processSpecialKeys(key, x, y); }
static void mousebutton(int btn,int state,int x,int y)	{TESTER->MouseButton(btn,state,x,y);}
static void mousemotion(int x, int y)					{TESTER->MouseMotion(x,y);}

////////////////////////////////////////////////////////////////////////////////

Tester::Tester(int argc,char **argv) {
	WinX=640;
	WinY=480;
	LeftDown=MiddleDown=RightDown=false;
	MouseX=MouseY=0;

	cloth = new ParticleSystem(); 
	// Create the window
	glutInitDisplayMode( GLUT_RGBA| GLUT_DOUBLE | GLUT_DEPTH );
	glutInitWindowSize( WinX, WinY );
	glutInitWindowPosition( 0, 0 );
	WindowHandle = glutCreateWindow( WINDOWTITLE );
	glutSetWindowTitle( WINDOWTITLE );	
	glEnable(GL_LIGHTING);
	//glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_SMOOTH);
	//glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);

	GLfloat light0_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light0_diffuse[] = { 0.3, 0.3, 0.3, 1.0 };	
	GLfloat light0_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light0_position[] = { 0, 200, 100, 0.0 };
	GLfloat spot_direction[] = { 0.1, -1.0, 0.3 };
	GLfloat attenuation[] = { 1, 0, 0, 0 };

	glEnable(GL_LIGHT0);
	glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light0_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
	glLightfv(GL_LIGHT0, GL_SPOT_DIRECTION, spot_direction);
	glLightfv(GL_LIGHT0, GL_CONSTANT_ATTENUATION, attenuation);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light0_specular);
	GLfloat light1_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light1_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light1_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light1_position[] = { 0, 10, 3, 1.0 };	

	glEnable(GL_LIGHT1);
	glLightfv(GL_LIGHT1, GL_POSITION, light1_position);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light1_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light0_specular);
	//glLightfv(GL_LIGHT1, GL_CONSTANT_ATTENUATION, attenuation); 

	GLfloat light2_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light2_diffuse[] = { 0.5, 0.0, 0.5, 1.0 };
	GLfloat light2_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light2_position[] = { 1, 5, 20, 1.0 };

	glEnable(GL_LIGHT2);
	glLightfv(GL_LIGHT2, GL_POSITION, light2_position);
	glLightfv(GL_LIGHT2, GL_AMBIENT, light2_ambient);
	glLightfv(GL_LIGHT2, GL_DIFFUSE, light2_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light0_specular);
	//glLightfv(GL_LIGHT1, GL_CONSTANT_ATTENUATION, attenuation);


	glutSetWindow( WindowHandle );

	//ok works
	glEnable(GL_DEPTH_TEST);	

	// Background color
	glClearColor( 0., 0., 0., 1. );	

	// Callbacks
	glutDisplayFunc( display );
	glutIdleFunc( idle );
	glutKeyboardFunc( keyboard );
	glutSpecialFunc(keyboard2);
	glutMouseFunc( mousebutton );
	glutMotionFunc( mousemotion );
	glutPassiveMotionFunc( mousemotion );
	glutReshapeFunc( resize );

	// Initialize components

	Cam.SetAspect(float(WinX)/float(WinY));	
	/*
	if (this->skin.texcoords.size() >0)
	this->skin.textureLoader.LoadGLTextures(skin.texName.c_str());
	time = 0.0;
	*/ 
	Cube = *new SpinningCube();	
	glPointSize(3.0);
	wind = *new Vector3(); 
	pos = *new Vector3(); 
	rot = 0.1; 
	position = *new Matrix34(); 
	position.Identity(); 
	move = 0.1;
	start = clock(); 
	para = *new Parachute(); 
}
	
////////////////////////////////////////////////////////////////////////////////

Tester::~Tester() {
	glFinish();
	glutDestroyWindow(WindowHandle);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Update() {
	// Update the components in the world
	Cam.Update();
	Cube.Update();
	//cloth->update(0.01, wind); 
	cloth->update(1/120.0, wind);
	para.update(1 / 120.0, wind);
	// Tell glut to re-display the scene
	glutSetWindow(WindowHandle);
	glutPostRedisplay();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Reset() {
	Cam.Reset();
	Cam.SetAspect(float(WinX)/float(WinY));
	Cube.Reset();	
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Draw() {

	// Begin drawing scene
	glViewport(0, 0, WinX, WinY);
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//GLfloat cyan[] = { 0.f, .5f, .5f, 1.f };
	GLfloat white[] = { 0.8f, 0.8f, 0.8f, 1.0f };
	GLfloat shiny[] = { 50.f };

	glMaterialfv(GL_FRONT, GL_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_SHININESS, shiny);

	glDisable(GL_CULL_FACE);
	glNormal3f(0, 1, 0);
	glBegin(GL_QUADS);
		glVertex3f(-50, -10, -50);
		glVertex3f(50, -10, -50);
		glVertex3f(50, -10, 50);		
		glVertex3f(-50, -10, 50);			
	glEnd();
	glEnable(GL_CULL_FACE);
	// Draw components
	Cam.Draw();		// Sets up projection & viewing matrices
	//Cube.Draw();
	glLoadIdentity(); 
	cloth->draw();
	// Finish drawing scene
	glLoadIdentity();
	para.draw(); 
	glFinish();
	glutSwapBuffers();
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Quit() {
	glFinish();
	glutDestroyWindow(WindowHandle);
	exit(0);
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Resize(int x,int y) {
	WinX = x;
	WinY = y;
	Cam.SetAspect(float(WinX)/float(WinY));
}

////////////////////////////////////////////////////////////////////////////////

void Tester::Keyboard(int key,int x,int y) {
	switch(key) {
		case 0x1b:		// Escape
			Quit();
			break;  
		case 'r':
			Reset();
			break;		
		case 't':
			cloth->reset();			
			//wind.Zero();
			pos.Zero(); 
			break;
		case 'y':
			para.reset();
			break;
		case '1':
			wind += Vector3(0.1, 0, 0);
			std::cout << "WIND X IS NOW " << wind.x << std::endl;
			break;
		case '2':
			wind += Vector3(0, 0.1, 0);
			std::cout << "WIND Y IS NOW " << wind.y << std::endl;
			break;
		case '3':
			wind += Vector3(0, 0, 0.1);
			std::cout << "WIND Z IS NOW " << wind.z << std::endl;
			break;
		case '4':
			wind.Zero();
			std::cout << "The wind... is troubled today" << std::endl; 
			break; 
		case 'w':
			position.MakeTranslate(Vector3(0, move, 0));
			cloth->move(position); 
			break; 
		case 's':			
			position.MakeTranslate(Vector3(0, -move, 0));
			cloth->move(position);
			break;
		case 'a':			
			position.MakeTranslate(Vector3(-move, 0, 0));
			cloth->move(position);
			break;
		case 'd':			
			position.MakeTranslate(Vector3(move, 0, 0));
			cloth->move(position);
			break;
		case 'j':			
			position.MakeTranslate(Vector3(0, 0, -move));
			cloth->move(position);
			break;
		case 'k':			
			position.MakeTranslate(Vector3(0, 0, move));
			cloth->move(position);
			break;	
		case '.':
			rotation.MakeRotateX(rot); 
			cloth->rotate(rotation);
			break;
		case '/':
			rotation.MakeRotateX(-rot);
			cloth->rotate(rotation);
			break;
	}
}

////////SPECIAL KEYS///////////////////
void Tester::processSpecialKeys(int key, int x, int y)
{
	switch (key) {
		case GLUT_KEY_LEFT:
			rotation.MakeRotateY(rot); 
			cloth->rotate(rotation);			
			break;
		case GLUT_KEY_RIGHT:
			rotation.MakeRotateY(-rot);
			cloth->rotate(rotation);
			break;
		case GLUT_KEY_UP:
			rotation.MakeRotateZ(rot);
			cloth->rotate(rotation);
			break;
		case GLUT_KEY_DOWN:
			rotation.MakeRotateZ(-rot);
			cloth->rotate(rotation);
			break;

	}
}
////////////////////////////////////////////////////////////////////////////////

void Tester::MouseButton(int btn,int state,int x,int y) {
	if(btn==GLUT_LEFT_BUTTON) {
		LeftDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_MIDDLE_BUTTON) {
		MiddleDown = (state==GLUT_DOWN);
	}
	else if(btn==GLUT_RIGHT_BUTTON) {
		RightDown = (state==GLUT_DOWN);
	}
}

////////////////////////////////////////////////////////////////////////////////

void Tester::MouseMotion(int nx,int ny) {
	int dx = nx - MouseX;
	int dy = -(ny - MouseY);

	MouseX = nx;
	MouseY = ny;

	// Move camera
	// NOTE: this should really be part of Camera::Update()
	if(LeftDown) {
		const float rate=1.0f;
		Cam.SetAzimuth(Cam.GetAzimuth()+dx*rate);
		Cam.SetIncline(Cam.GetIncline()-dy*rate);
	}
	if(RightDown) {
		const float rate=0.01f;
		Cam.SetDistance(Cam.GetDistance()*(1.0f-dx*rate));
	}
}

////////////////////////////////////////////////////////////////////////////////
