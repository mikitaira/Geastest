#include "GeasDynamics.h"

GeasDynamics::GeasDynamics(std::string str)
{
	dT = defaltTimeStep;
	dvT = defaltDividedTimeStep;
	mN = defaltMaxIteration;
	broadphase = new btDbvtBroadphase();
	collisionConfiguration = new btDefaultCollisionConfiguration();
	dispatcher = new btCollisionDispatcher(collisionConfiguration);
	solver = new btSequentialImpulseConstraintSolver;
	dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);

	isGrav = false;

	std::cout << "*** Initialized GeassDynamics ! ***" << str << std::endl;
}


GeasDynamics::~GeasDynamics()
{
	delete dynamicsWorld;
	delete solver;
	delete collisionConfiguration;
	delete dispatcher;
	delete broadphase;
}



bool GeasDynamics::doTimeStep()
{
	// ** First, this function execute softbody simulation. ** //
	for (int i = 0; i < mN; i++)
	{
		// set gravity force to all soft body element
		// already unused : if (isGrav) applyGravityToSoftbody();		
		for (int nb = 0; nb < softBodyCount(); nb++) {
			softbody(nb)->Vx.doTimeStep(dvT);
			if(softbody(nb)->targetElement!=NULL) softbody(nb)->updatePose();
		}
		
	}

	// ** Second, the process goes on rigid body simulation. ** //
	//if (rigidBody != NULL) 
	//{
	//	rigidbody()->rigidDynamicsWorld->stepSimulation(dT, 1, dT);

	//}

	if (rigidBody != NULL){

		rigidbody()->rigidDynamicsWorld->stepSimulation(dvT, mN-1, dvT);

	}

	return true;
}

/*
void GeassDynamics::applyGravityToSoftbody()
{
	for (int nb = 0; nb < softBodyCount(); nb++) {
		for (int k = 0; k < softbody(nb)->voxelList()->size(); k++) {
			Vec3D<float> gravForce = Vec3D<float>(0, gravity() *  softbody(nb)->Vx.voxel(k)->material()->mass(), 0);
			gravForce = softbody(nb)->getOrientation().RotateVec3DInv(gravForce);
			softbody(nb)->voxelList()->at(k)->external()->setForce(gravForce);
		}
	}
}
*/


void GeasDynamics::setDeltaDividedTime(double dvt)
{
	dvT = dvt;
}

void GeasDynamics::setDeltaTime(double dt)
{
	dT = dt;
}


void GeasDynamics::setMaxIteration(int N)
{
	mN = N;
}

void GeasDynamics::setGround(Vec3D<float> pos, Vec3D<float> offset, Quat3D<float> trans)
{
	groundPos = pos;
	groundOffset = offset;
	groundTrans = trans;

	rigidBody->setGround(pos, offset, trans);
};

void GeasDynamics::setGravity(double g)
{
	grav = g;
	isGrav = true;
	dynamicsWorld->setGravity(btVector3(0, grav, 0));

}
