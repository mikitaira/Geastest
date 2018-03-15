#ifndef __VOXROBOT_H__
#define __VOXROBOT_H__

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <math.h>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include <unordered_map>
#include <Voxelyze.h>
#include <VX_MeshRender.h>
#include "VoxGraphics.h"
#include <random>

enum ENVIRONMENT {LAND, WATER, AIR};

struct ActVoxel {
	CVX_Voxel* voxel;
	CVX_Material* material;
	int voxIndices; //Voxelyze::voxelsList のインデックス
	float time;
};

class VoxRobot
{
    public:
    VoxRobot();
    ~VoxRobot();

	void initVoxRobot();
    void initVoxelyze();
    void initVoxels();
	double vibration(double T, double samplingRate, double phi);
	void actuateVoxels(ActVoxel *avoxel, bool act, double phase);
	Vec3D<float> fluid(ActVoxel *avoxel);
    void recallRobot(int rep);
	void iniExternal();
	void alterVoxels(int alterCount);
	void createActVoxelList();

	void logOutPut();

	void resetSimTime();

    CVX_Material* createMaterial(float pa, float dens, float poissons,
                                 int red, int green, int blue, int alpha,
                                 float cte, float kfric, float sfric, float cdamp, float idamp, float gdamp);

    void setVx(CVoxelyze Vx);
    CVoxelyze getVx();

    void setMeshRender(CVX_MeshRender* meshRender);
    CVX_MeshRender* getMeshRender();

    void setMaterial(CVX_Material* vx_mat);
    CVX_Material* getMaterial();

    void setDefaultDynamicRedMaterial(CVX_Material* material);
    void setDefaultDynamicGreenMaterial(CVX_Material* material);
    void setDefaultDynamicBlueMaterial(CVX_Material* material);

    public:
    ENVIRONMENT env;

    int timeStep;

	std::vector<std::vector<ActVoxel>>  actVoxelList;

    CVoxelyze Vx;
    CVX_MeshRender* meshRender;
    CVX_Material* vx_mat;

    CVX_Material*   matRed;
    CVX_Material*   matGreen;
    CVX_Material*   matBlue;

	int firstStage;
	int secondStage;

};

#endif // VOXROBOT_H
