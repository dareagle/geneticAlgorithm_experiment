

#pragma once


#include "OpenGLES.hpp"


#include "Graphic/Color/ShaderColor.hpp"
#include "Graphic/Color/GeometryColor.hpp"
#include "Graphic/StackRenderer.hpp"
#include "Graphic/BulletDebugDraw.hpp"
#include "Graphic/Inst/InstShader.hpp"
#include "Graphic/Inst/InstGeometry.hpp"

#include "Physic/World.hpp"
#include "Physic/Trimesh.hpp"
#include "Physic/Vehicle.hpp"

#include "Threading/Producer.hpp"

#include "Wrapper/PhysicWrapper.hpp"

#include "../Simulation/Simulation.hpp"


#include <string>
#include <list>
#include <map>


#include <glm/vec2.hpp> // glm::vec2
#include <glm/vec3.hpp> // glm::vec3
#include <glm/mat4x4.hpp> // glm::mat4


class Data
{

	//
	//
	// singleton

private:
	static Data*	m_pInstance;

	Data();
	~Data();
private:
	void	initialise();

public:
	static void	create();
	static void	destroy();
	static Data*	get();

	// singleton
	//
	//

public:

	//
	// camera

	glm::vec2	m_vec2_rotations;
	glm::vec3	m_vec3_center;

	int			m_timeout_camera;
	int			m_index_targetedCar;

	// camera
	//


	//
	//
	// experimental

	ShaderColor		m_ShaderColor;

	// GeometryColor	m_GeometryColor_chassis;
	// // GeometryColor	m_GeometryColor_chassis_dead;
	// GeometryColor	m_GeometryColor_wheel;
	GeometryColor	m_GeometryColor_circuit_skelton;
	GeometryColor	m_GeometryColor_circuit_ground;
	GeometryColor	m_GeometryColor_circuit_walls;

	std::list<GeometryColor>	m_GeometryColor_trails;

	glm::mat4		m_composedMatrix;
	StackRenderer	m_StackRenderer;

	BulletDebugDraw	m_DebugDraw;


	struct t_graphic
	{
		// glm::mat4		m_proj, m_view;
		InstShader		m_InstShader;
		InstGeometry	m_InstGeometry_chassis;
		InstGeometry	m_InstGeometry_wheel;
	}
	m_graphic;


	//

	std::vector<float>	m_arr_skelton_left;
	std::vector<float>	m_arr_skelton_right;

	//

	Physic::World	m_PhysicWorld;
	Physic::World	m_PhysicWorld2;
	Physic::World	m_PhysicWorld3;

	PhysicWrapper*	m_pPhysicWrapper;
	Simulation*		m_pSimulation;

	int				m_simualtion_step;

	//

    Producer	m_Producer;
    int			m_running = 0;

	// experimental
	//
	//


	struct t_input
	{
		// bool	keys[SDLK_LAST];
		std::map<int, bool>	keys;
	}
	m_input;

	// struct t_error
	// {
	// 	std::string m_message;
	// }
	// m_error;
};
