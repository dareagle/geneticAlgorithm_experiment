

#pragma once


#include <vector>


class IPhysicWrapper
{
public:
	virtual ~IPhysicWrapper()
	{}

public: // to override

	//
	//
	// world

	virtual void	step(float elapsed_time, int world_index) = 0;

	//
	//
	// trimesh

	virtual void	createGround(const std::vector<float>& v, const std::vector<int>& i, int id) = 0;
	virtual void	createGhostWall(const std::vector<float>& v, const std::vector<int>& i) = 0;

	//
	//
	// vehicle

	virtual void	createVehicles(unsigned int total) = 0;
	virtual void	vehicle_applyEngineForce(int vehicle_index, float engineForce) = 0;
	virtual void	vehicle_setSteeringValue(int vehicle_index, float vehicleSteering) = 0;
	virtual void	vehicle_getOpenGLMatrix(int vehicle_index, float* mat4x4) = 0;
	virtual void	vehicle_reset(int vehicle_index, float* vec3, float* quat4) = 0;

	//
	//
	// raycast

	virtual bool	raycast(const float* vec3_from, const float* vec3_to, float* vec3_hit, int* id_hit) = 0;
};
