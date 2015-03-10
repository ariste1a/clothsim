////////////////////////////////////////
// tester.h
////////////////////////////////////////

#ifndef CSE169_TESTER_H
#define CSE169_TESTER_H

#include "core.h"
#include "camera.h"
#include "cube.h"
#include "CSE169_skeleton\skin.h"
#include "CSE169_skeleton\Skeleton.h"
#include "CSE169_skeleton\animation.h"
#include "CSE169_skeleton\ParticleSystem.h"
#include "CSE169_skeleton\Parachute.h"
////////////////////////////////////////////////////////////////////////////////

class Tester {
public:
	Tester(int argc,char **argv);
	~Tester();

	void Update();
	void Reset();
	void Draw();

	void Quit();

	// Event handlers
	void Resize(int x,int y);
	void Keyboard(int key,int x,int y);
	void MouseButton(int btn,int state,int x,int y);
	void MouseMotion(int x,int y);
	void processSpecialKeys(int key, int x, int y);
	time_t start;
	time_t current;
	float time;

private:
	// Window management
	int WindowHandle;
	int WinX,WinY;

	// Input
	bool LeftDown,MiddleDown,RightDown;
	int MouseX,MouseY;

	// Components
	Camera Cam;
	SpinningCube Cube;
	ParticleSystem *cloth; 
	Vector3 wind; 
	Vector3 pos; 
	Matrix34 position; 
	Matrix34 rotation; 
	float rot; 
	float move; 
	Parachute para; 
};

////////////////////////////////////////////////////////////////////////////////

/*
The 'Tester' is a simple top level application class. It creates and manages a
window with the GLUT extension to OpenGL and it maintains a simple 3D scene
including a camera and some other components.
*/

#endif
