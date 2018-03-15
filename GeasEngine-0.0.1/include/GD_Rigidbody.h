#pragma once

#include <btBulletDynamicsCommon.h>
#include <Voxelyze.h>
#include <vector>

typedef btTransform gdTransform;
typedef btRigidBody gdRigidBody;

class GD_Rigidbody
{
public:
	// ** Geometry of rigid body
	enum collisionShapeType {
		BOX,
		SPHERE,
		CAPSULE,
		CYLINDER,
		CONVEX_MESH,
		SOFTBODY,
		GROUND,
	};

	std::vector<int> shapeList;
	std::vector<gdRigidBody *> rigidBodyList;
	btDiscreteDynamicsWorld* rigidDynamicsWorld;

public:
	GD_Rigidbody(btDiscreteDynamicsWorld* dynamicsWorld);
	~GD_Rigidbody();
	gdRigidBody* setGround(Vec3D<float> pos, Vec3D<float> offset, Quat3D<float> trans);
	gdRigidBody* addSpehre(double rad, double mass, Vec3D<float> offset, Quat3D<float> trans);
	gdRigidBody * addCapsule(double rad, double height, double mass, Vec3D<float> offset, Quat3D<float> trans);
	gdRigidBody * addCylider(Vec3D<float> halfExtent, double mass, Vec3D<float> offset, Quat3D<float> trans);
	gdRigidBody * addSoftbody(double mass, Vec3D<float> offset, Quat3D<float> trans);
	gdRigidBody * addBox(Vec3D<float> halfExtent, double mass, Vec3D<float> offset, Quat3D<float> trans);
	gdRigidBody * addRigidbody(gdRigidBody* rigidBody, int shapetype);

	int bodyCount() const { return rigidBodyList.size(); } //!< Returns the number of voxels currently in this voxelyze object.
	gdRigidBody* getRigidbody(int Index) const { return rigidBodyList[Index]; } //!< Returns a pointer to a voxel that has been added to this voxelyze object. CVX_Voxel public member functions can be safely called on this pointer to query or modify the voxel. A given index may or may not always return the same voxel - Use voxel pointers to keep permanent handles to specific voxels. This function is primarily used while iterating through all voxels in conjuntion with voxelCount(). @param[in] voxelIndex the current index of a voxel. Valid range from 0 to voxelCount()-1.
	const std::vector<gdRigidBody*>* bodyList() const { return &rigidBodyList; }
	int getShape(int Index) const { return shapeList[Index]; } //!< Returns a pointer to a voxel that has been added to this voxelyze object. CVX_Voxel public member functions can be safely called on this pointer to query or modify the voxel. A given index may or may not always return the same voxel - Use voxel pointers to keep permanent handles to specific voxels. This function is primarily used while iterating through all voxels in conjuntion with voxelCount(). @param[in] voxelIndex the current index of a voxel. Valid range from 0 to voxelCount()-1.
	const std::vector<int>* getShapeList() const { return &shapeList; }
};

