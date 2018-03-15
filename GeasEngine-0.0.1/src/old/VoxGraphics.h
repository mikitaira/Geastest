#ifndef __VOXGRAPHICS_H__
#define __VOXGRAPHICS_H__

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
#include "VoxRobot.h"
#include "GLCapture.hpp"
#include "Utility.h"


#ifdef _DEBUG


#pragma comment(lib,"lib\\opencv_world320d.lib")

#else											  
//ReleaseÉÇÅ[ÉhÇÃèÍçá

#pragma comment(lib,"lib\\opencv_world320.lib")

#endif

typedef unsigned char winObj;
enum dispWindows : unsigned char {
	MAIN = 1 << 0,
	EXT_X = 1 << 1,
	EXT_Y = 1 << 2,
	EXT_Z = 1 << 3,
	CUR_X = 1 << 4,
	CUR_Y = 1 << 5,
	CUR_Z = 1 << 6
};

inline void winSet(winObj& obj, dispWindows dat, bool set) { set ? obj |= dat : obj &= ~dat; }
inline bool winIsSet(winObj obj, dispWindows dat) { return (dat&obj) ? true : false; }
inline dispWindows operator++ (dispWindows& dat) { return dat = (dispWindows)(std::underlying_type<dispWindows>::type(dat) << 1); }

void setWinObj(std::string onoff);
void setSaveMovie(std::string dirname);
void idleGL();
void paintGL();
void paintCurrentForceX();
void paintCurrentForceY();
void paintCurrentForceZ();
void paintExternalForceX();
void paintExternalForceY();
void paintExternalForceZ();
void reshapeMain(int w, int h);
void reshapeCurrentForceX(int w, int h);
void reshapeCurrentForceY(int w, int h);
void reshapeCurrentForceZ(int w, int h);
void reshapeExternalForceX(int w, int h);
void reshapeExternalForceY(int w, int h);
void reshapeExternalForceZ(int w, int h);
void setupCamera(bool _3D);
void setupCameraZoom(bool _3D);
void voxUpdatePhysX();
void voxUpdateDisp();
void drawVoxels(int axis,bool line);
void drawCube(Vec3D<float> vertex[], int face[][4]);
void drawGrid(int gridSize);
void mouse(int button, int state, int x, int y);
void key(unsigned char key, int x, int y);
void keyUp(unsigned char key, int x, int y);
void mouseWheel(int WheelNumber, int Direction, int MouseX, int MouseY);
void setgSkipStep(float timestep);
void special(int key, int x, int y);
void specialUp(int key, int x, int y);
void setColorExtForce(CVoxelyze* voxelyzeInstance, int quadCount, std::vector<int>  &quadVoxIndices, std::vector<float>  &array, int coloraxis);
void setColorInnForce(CVoxelyze* voxelyzeInstance, int quadCount, std::vector<int>  &quadVoxIndices, std::vector<float>  &array, int coloraxis);
float getColorScale(float value, float scale);
void DrawString(std::string str, int w, int h, int x0, int y0);
void resetSimulation();
void startSimulation();
void pauseSimulation();
void alterVoxels();


struct MaterialStruct {
    GLfloat ambient[4];
    GLfloat diffuse[4];
    GLfloat specular[4];
    GLfloat emission[4];
    GLfloat shininess;
};

class VoxGraphics
{
    public:
		//VoxGraphics(int argc, char *argv[]);
        VoxGraphics();
        ~VoxGraphics();
		void initVoxGraphics(int argc, char *argv[]);
        void initGL();
        void initVx();
		void createWindow(int &id, char *name, int pos[2], int size[2], void(*dispCallback)(void), void(*resizeCallback)(int, int));
};


#endif // VOXGRAPHICS_H
