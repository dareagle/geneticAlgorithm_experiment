

#include "Model/Simulation.hpp"
#include "Model/TraceLogger.hpp"


#include <iostream>
#include <cstdlib>

#include <thread>

#include <SFML/Graphics.hpp>







void drawLine(sf::RenderWindow& rwindow, const t_line& line, const sf::Color& color1, const sf::Color& color2)
{
	const t_vec2f& p1 = line.p1;
	const t_vec2f& p2 = line.p2;

	// define a 100x100 square, red, with a 10x10 texture mapped on it
	sf::Vertex vertices[] = {
		sf::Vertex( sf::Vector2f(p1.x, p1.y), color1 ),
		sf::Vertex( sf::Vector2f(p2.x, p2.y), color2 )
	};

	// draw it
	rwindow.draw(vertices, 2, sf::Lines);
}

void drawLines(sf::RenderWindow& rwindow, const t_lines& lines, const sf::Color& color1, const sf::Color& color2)
{
	static std::vector<sf::Vertex>	s_vertices;

	s_vertices.clear();
	for (unsigned int i = 0; i < lines.size(); ++i)
	{
		s_vertices.push_back( sf::Vertex( sf::Vector2f(lines[i].p1.x, lines[i].p1.y), color1 ) );
		s_vertices.push_back( sf::Vertex( sf::Vector2f(lines[i].p2.x, lines[i].p2.y), color2 ) );
	}

	rwindow.draw(&(s_vertices[0]), s_vertices.size(), sf::Lines);
}

void drawPoint(sf::RenderWindow& rwindow, const t_vec2f& point, const sf::Color& color)
{
	float size = 25.0f;
	sf::Vertex vertices[] = {
		sf::Vertex(sf::Vector2f(point.x - size, point.y - size), color),
		sf::Vertex(sf::Vector2f(point.x + size, point.y + size), color),
		sf::Vertex(sf::Vector2f(point.x + size, point.y - size), color),
		sf::Vertex(sf::Vector2f(point.x - size, point.y + size), color)
	};

	rwindow.draw(vertices, 4, sf::Lines);
}

void drawCar(sf::RenderWindow& rwindow, const Car& car, const sf::Color& color)
{
	const t_vec2f& position = car.getPosition();
	float angle = car.getAngle();

	float size_h = 25.0f;
	float size_v = 12.5f;

	t_vec2f	positions[4] = {
		t_vec2f(position.x - size_h, position.y - size_v),
		t_vec2f(position.x + size_h, position.y - size_v),
		t_vec2f(position.x + size_h, position.y + size_v),
		t_vec2f(position.x - size_h, position.y + size_v)
	};

	for (int i = 0; i < 4; ++i)
		positions[i] = rotateVec2(positions[i], position, angle);

	sf::Color col = (car.isAlive() ? color : sf::Color::Red);

	sf::Vertex p1(sf::Vector2f(positions[0].x, positions[0].y), col);
	sf::Vertex p2(sf::Vector2f(positions[1].x, positions[1].y), col);
	sf::Vertex p3(sf::Vector2f(positions[2].x, positions[2].y), col);
	sf::Vertex p4(sf::Vector2f(positions[3].x, positions[3].y), col);

	sf::Vertex vertices[] = { p1,p2, p2,p3, p3,p4, p4,p1 };

	rwindow.draw(vertices, 8, sf::Lines);

	///

	if (!car.isAlive())
		return;

	const Car::t_sensors&	sensors = car.getSensors();

	for (Car::t_sensor sensor : sensors)
	{
		drawLine(rwindow, sensor.m_line, sf::Color::Red, sf::Color::Blue);

		t_vec2f	pos;
		pos.x = sensor.m_line.p1.x + (sensor.m_line.p2.x - sensor.m_line.p1.x) * sensor.m_value;
		pos.y = sensor.m_line.p1.y + (sensor.m_line.p2.y - sensor.m_line.p1.y) * sensor.m_value;

		drawPoint(rwindow, pos, sf::Color::Blue);
	}
}















int	main()
{
	Simulation	tmp_sim("./res/Map_test.txt");

	const t_lines&	checkpoints = tmp_sim.getCircuit().getCheckpoints();
	const t_lines&	walls = tmp_sim.getCircuit().getWalls();

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

	// Start the game loop
	while (window.isOpen())
	{
		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window: exit
			if (event.type == sf::Event::Closed)
				window.close();

			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape))
				window.close();

			if ((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::P))
			{
				D_MYLOG("getCurrentGeneration=" << tmp_sim.getCurrentGeneration());
				D_MYLOG("getBestFitness=" << tmp_sim.getBestFitness());

				for (unsigned int i = 0; i < tmp_sim.getCars().size(); ++i)
				{
					const Car& c = tmp_sim.getCars()[i];

					const t_vec2f& pos = c.getPosition();
					float angle = c.getAngle();

					D_MYLOG("[" << i << "] pos=" << pos.x << "/" << pos.y << ", angle=" << angle << ", alive=" << c.isAlive() << ", id=" << tmp_sim.getGenomes()[i].m_id);
				}
			}
		}


		int iteration_nbr = 5;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			iteration_nbr = 90;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			iteration_nbr = 1;

		// update
		for (int i = 0; i < iteration_nbr; ++i)
			tmp_sim.update(0.125f);

		// go faster if Up is pressed (debug <3)
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
		{
			// for (int i = 0; i < 90; ++i)
			// 	tmp_sim.update(0.125f);
		}

		// Clear screen
		window.clear();

		// render starting point
		drawPoint(window, tmp_sim.getCircuit().getStartingPositon(), sf::Color::Blue);

		// render circuit
		drawLines(window, checkpoints, sf::Color(255,255,255), sf::Color(128,128,128));
		drawLines(window, walls, sf::Color::Blue, sf::Color(128,128,128));

		// render cars
		for (auto& elem :  tmp_sim.getCars())
			drawCar(window, elem, sf::Color::Green);

		// render trails
		const std::vector< std::vector<t_line> >&	trails = tmp_sim.getTrails();
		if (!trails.empty())
		{
			for (unsigned int i = 0; i < trails.size() - 1; ++i)
			{
				sf::Color	color = sf::Color::Black;
				color.g = 128 + ((float)i / trails.size()) * 128;
				drawLines(window, trails[i], color, color);
			}

			drawLines(window, trails.back(), sf::Color::White, sf::Color::White);
		}

		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
}
