
#ifndef D_DATA_HPP
#define D_DATA_HPP


#include <string>
#include <list>
#include <map>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengles2.h>

// #include <GLES2/gl2.h>
// #include <GLES2/gl2ext.h>

#include <glm/vec2.hpp> // glm::vec2
#include <glm/vec3.hpp> // glm::vec3


#include "Graphic/ShaderColor.hpp"
#include "Graphic/GeometryColor.hpp"
#include "Graphic/StackRenderer.hpp"
#include "Graphic/BulletDebugDraw.hpp"

#include "Physic/World.hpp"
#include "Physic/Trimesh.hpp"
#include "Physic/Vehicle.hpp"

#include "Wrapper/PhysicWrapper.hpp"

#include "../Simulation/Simulation.hpp"


// #include <btBulletDynamicsCommon.h>

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

	GeometryColor	m_GeometryColor_chassis;
	// GeometryColor	m_GeometryColor_chassis_dead;
	GeometryColor	m_GeometryColor_wheel;
	GeometryColor	m_GeometryColor_circuit_skelton;
	GeometryColor	m_GeometryColor_circuit_ground;
	GeometryColor	m_GeometryColor_circuit_walls;

	std::list<GeometryColor>	m_GeometryColor_trails;

	glm::mat4		m_composedMatrix;
	StackRenderer	m_StackRenderer;

	BulletDebugDraw	m_DebugDraw;

	//

	std::vector<float>	m_arr_skelton_left;
	std::vector<float>	m_arr_skelton_right;

	//

	Physic::World	m_PhysicWorld;

	PhysicWrapper*	m_pPhysicWrapper;
	Simulation*		m_pSimulation;

	int				m_simualtion_step;

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



#endif // D_DATA_HPP
