#pragma once
#include <iostream>
#include <vector>
#include <Voxelyze.h>

#define DEFAULT_VOXEL_SIZE 0.001

class GD_Softbody
{
public:
	// ** Geometry of rigid body
	enum collisionShapeType 
	{
		VOXEL
	};
	Vec3D<double> position;
	Quat3D<double> quaternion;
	CVX_Voxel* targetElement;
	Vec3D<double> targetPosition; // It considers the target element as a reference the point of the local axis
	Quat3D<double> targetOrientation; // It considers the target element as a reference the point of the local axis


public:
	GD_Softbody(collisionShapeType col = VOXEL, double voxelSize = DEFAULT_VOXEL_SIZE);
	~GD_Softbody();

	void updatePose();

	CVX_Material* addMaterial(float youngsModulus, float density);
	CVX_Voxel* addVoxel(CVX_Material *material, int xIndex, int yIndex, int zIndex);
	const std::vector<CVX_Voxel*>* voxelList() const { return Vx.voxelList(); }
	Vec3D<double> getGlobalPostion() { return position; };
	Quat3D<double> getOrientation() { return quaternion; };
	void setPosition() { position = Vec3D<double>(); };
	void setPosition(Vec3D<double> gPos) { position = gPos; Vx.setPosition(gPos);};
	void setTargetElement(CVX_Voxel* tElement){
		targetElement = tElement;
		targetPosition = tElement->position();
		targetOrientation = tElement->orientation();
		Vx.setOrigin(tElement);
		updatePose();
	};
	void setOrientation() { quaternion = Quat3D<double>(); };
	void setOrientation(Quat3D<double> quat) { quaternion = quat; Vx.setOrientation(quat); };
	void setOrientation(CVX_Voxel* voxel) { quaternion = voxel->orientation(); };


public:
	// ** World for soft body based on Voxelyze ** // 
	// ** Wiki reference : https://github.com/jonhiller/Voxelyze **//
	CVoxelyze Vx;
};

