#pragma once

#include <iostream>
#include <btBulletDynamicsCommon.h>
#include <Voxelyze.h>

#include "GD_Rigidbody.h"
#include "GD_Softbody.h"


#define defaltTimeStep 1e-2
#define defaltDividedTimeStep 1e-6
#define defaltMaxIteration 1e4

typedef btTransform gdTransform;
typedef btVector3 gdVector3D;
typedef btRigidBody gdRigidBody;

class GeasDynamics
{
public:

	GD_Softbody* softBody;
	std::vector<GD_Softbody*> softBodyList;
	GD_Rigidbody* rigidBody;
	Vec3D<float> groundPos;
	Vec3D<float> groundOffset;
	Quat3D<float> groundTrans;

	// ** The world for a rigid body based on Bullet physics ** //
	// ** Wiki reference : http://bulletphysics.org/mediawiki-1.5.8/index.php/Main_Page **//
	btBroadphaseInterface* broadphase;
	btDefaultCollisionConfiguration* collisionConfiguration;
	btCollisionDispatcher* dispatcher;
	btSequentialImpulseConstraintSolver* solver;
	btDiscreteDynamicsWorld* dynamicsWorld;

private:
	double dT;
	double dvT;
	int mN;
	double grav;
	bool isGrav;



public:
	GeasDynamics(std::string str);
	~GeasDynamics();

	bool doTimeStep();
	void applyGravityToSoftbody();
	void setDeltaDividedTime(double dvt = defaltDividedTimeStep); //!< Sets delta time for this simulation to apply to all objects.
	void setDeltaTime(double dt = defaltDividedTimeStep); //!< Sets delta time for this simulation to apply to all objects.
	void setMaxIteration(int N);
	float deltaTime(void) const { return dT; }
	float deltaDividedTime(void) const { return dvT; }

	void setGround(Vec3D<float> pos, Vec3D<float> offset, Quat3D<float> trans);
	void setGravity(double g = -9.80665); //!< Sets the gravitational acceleration to apply to all voxels. Gravity acts in the -Z direction. Set to 0 to disable. @param[in] g Gravitational acceleration in g's. 1 g = -9.80665 m/s^2.
	float gravity(void) const { return grav; } //!< Returns the current gravitational acceleration in g's. 1 g = -9.80665 m/s^2.
	bool isGravity(){ return isGrav ? true : false; };

	void addSoftbody(GD_Softbody::collisionShapeType col, double voxelSize) {
		softBodyList.push_back(new GD_Softbody(col, voxelSize)); 
		if(isGrav)softBodyList.at(softBodyCount() - 1)->Vx.setGravity(1); 
	}
	GD_Softbody* softbody(int index) { if (softBodyList.size() != 0) { return softBodyList.at(index); } else return NULL; }
	int softBodyCount() const { return softBodyList.size(); } 

	bool rigidbodyExists() { return rigidBody ? true : false; }
	GD_Rigidbody* rigidbody() { if (!rigidBody) {  rigidBody = new GD_Rigidbody(dynamicsWorld); } return rigidBody; }




};

