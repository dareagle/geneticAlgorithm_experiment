

#include "Simulation.hpp"


Simulation::Simulation(IPhysicWrapper* pPhysicWrapper)
	: m_pPhysicWrapper(pPhysicWrapper)
{
}

Simulation::~Simulation()
{
}

void	Simulation::initialise(
	const std::vector<float>& arr_left,
	const std::vector<float>& arr_right,
	Circuit::t_geometry_callback callback)
{
	m_Circuit.initialise(m_pPhysicWrapper, arr_left, arr_right, callback);

	std::vector<unsigned int> tmp_hidden;
	tmp_hidden.push_back(10);
	tmp_hidden.push_back(5);
	m_NNTopology.init(15, tmp_hidden, 2);

	m_GenAlgo.init(m_NNTopology);

	unsigned int	num_car = m_GenAlgo.getGenomes().size();

	m_pPhysicWrapper->createVehicles(num_car + 1); // plus one for the best car

	Car::initialise(m_pPhysicWrapper);

	m_Cars.reserve(num_car);
	for (unsigned int ii = 0; ii < num_car; ++ii)
		m_Cars.push_back(Car(ii));

	m_pBest_car = new Car(num_car);
}

void	Simulation::update(float elapsed_time)
{
	m_pPhysicWrapper->step(elapsed_time);

	bool	someone_is_alive = false;

	for (unsigned int i = 0; i < m_Cars.size(); ++i)
	{
		if (!m_Cars[i].isAlive())
			continue;

		someone_is_alive = true;

		m_Cars[i].update( m_GenAlgo.getNNetworks()[i] );
	}

	if (someone_is_alive)
		return;

	// rate genomes
	for (unsigned int i = 0; i < m_Cars.size(); ++i)
	{
		float final_fitness = m_Cars[i].getFitness();

		// this will reward the fastest car once the reaching the end of the circuit
		final_fitness += static_cast<float>(m_Cars[i].getTotalUpdates()) * 0.00001f;
		
		m_GenAlgo.rateGenome(i, final_fitness);
	}

	m_GenAlgo.BreedPopulation();

	if (m_GenAlgo.isAGreatGeneration())
	{
		Car& car = m_Cars[ m_GenAlgo.getAlpha().m_index ];

		m_car_trails.push_back( car.getTrail() );

		while (m_car_trails.size() > 5)
			m_car_trails.pop_front();
	}

	//
	//

	for (Car& car : m_Cars)
		car.reset();
}

void	Simulation::updateBest(float elapsed_time)
{
	if (m_GenAlgo.getAlpha().m_fitness == 0.0f)
		return;

	m_pPhysicWrapper->step(elapsed_time);

	if (m_pBest_car->isAlive())
	{
		NeuralNetwork	tmp_ann(m_NNTopology);

		tmp_ann.setWeights( m_GenAlgo.getAlpha().m_weights );

		m_pBest_car->update( tmp_ann );
	}
	else
	{
		m_pBest_car->reset();
	}
}


