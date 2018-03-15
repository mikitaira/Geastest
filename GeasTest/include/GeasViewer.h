#pragma once

#ifndef __GEASSVIEWER_H__
#define __GEASSVIEWER_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <vector>
#include <GL/glut.h>
#include <GL/gl.h>
#include <GL/freeglut.h>


#include <Voxelyze.h>
#include <VX_Logger.h>
#include <VX_MeshRender.h>
#include "GeasDynamics.h"
#include "AgentSimulation.h"


typedef unsigned char winObj;
enum dispWindows : unsigned char {
	MAIN = 1 << 0,
	SUB_1 = 1 << 1,
	SUB_2 = 1 << 2,
	SUB_3 = 1 << 3,
};


struct MaterialStruct {
	GLfloat ambient[4];
	GLfloat diffuse[4];
	GLfloat specular[4];
	GLfloat emission[4];
	GLfloat shininess;
};


#define mainWindowW 500
#define mainWindowH 500
#define subWindowW 300
#define subWindowH 300
#define mainWindowPosX 10
#define mainWindowPosY 10

inline void winSet(winObj& obj, dispWindows dat, bool set) { set ? obj |= dat : obj &= ~dat; }
inline bool winIsSet(winObj obj, dispWindows dat) { return (dat&obj) ? true : false; }
inline dispWindows operator++ (dispWindows& dat) { return dat = (dispWindows)(std::underlying_type<dispWindows>::type(dat) << 1); }

void idleGL();
void paintGL();
void reshapeMain(int w, int h);
void mouse(int button, int state, int x, int y);
void mouseWheel(int WheelNumber, int Direction, int x, int y);
void key(unsigned char key, int x, int y);
void special(int key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void specialUp(int key, int x, int y);

void initializeViewer(int argc, char * argv[]);
void setWinObj(std::string onoff);
void createWindow(int & id, char * name, int pos[2], int size[2], void(*dispCallback)(void), void(*resizeCallback)(int, int));
void initGL();
void setCurrentTime(int t);


void setupCamera(bool _3D);
void drawGrid(int gridSize);
void drawVoxels(int index, int axis, bool line);
void resetSimulation();
void startSimulation();
void pauseSimulation();

void setgSkipStep(float timestep);
float getColorScale(float value, float scale);
void DrawString(std::string str, int w, int h, int x0, int y0);
// void alterVoxels();
// void setSaveMovie(std::string dirname);


class GeasViewer
{
public:
	GeasViewer() { std::cout << "*** Initialized GeassViewer ! ***" << std::endl; };

	~GeasViewer()
	{
	};
};


#endif // GEASSVIEWER_H

