

#include "Vehicle.hpp"

#include <Bullet/btBulletDynamicsCommon.h>


namespace	Physic
{


Vehicle::Vehicle(btDiscreteDynamicsWorld* pDynamicsWorld)
{
#define CUBE_HALF_EXTENTS 1

	//
	//
	// STATS

	btVector3 wheelDirectionCS0(0,0,-1);
	btVector3 wheelAxleCS(1,0,0);
	float	wheelRadius = 0.5f;
	float	wheelWidth = 0.2f;
	btScalar suspensionRestLength(0.6);

	float	wheelFriction = 1000;//BT_LARGE_FLOAT;
	float	suspensionStiffness = 20.f;
	float	suspensionDamping = 2.3f;
	float	suspensionCompression = 4.4f;
	float	rollInfluence = 0.1f;//1.0f;


	//
	//
	// CHASSIS

	m_pChassisShape = new btBoxShape(btVector3(1.f,2.f, 0.5f));
	m_pCompound = new btCompoundShape();

	btTransform localTrans;
	localTrans.setIdentity();
	localTrans.setOrigin(btVector3(0,0,1));
	m_pCompound->addChildShape(localTrans, m_pChassisShape);


	btScalar	mass = 80;
	btVector3 localInertia(0,0,0);
	m_pCompound->calculateLocalInertia(mass,localInertia);

	btTransform tr;
	tr.setIdentity();
	// tr.setOrigin(btVector3(10,10,10));
	tr.setOrigin(btVector3(0,0,10));

	m_pMotionState = new btDefaultMotionState(tr);
	btRigidBody::btRigidBodyConstructionInfo cInfo(mass, m_pMotionState, m_pCompound, localInertia);
	m_pCarChassis = new btRigidBody(cInfo);
	// pDynamicsWorld->addRigidBody(m_pCarChassis, 0, 0);


	//
	//
	// VEHICLE

	btRaycastVehicle::btVehicleTuning	tuning;
	m_pVehicleRayCaster = new btDefaultVehicleRaycaster(pDynamicsWorld);

	m_pVehicle = new btRaycastVehicle(tuning, m_pCarChassis, m_pVehicleRayCaster);

	// never deactivate the vehicle
	m_pCarChassis->setActivationState(DISABLE_DEACTIVATION);

	// m_physic.dynamicsWorld->addVehicle(m_pVehicle);


	//
	//
	// WHEELS

	float connectionHeight = 1.2f;

	// choose coordinate system
	int rightIndex = 0;
	int upIndex = 2;
	int forwardIndex = 1;
	m_pVehicle->setCoordinateSystem(rightIndex,upIndex,forwardIndex);

	bool isFrontWheel = true;

	btVector3 connectionPointCS0(CUBE_HALF_EXTENTS-(0.3*wheelWidth),2*CUBE_HALF_EXTENTS-wheelRadius, connectionHeight);
	m_pVehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,tuning,isFrontWheel);

	connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS+(0.3*wheelWidth),2*CUBE_HALF_EXTENTS-wheelRadius, connectionHeight);
	m_pVehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,tuning,isFrontWheel);

	isFrontWheel = false;

	connectionPointCS0 = btVector3(-CUBE_HALF_EXTENTS+(0.3*wheelWidth),-2*CUBE_HALF_EXTENTS+wheelRadius, connectionHeight);
	m_pVehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,tuning,isFrontWheel);

	connectionPointCS0 = btVector3(CUBE_HALF_EXTENTS-(0.3*wheelWidth),-2*CUBE_HALF_EXTENTS+wheelRadius, connectionHeight);
	m_pVehicle->addWheel(connectionPointCS0,wheelDirectionCS0,wheelAxleCS,suspensionRestLength,wheelRadius,tuning,isFrontWheel);
	
	for (int i = 0; i < m_pVehicle->getNumWheels(); ++i)
	{
		btWheelInfo&	wheel = m_pVehicle->getWheelInfo(i);

		wheel.m_suspensionStiffness = suspensionStiffness;
		wheel.m_wheelsDampingRelaxation = suspensionDamping;
		wheel.m_wheelsDampingCompression = suspensionCompression;
		wheel.m_frictionSlip = wheelFriction;
		wheel.m_rollInfluence = rollInfluence;
	}

#undef	CUBE_HALF_EXTENTS



	// // btTransform transform;
	// // m_pCarChassis->getMotionState()->getWorldTransform(transform);
	// // m_pCarChassis->getMotionState()->setWorldTransform(transform);

	// btTransform initialTransform;
	// initialTransform.setIdentity();
	// initialTransform.setOrigin(btVector3(0,0,10));
	// initialTransform.setRotation(btQuaternion(0,0,1,1));

	// m_pCarChassis->setWorldTransform(initialTransform);
	// m_pMotionState->setWorldTransform(initialTransform);
}

Vehicle::~Vehicle()
{
	delete m_pVehicle;
	delete m_pVehicleRayCaster;

	delete m_pCarChassis;
	delete m_pMotionState;
	delete m_pCompound;
	delete m_pChassisShape;
}


void	Vehicle::applyEngineForce(float engineForce)
{
	int wheelIndex = 2;
	m_pVehicle->applyEngineForce(engineForce, wheelIndex);
	wheelIndex = 3;
	m_pVehicle->applyEngineForce(engineForce, wheelIndex);
}

void	Vehicle::setSteeringValue(float vehicleSteering)
{
	int wheelIndex = 0;
	m_pVehicle->setSteeringValue(vehicleSteering, wheelIndex);
	wheelIndex = 1;
	m_pVehicle->setSteeringValue(vehicleSteering, wheelIndex);
}

void	Vehicle::fullBrake()
{
	m_pCarChassis->clearForces();
	btVector3 zeroVector(0,0,0);
	m_pCarChassis->setLinearVelocity(zeroVector);
	m_pCarChassis->setAngularVelocity(zeroVector);

	m_pVehicle->setBrake(100, 0);
	m_pVehicle->setBrake(100, 1);
	m_pVehicle->setBrake(100, 2);
	m_pVehicle->setBrake(100, 3);

	m_pVehicle->updateVehicle(0);

	m_pVehicle->setBrake(0, 0);
	m_pVehicle->setBrake(0, 1);
	m_pVehicle->setBrake(0, 2);
	m_pVehicle->setBrake(0, 3);
}

void	Vehicle::setPosition(const float* pVec3_pos)
{
	btVector3	pos(pVec3_pos[0], pVec3_pos[1], pVec3_pos[2]);

	btTransform initialTransform = m_pCarChassis->getWorldTransform();

	initialTransform.setOrigin(pos);

	m_pCarChassis->setWorldTransform(initialTransform);
	m_pMotionState->setWorldTransform(initialTransform);
}

void	Vehicle::setRotation(const float* pQuat_rot)
{
	btQuaternion	rot(pQuat_rot[0], pQuat_rot[1], pQuat_rot[2], pQuat_rot[3]);

	btTransform initialTransform = m_pCarChassis->getWorldTransform();

	initialTransform.setRotation(rot);

	m_pCarChassis->setWorldTransform(initialTransform);
	m_pMotionState->setWorldTransform(initialTransform);
}

void	Vehicle::getOpenGLMatrix(btScalar* mat4)
{
	m_pCarChassis->getWorldTransform().getOpenGLMatrix(mat4);
}

//

int		Vehicle::getNumWheels() const
{
	return m_pVehicle->getNumWheels();
}

void	Vehicle::getWheelMatrix(int index, float* mat4) const
{
	m_pVehicle->getWheelTransformWS(index).getOpenGLMatrix(mat4);
}




};

