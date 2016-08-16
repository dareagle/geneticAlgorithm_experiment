

#include "Model/Simulation.hpp"
#include "Model/TraceLogger.hpp"


#include <iostream>
#include <cstdlib>

#include <thread>

#include <SFML/Graphics.hpp>
#include <SFML/OpenGL.hpp>







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
	float size = 10.0f;
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

	glLineWidth(5.0f);

	rwindow.draw(vertices, 8, sf::Lines);

	glLineWidth(1.0f);

	///

	if (!car.isAlive())
		return;

	const Car::t_sensors&	sensors = car.getSensors();

	for (Car::t_sensor sensor : sensors)
	{
		sf::Color	LightBlue(128,128,255);
		drawLine(rwindow, sensor.m_line, LightBlue, LightBlue);

		t_vec2f	pos;
		pos.x = sensor.m_line.p1.x + (sensor.m_line.p2.x - sensor.m_line.p1.x) * sensor.m_value;
		pos.y = sensor.m_line.p1.y + (sensor.m_line.p2.y - sensor.m_line.p1.y) * sensor.m_value;

		drawPoint(rwindow, pos, sf::Color::Yellow);
	}
}















int	main()
{
	Simulation	tmp_sim("./res/Map_test.txt");

	const t_lines&	checkpoints = tmp_sim.getCircuit().getCheckpoints();
	const t_lines&	walls = tmp_sim.getCircuit().getWalls();

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(800, 600), "SFML window");

	sf::Vector2f	camera_center;

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


		int iteration_nbr = 3;

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			iteration_nbr = 90;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			iteration_nbr = 1;

		// update
		for (int i = 0; i < iteration_nbr; ++i)
			tmp_sim.update(0.25f);


		{ // camera

			sf::View	view;
			view.reset(sf::FloatRect(0, 0, 800, 600));

			t_vec2f	target;
			
			unsigned int index = 0;
			for (; index < tmp_sim.getCars().size(); ++index)
				if (tmp_sim.getCars()[index].isAlive())
					break;

			if (index < tmp_sim.getCars().size())
				target = tmp_sim.getCars()[index].getPosition();

			sf::Vector2f	diff(target.x-camera_center.x-200, target.y-camera_center.y);

            camera_center.x += diff.x * 0.1;
            camera_center.y += diff.y * 0.1;

			// view.setCenter(400,300);
			if (iteration_nbr <= 3)
				view.setCenter(camera_center);

			window.setView(view);

		} // camera

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

		{ // hud

			// window.setView(window.getDefaultView());

			sf::View	view;
			view.reset(sf::FloatRect(0, 0, 800, 600));
			view.move(-100,-100);
			window.setView(view);

			unsigned int index = 0;
			for (; index < tmp_sim.getCars().size(); ++index)
				if (tmp_sim.getCars()[index].isAlive())
					break;

			if (index < tmp_sim.getCars().size())
			{
				const GeneticAlgorithm::t_genome& genome = tmp_sim.getGenomes()[index];

				unsigned int	ann_topology_length = 4;
				unsigned int	ann_topology[] = {5,4,3,2};

				unsigned int	windex = 0;

				for (unsigned int index = 1; index < ann_topology_length; ++index)
				{
					int prev_layer = ann_topology[index-1];
					int curr_layer = ann_topology[index];

					float prev_dec_y = (1.0f - ((float)prev_layer / 5)) * 185;
					float curr_dec_y = (1.0f - ((float)curr_layer / 5)) * 185;

					for (int pindex = 0; pindex < prev_layer; ++pindex)
						for (int cindex = 0; cindex < curr_layer; ++cindex)
						{
							float curr_x = (index-1) * 75;

							sf::Vector2f	p1;
							p1.x = curr_x;
							p1.y = prev_dec_y+pindex*75;

							sf::Vector2f	p2;
							p2.x = curr_x+75;
							p2.y = curr_dec_y+cindex*75;

							float wvalue = genome.m_weights[windex++];

							float ratio = wvalue * 10;
							if (ratio < 0)  ratio = -ratio;
							if (ratio < 1)  ratio = 1;

							float thickness = ratio;

							float length = sqrtf( (p1.x-p2.x)*(p1.x-p2.x) + (p1.y-p2.y)*(p1.y-p2.y) );

							sf::RectangleShape line(sf::Vector2f(length, thickness));

							line.setOrigin(thickness/2, thickness/2);
							line.setPosition(p1);


							if (wvalue > 0)
								line.setFillColor(sf::Color::Red);
							else
								line.setFillColor(sf::Color::Blue);

							float angle = atan2f(p2.y-p1.y, p2.x-p1.x);

							// line.rotate(angle);
							// line.rotate(angle * 3.14f / 180.0f);
							line.rotate(angle * 180.0f / 3.14f);
							window.draw(line);


							// if (wvalue > 0)
							// 	Renderer.drawLine( curr_x, prev_dec_y+pindex*75, curr_x+75, curr_dec_y+cindex*75, "#ff0000", ratio);
							// else
							// 	Renderer.drawLine( curr_x, prev_dec_y+pindex*75, curr_x+75, curr_dec_y+cindex*75, "#0000ff", ratio);
						}
				}

				for (unsigned int index = 0; index < ann_topology_length; ++index)
				{
					int curr_layer = ann_topology[index];

					float curr_dec_y = (1.0f - ((float)curr_layer / 5)) * 185;

					for (int cindex = 0; cindex < curr_layer; ++cindex)
					{
						int curr_x = (index) * 75;

						// Renderer.drawCircle(curr_x, curr_dec_y+cindex*75, 13, "#ffff00");
						// Renderer.drawCircle(curr_x, curr_dec_y+cindex*75, 10, "#00ff00");

						sf::CircleShape circle;
						circle.setRadius(10);
						circle.setOrigin(10,10);
						circle.setOutlineColor(sf::Color::Yellow);
						circle.setOutlineThickness(3);
						circle.setPosition(curr_x, curr_dec_y+cindex*75);
						window.draw(circle);
					}
				}

			}

		} // hud

		// Update the window
		window.display();
	}

	return EXIT_SUCCESS;
}
