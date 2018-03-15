#include "GD_Softbody.h"
#include <iostream>

GD_Softbody::GD_Softbody(collisionShapeType col, double voxelSize)
{
	Vx = CVoxelyze(voxelSize);
	Vx.setGravity(0);
	Vx.enableFloor(false);
	Vx.setMaxLayer(1);

	setPosition();
	setOrientation();
	targetElement = NULL;
	targetPosition = Vec3D<double>();
	targetOrientation = Quat3D<double>();
	std::cout << "*** Initialized GD_Softbody ! ***" << std::endl;
}

GD_Softbody::~GD_Softbody()
{
}

void GD_Softbody::updatePose()
{
	if (targetElement != NULL) {
		//std::cout << "is called?" << std::endl;

		Vec3D<double>updatePosition = targetElement->position() - targetPosition;
		Quat3D<double>updateOrientation = targetElement->orientation() * targetOrientation.Inverse();

		targetPosition = targetElement->position();
		targetOrientation = targetElement->orientation();


		setOrientation(updateOrientation * quaternion);
		setPosition(position + quaternion.RotateVec3DInv(updatePosition));

	}
}

CVX_Material* GD_Softbody::addMaterial(float youngsModulus = 1e6, float density = 1e3)
{
	return (Vx.addMaterial(youngsModulus, density));
}

CVX_Voxel* GD_Softbody::addVoxel(CVX_Material* material, int xIndex, int yIndex, int zIndex)
{
	return (Vx.setVoxel(material, xIndex, yIndex, zIndex));
}

