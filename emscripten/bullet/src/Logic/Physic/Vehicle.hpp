

#ifndef D_VEHICLE_HPP
#define D_VEHICLE_HPP



class	btCollisionShape;
class	btCompoundShape;
class	btDefaultMotionState;
class	btRigidBody;

class	btDefaultVehicleRaycaster;
class	btRaycastVehicle;

class	btDiscreteDynamicsWorld;


namespace	Physic
{


class World;

class Vehicle
{
private:
	friend World;

private:
	btCollisionShape*	m_pChassisShape;
	btCompoundShape*	m_pCompound;
	btDefaultMotionState*	m_pMotionState;
	btRigidBody*		m_pCarChassis;

	btDefaultVehicleRaycaster*	m_pVehicleRayCaster;
	btRaycastVehicle*	m_pVehicle;

private:
	Vehicle(btDiscreteDynamicsWorld* pDynamicsWorld);
	~Vehicle();

public:
	void	applyEngineForce(float engineForce);
	void	setSteeringValue(float vehicleSteering);
	void	fullBrake();

public:
	void	setPosition(const float* pVec3_pos);
	void	setRotation(const float* pQuat_rot);

public:
	void	getOpenGLMatrix(float* mat4);

	int		getNumWheels() const;
	void	getWheelMatrix(int index, float* mat4) const;

};


};


#endif // D_VEHICLE_HPP

