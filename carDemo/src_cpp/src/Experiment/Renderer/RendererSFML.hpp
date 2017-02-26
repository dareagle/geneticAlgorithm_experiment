

#ifndef D_RENDERERSFML_HPP
#define D_RENDERERSFML_HPP


#include "../Simulation/Simulation.hpp"


#include <functional>


class RendererSFML
{
private: // attributs
public: // ctor/dtor
	RendererSFML();

public: // method(s)
	void	run(Simulation& my_Simulation, std::function<void()> simulation_callback);
};



#endif // D_RENDERERSFML_HPP
