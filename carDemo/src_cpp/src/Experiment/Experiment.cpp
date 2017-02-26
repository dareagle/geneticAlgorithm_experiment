

#include "Experiment.hpp"



#include "Simulation/Simulation.hpp"
#include "Simulation/TraceLogger.hpp"

#include "Renderer/RendererSFML.hpp"


#include <iostream>
#include <cstdlib>

#include <thread>










Experiment::Experiment()
{
}

void	Experiment::run()
{
	Simulation	my_Simulation("./res/Map_test.txt");

	RendererSFML	my_RendererSFML;

	my_RendererSFML.run(my_Simulation, [&]()
	{
		my_Simulation.update(0.25f);
	});
}


