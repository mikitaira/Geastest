#include "GD_Rigidbody.h"

#include <iostream>

GD_Rigidbody::GD_Rigidbody(btDiscreteDynamicsWorld* dynamicsWorld)
{
	rigidDynamicsWorld = dynamicsWorld;

	std::cout << "*** Initialized GD_Rigidbody ! ***" << std::endl;
}


GD_Rigidbody::~GD_Rigidbody()
{
}

gdRigidBody* GD_Rigidbody::setGround(Vec3D<float> pos, Vec3D<float> offset, Quat3D<float> trans)
{

	btCollisionShape* groundShape = new btStaticPlaneShape(btVector3(pos.x, -pos.y, pos.z), 1);
	btDefaultMotionState* groundMotionState 
		= new btDefaultMotionState(btTransform(btQuaternion(trans.x, trans.y, trans.z, trans.w), btVector3(offset.x, offset.y, offset.z)));
	gdRigidBody::btRigidBodyConstructionInfo groundRigidBodyCI(0, groundMotionState, groundShape, btVector3(0, 0, 0));
	gdRigidBody* groundRigidBody = new gdRigidBody(groundRigidBodyCI);
	groundRigidBody->setRestitution(btScalar(0.8f));
	rigidDynamicsWorld ->addRigidBody(groundRigidBody);
	return (groundRigidBody);
}

gdRigidBody* GD_Rigidbody::addBox(Vec3D<float> halfExtent, double mass, Vec3D<float> offset, Quat3D<float> trans)
{
	btCollisionShape* shape = new btBoxShape(btVector3(halfExtent.x, halfExtent.y, halfExtent.z));
	btDefaultMotionState* shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(trans.x, trans.y, trans.z, trans.w), btVector3(offset.x, offset.y, offset.z)));
	btVector3 shapeInertia(0, 0, 0);
	shape->calculateLocalInertia(mass, shapeInertia);
	gdRigidBody::btRigidBodyConstructionInfo shapeBoxCI(mass, shapeMotionState, shape, shapeInertia);
	gdRigidBody* body = new gdRigidBody(shapeBoxCI);
	return addRigidbody(body, BOX);
}


gdRigidBody* GD_Rigidbody::addSpehre(double rad, double mass, Vec3D<float> offset, Quat3D<float> trans)
{
	btCollisionShape* shape = new btSphereShape(rad);
	btDefaultMotionState* shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(trans.x, trans.y, trans.z, trans.w), btVector3(offset.x, offset.y, offset.z)));
	btVector3 shapeInertia(0, 0, 0);
	shape->calculateLocalInertia(mass, shapeInertia);
	gdRigidBody::btRigidBodyConstructionInfo shapeSphereCI(mass, shapeMotionState, shape, shapeInertia);
	gdRigidBody* body = new gdRigidBody(shapeSphereCI);

	return addRigidbody(body, SPHERE);
}

gdRigidBody* GD_Rigidbody::addCapsule(double rad, double height, double mass, Vec3D<float> offset, Quat3D<float> trans)
{
	btCollisionShape * shape = new btCapsuleShape(rad, height);
	btDefaultMotionState* shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(trans.x, trans.y, trans.z, trans.w), btVector3(offset.x, offset.y, offset.z)));
	btVector3 shapeInertia(0, 0, 0);
	shape->calculateLocalInertia(mass, shapeInertia);
	gdRigidBody::btRigidBodyConstructionInfo shapeCapsuleCI(mass, shapeMotionState, shape, shapeInertia);
	gdRigidBody* body = new gdRigidBody(shapeCapsuleCI);
	return addRigidbody(body, CAPSULE);
}

gdRigidBody* GD_Rigidbody::addCylider(Vec3D<float> halfExtent, double mass, Vec3D<float> offset, Quat3D<float> trans)
{
	btCollisionShape * shape = new btCylinderShape(btVector3(halfExtent.x, halfExtent.y, halfExtent.z));
	btDefaultMotionState* shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(trans.x, trans.y, trans.z, trans.w), btVector3(offset.x, offset.y, offset.z)));
	btVector3 shapeInertia(0, 0, 0);
	shape->calculateLocalInertia(mass, shapeInertia);
	gdRigidBody::btRigidBodyConstructionInfo shapeCylinderCI(mass, shapeMotionState, shape, shapeInertia);
	gdRigidBody* body = new gdRigidBody(shapeCylinderCI);
	return addRigidbody(body, CYLINDER);
}

gdRigidBody* GD_Rigidbody::addSoftbody(double mass, Vec3D<float> offset, Quat3D<float> trans)
{
	btCollisionShape * shape = new btEmptyShape();
	btDefaultMotionState* shapeMotionState = new btDefaultMotionState(btTransform(btQuaternion(trans.x, trans.y, trans.z, trans.w), btVector3(offset.x, offset.y, offset.z)));
	gdRigidBody::btRigidBodyConstructionInfo shapeSoftbodyCI(mass, shapeMotionState, shape, btVector3(0, 0, 0));
	gdRigidBody* body = new gdRigidBody(shapeSoftbodyCI);
	return addRigidbody(body, SOFTBODY);
}


gdRigidBody* GD_Rigidbody::addRigidbody(gdRigidBody* rigidBody, int shapetype)
{
	rigidBodyList.push_back(rigidBody);
	shapeList.push_back(shapetype);
	rigidBody->setRestitution(0.5f);
	rigidDynamicsWorld->addRigidBody(rigidBody);
	return (rigidBody);
}

