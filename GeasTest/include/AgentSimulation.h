#pragma once

#ifndef __AGENTSIMULATION_H__
#define __AGENTSIMULATION_H__

#include "GeasDynamics.h"

#define userDeltaDivitedTime 1.0e-6
#define userDeltaTime 1.0e-3
#define maxIterNum 1e3
#include <string>
class AgentSimulation
{

public:
	int timeStep;
	GeasDynamics Gd = GeasDynamics("As");

public:
	AgentSimulation()
	{
	};

	~AgentSimulation() {};

	int configuration() {
		
		// ** Common configuration ** //
		Gd.setDeltaTime(userDeltaTime);
		Gd.setDeltaDividedTime(userDeltaDivitedTime);
		Gd.setMaxIteration(maxIterNum);
		Gd.setGravity(-1);

		// ** Rigid bodied agent ** //
		Gd.rigidBody = new GD_Rigidbody(Gd.dynamicsWorld);

		Gd.rigidbody()->setGround(Vec3D<float>(0, 1, 0), Vec3D<float>(0, -1, 0), Quat3D<float>(0, 0, 0, 1));
		//Gd.rigidbody()->addSpehre(0.3, 1, Vec3D<float>(-0.4, 0.50, 0), Quat3D<float>(1, 0, 0, 1));
		//std::cout << Gd.rigidbody()->bodyList()->at(0)->getRestitution() << std::endl;
		//Gd.rigidbody()->addBox(Vec3D<float>(1.0, 1.0, 1.0), 1.0, Vec3D<float>(0.5, 10, 0), Quat3D<float>(0, 0, 0.707, 0.707));
		//Gd.rigidbody()->addCapsule(1.0, 2.0, 1.0, Vec3D<float>(5.5, 10, 0), Quat3D<float>(0, 0, 0.707, 0.707));
		// ˆÈ‰º‚ÍŒŸØ—p
		//	Gd.rigidbody()->addCylider(Vec3D<float>(0.30, 0.020, 0.30), 1.0, Vec3D<float>(-0.4, 0.5, 0), Quat3D<float>(0.707, 0, 0, 0.707));
		//Gd.rigidbody()->addCylider(Vec3D<float>(0.10, 0.020, 0.30), 1.0, Vec3D<float>(0.4, 0.5, 0), Quat3D<float>(0.707, 0, 0, 0.707));
		Gd.rigidbody()->addCylider(Vec3D<float>(0.305, 0.020, 0.305), 1.0, Vec3D<float>(0, 0.35, 0), Quat3D<float>(0.707, 0, 0, 0.707));



		// ** Soft bodied agent ** // 
		// for testing gravity 
		//double r = 0.4;
		//int nb = 0;
		//for (double theta = 0; theta < 2 * PI; theta += PI/2) {
		//	Gd.addSoftbody(GD_Softbody::VOXEL, 0.01); //5mm voxels		
		//	CVX_Material* mat = Gd.softbody(nb)->addMaterial(1e6, 1e3);
		//	mat->setCollisionDamping(0.5);			
		//	Gd.softbody(nb)->Vx.enableFloor(true);
		//	double x = r * cos(theta);
		//	double y = r * sin(theta);
		//	Gd.softbody(nb)->setPosition(Vec3D<double>(-0.4 + x, 0.5 + y, 0.0));
		//	Gd.softbody(nb)->setOrientation(Quat3D<double>(theta-PI/2, Vec3D<double>(0, 0, 1)));
		//	for (int i = 0; i<1; i++) {
		//		for (int j = 0; j < 1; j++) {
		//			for (int k = 0; k < 1; k++) {
		//				Gd.softbody(nb)->addVoxel(mat, -2 + i, -0 + j, -2 + k);
		//			}
		//		}
		//	}
		//	nb++;
		//}

		// for testing spring joint
		//int nb = 0;
		//Gd.addSoftbody(GD_Softbody::VOXEL, 0.01); //5mm voxels		
		//Gd.softbody(nb)->Vx.enableFloor(true);
		//CVX_Material* mat = Gd.softbody(nb)->addMaterial(1e6, 1e3);
		//mat->setCollisionDamping(0.5);
		//Gd.softbody(nb)->setPosition(Vec3D<double>(-0.4, 0.0 + Gd.softbody(nb)->Vx.voxelSize()/2, 0.0));
		//Gd.softbody(nb)->setOrientation(Quat3D<double>(0, Vec3D<double>(0, 0, 1)));
		//for (int i = 0; i<1; i++) {
		//	for (int j = 0; j < 10; j++) {
		//		for (int k = 0; k < 1; k++) {
		//			Gd.softbody(nb)->addVoxel(mat, -2 + i, 50 + j, -2 + k);
		//			
		//		}
		//	}
		//}
		//Gd.softbody(nb)->setTargetElement(Gd.softbody(nb)->voxelList()->at(/*k + 3 * j + 3 * 3 * i*/0));

		//for (int i = 0; i < Gd.softbody(0)->voxelList()->size(); i++) {
		//	Vec3D<float> grav = Vec3D<float>(0, Gd.gravity() *  Gd.softbody(0)->Vx.voxel(i)->material()->mass(), 0);
		//	grav = Gd.softbody(0)->getOrientation().RotateVec3DInv(grav);
		//	Gd.softbody(0)->voxelList()->at(i)->external()->setForce(grav);
		//}

		double r = 0.325;
		//double r = 0.1;
		int nbSoftbody = 0;
		for (double theta = PI / 2 * 0; theta <= PI / 2 * 3; theta += PI / 2) {
			//for (double theta = 0; theta < 2 * PI; theta += PI / 2) {

			Gd.addSoftbody(GD_Softbody::VOXEL, 0.01); //5mm voxels		

			Gd.softbody(nbSoftbody)->Vx.enableFloor(true);
			Gd.softbody(nbSoftbody)->Vx.setGravity(1);
			CVX_Material* mat = Gd.softbody(nbSoftbody)->addMaterial(1e6, 1e3);
			mat->setGreen(255);

			mat->setCollisionDamping(0.5);

			double x = r * cos(theta);
			double y = r * sin(theta);
			Gd.softbody(nbSoftbody)->setPosition(Vec3D<double>(0.0 + x, 0.35 + y, 0.0));
			Gd.softbody(nbSoftbody)->setOrientation(Quat3D<double>(theta - PI / 2, Vec3D<double>(0, 0, 1)));

			//Gd.softbody(nbSoftbody)->addVoxel(mat,0, 0, 0);
			for (int i = 0; i<5; i++) {
				for (int j = 0; j < 5; j++) {
					for (int k = 0; k < 5; k++) {
						Gd.softbody(nbSoftbody)->addVoxel(mat, -2 + i, -2 + j, -2 + k);
					}
				}
			}

			Gd.softbody(nbSoftbody)->setTargetElement(Gd.softbody(nbSoftbody)->Vx.voxel(0, 0, 0));
			nbSoftbody++;

		}



		timeStep = 0;
		return 0;
	};

	int simulation() 
	{


		std::cout << "*** "<< timeStep + 1 <<" times Simulation ***" << std::endl;

		gdRigidBody* body = Gd.rigidbody()->bodyList()->at(0);
		btTransform trans;
		body->getMotionState()->getWorldTransform(trans);

		btCollisionShape * btCS = body->getCollisionShape();
		btCS->getShapeType();

			
		//std::cout << "Elapsed time [s]: " << timeStep * Gd.deltaTime()
		//	<< "\t sphere height: " << trans.getOrigin().getY()
		//	<< "\t sphere velocity.y: " << body->getLinearVelocity().getY() << std::endl;
		//std::cout << "Elapsed time [s]: " << timeStep * Gd.deltaTime()
		//	<< "\t voxel height: " << Gd.softbody(0)->position.y
		//	<< "\t voxel velocity.y: " << -Gd.softbody(0)->voxelList()->at(0)->velocity().x << std::endl << std::endl;
		
		Gd.doTimeStep(); //simulate  100 timesteps.

		timeStep++;
		std::cout << "*** Executed ***" << std::endl;
		return 0;
	};
};

#endif // GEASSVIEWER_H

