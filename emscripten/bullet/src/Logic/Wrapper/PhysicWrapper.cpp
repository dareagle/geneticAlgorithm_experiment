

#include "PhysicWrapper.hpp"


#include "../Data.hpp"

#include "../Physic/World.hpp"
#include "../Physic/Trimesh.hpp"
#include "../Physic/Vehicle.hpp"


PhysicWrapper::~PhysicWrapper()
{}


//
//
// world

void	PhysicWrapper::step()
{
	Data::get()->m_PhysicWorld.step();
}

//
//
// trimesh

void	PhysicWrapper::createGround(const std::vector<float>& b, const std::vector<int>& i, int id)
{
	Data::get()->m_PhysicWorld.createGround(b, i, id);
}

void	PhysicWrapper::createGhostWall(const std::vector<float>& b, const std::vector<int>& i)
{
	Data::get()->m_PhysicWorld.createWall(b, i);
}

//
//
// vehicle

void	PhysicWrapper::createVehicles(unsigned int total)
{
	for (unsigned int i = 0; i < total; ++i)
		Data::get()->m_PhysicWorld.createVehicle();
}


void	PhysicWrapper::vehicle_applyEngineForce(int vehicle_index, float engineForce)
{
	Data::get()->m_PhysicWorld.getVehicle(vehicle_index)->applyEngineForce(engineForce);
}

void	PhysicWrapper::vehicle_setSteeringValue(int vehicle_index, float vehicleSteering)
{
	Data::get()->m_PhysicWorld.getVehicle(vehicle_index)->setSteeringValue(vehicleSteering);
}

void	PhysicWrapper::vehicle_getOpenGLMatrix(int vehicle_index, float* mat4x4)
{
	Data::get()->m_PhysicWorld.getVehicle(vehicle_index)->getOpenGLMatrix(mat4x4);
}

void	PhysicWrapper::vehicle_reset(int vehicle_index, float* vec3, float* quat4)
{
	btVector3		pos(vec3[0], vec3[1], vec3[2]);
	btQuaternion	rot(quat4[0], quat4[1], quat4[2], quat4[3]);

	Data::get()->m_PhysicWorld.getVehicle(vehicle_index)->fullBrake();
	Data::get()->m_PhysicWorld.getVehicle(vehicle_index)->setPosition(pos);
	Data::get()->m_PhysicWorld.getVehicle(vehicle_index)->setRotation(rot);
}

//
//
// raycast

bool	PhysicWrapper::raycast(const float* pVec3_from, const float* pVec3_to, float* pVec3_hit, int* pID_hit)
{
	// init

	if (pVec3_hit)
		memset(pVec3_hit, 0, 3 * sizeof(float));
	if (pID_hit)
		*pID_hit = -1;

	// raycast

	// bool	success = Data::get()->m_PhysicWorld.raycast(pVec3_from, pVec3_to);
	Physic::Trimesh*	pTrimesh;
	bool	success = Data::get()->m_PhysicWorld.raycast(pVec3_from, pVec3_to, &pTrimesh, pVec3_hit);

	// result

	if (success)
	{
		// if (pID_hit)
		// 	*pID_hit = Data::get()->m_PhysicWorld.getRaycastedTrimesh()->getID();
		// if (pVec3_hit)
		// 	Data::get()->m_PhysicWorld.getRaycastedPosition(pVec3_hit);
		if (pID_hit)
			*pID_hit = pTrimesh->getID();
	}

	return success;
}
