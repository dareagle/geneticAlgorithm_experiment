

#include "Data.hpp"


#include <iostream>



#include <vector>

#include <chrono>
#include <numeric>

#include <glm/gtc/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale, glm::perspective
#include <glm/gtc/type_ptr.hpp> // glm:value_ptr

//
//
// singleton

Data*	Data::m_pInstance = nullptr;

Data::Data()
{

	//
	// camera

	m_vec2_rotations.x = 0.5;
	m_vec2_rotations.y = 0.95;

	m_timeout_camera = 0;
	m_index_targetedCar = -1;

	// camera
	//


	//
	//
	// circuit data

#define D_PUSH_POINT(in_container, in_x, in_y, in_z)	\
	(in_container).push_back( (in_x) );	\
	(in_container).push_back( (in_y) );	\
	(in_container).push_back( (in_z) );

	// centered on 0
	// D_PUSH_POINT(m_arr_skelton_left, -5, -5, 1); D_PUSH_POINT(m_arr_skelton_right, -5,  5, 1);
	D_PUSH_POINT(m_arr_skelton_left, -10, -5, 1); D_PUSH_POINT(m_arr_skelton_right, -10,  5, 1);

	//*/

    // D_PUSH_POINT(m_arr_skelton_left, 0,0,0); D_PUSH_POINT(m_arr_skelton_right, 0,10,0);
    D_PUSH_POINT(m_arr_skelton_left, 90,0,0); D_PUSH_POINT(m_arr_skelton_right, 90,0,0);



    { // infinity loop

        float pente = 7;

        float height = 6;
        float inner_radius = 50;
        float outer_radius = 60;


        D_PUSH_POINT(m_arr_skelton_left, 30,0,+pente); D_PUSH_POINT(m_arr_skelton_right, 30,0,0);

        for (int i = 0; i < 2; ++i)
        {
            D_PUSH_POINT(m_arr_skelton_left, outer_radius,outer_radius,height); D_PUSH_POINT(m_arr_skelton_right, inner_radius,inner_radius,height);
            D_PUSH_POINT(m_arr_skelton_left, -outer_radius,outer_radius,height); D_PUSH_POINT(m_arr_skelton_right, -inner_radius,inner_radius,height);

            D_PUSH_POINT(m_arr_skelton_left, -outer_radius,-outer_radius,height); D_PUSH_POINT(m_arr_skelton_right, -inner_radius,-inner_radius,height);
            D_PUSH_POINT(m_arr_skelton_left, outer_radius,-outer_radius,height); D_PUSH_POINT(m_arr_skelton_right, inner_radius,-inner_radius,height);
        }

        D_PUSH_POINT(m_arr_skelton_left, 40,0,0); D_PUSH_POINT(m_arr_skelton_right, 40,0,+pente*2);

        for (int i = 0; i < 2; ++i)
        {
            D_PUSH_POINT(m_arr_skelton_left, inner_radius,-inner_radius,height); D_PUSH_POINT(m_arr_skelton_right, outer_radius,-outer_radius,height);
            D_PUSH_POINT(m_arr_skelton_left, -inner_radius,-inner_radius,height); D_PUSH_POINT(m_arr_skelton_right, -outer_radius,-outer_radius,height);

            D_PUSH_POINT(m_arr_skelton_left, -inner_radius,inner_radius,height); D_PUSH_POINT(m_arr_skelton_right, -outer_radius,outer_radius,height);
            D_PUSH_POINT(m_arr_skelton_left, inner_radius,inner_radius,height); D_PUSH_POINT(m_arr_skelton_right, outer_radius,outer_radius,height);
        }

        D_PUSH_POINT(m_arr_skelton_left, 30,0,0); D_PUSH_POINT(m_arr_skelton_right, 30,0,-pente);

    } // infinity loop

	/**/

    D_PUSH_POINT(m_arr_skelton_left, 90,0,0); D_PUSH_POINT(m_arr_skelton_right, 90,0,0);

    { // turned looping

		D_PUSH_POINT(m_arr_skelton_left, 40,0,40); D_PUSH_POINT(m_arr_skelton_right, 40,0,40);
		D_PUSH_POINT(m_arr_skelton_left, -40,0,40); D_PUSH_POINT(m_arr_skelton_right, -40,0,40);

		D_PUSH_POINT(m_arr_skelton_left, -40,40,-10); D_PUSH_POINT(m_arr_skelton_right, -40,30,0);
		D_PUSH_POINT(m_arr_skelton_left, 20,40,0); D_PUSH_POINT(m_arr_skelton_right, 0,40,-10);

		D_PUSH_POINT(m_arr_skelton_left, 20,20,0); D_PUSH_POINT(m_arr_skelton_right, 35,30,0);
		D_PUSH_POINT(m_arr_skelton_left, 40,0,0); D_PUSH_POINT(m_arr_skelton_right, 45,0,0);

    } // turned looping

    D_PUSH_POINT(m_arr_skelton_left, 30,0,0); D_PUSH_POINT(m_arr_skelton_right, 30,0,0);

    { // looping

        float r = 60;
        float d = -20;

        D_PUSH_POINT(m_arr_skelton_left, r,0,0); D_PUSH_POINT(m_arr_skelton_right, r,0,0);
        D_PUSH_POINT(m_arr_skelton_left, r,0,r); D_PUSH_POINT(m_arr_skelton_right, r,0,r);
        D_PUSH_POINT(m_arr_skelton_left, -r,d,r); D_PUSH_POINT(m_arr_skelton_right, -r,d,r);
        D_PUSH_POINT(m_arr_skelton_left, -r,d,-r); D_PUSH_POINT(m_arr_skelton_right, -r,d,-r);
        D_PUSH_POINT(m_arr_skelton_left, r,d,-r); D_PUSH_POINT(m_arr_skelton_right, r,d,-r);
        D_PUSH_POINT(m_arr_skelton_left, r,0,0); D_PUSH_POINT(m_arr_skelton_right, r,0,0);

        D_PUSH_POINT(m_arr_skelton_left, 20,0,0); D_PUSH_POINT(m_arr_skelton_right, 20,0,0);

    } // looping


    D_PUSH_POINT(m_arr_skelton_left, 30,0,0); D_PUSH_POINT(m_arr_skelton_right, 30,0,0)



    D_PUSH_POINT(m_arr_skelton_left, 30,0,0); D_PUSH_POINT(m_arr_skelton_right, 30,0,0);


    { // vertical corner

        D_PUSH_POINT(m_arr_skelton_left, 30,0,0); D_PUSH_POINT(m_arr_skelton_right, 30,0,0);

        D_PUSH_POINT(m_arr_skelton_left, 30,0,30); D_PUSH_POINT(m_arr_skelton_right, 30,0,30);

        D_PUSH_POINT(m_arr_skelton_left, 10,0,40); D_PUSH_POINT(m_arr_skelton_right, 10,0,40);

        D_PUSH_POINT(m_arr_skelton_left, 10,30,30); D_PUSH_POINT(m_arr_skelton_right, 10,20,15);
        D_PUSH_POINT(m_arr_skelton_left, -10,30,-30); D_PUSH_POINT(m_arr_skelton_right, -10,20,-15);

        D_PUSH_POINT(m_arr_skelton_left, -10,0,-40); D_PUSH_POINT(m_arr_skelton_right, -10,0,-40);

        D_PUSH_POINT(m_arr_skelton_left, -30,0,-30); D_PUSH_POINT(m_arr_skelton_right, -30,0,-30);

        D_PUSH_POINT(m_arr_skelton_left, -30,0,0); D_PUSH_POINT(m_arr_skelton_right, -30,0,0);

    } // vertical corner

    D_PUSH_POINT(m_arr_skelton_left, -30,0,0); D_PUSH_POINT(m_arr_skelton_right, -30,0,0)

	//*/

	/**

	// forward (x)
	D_PUSH_POINT(m_arr_skelton_left, 20,  0, 1); D_PUSH_POINT(m_arr_skelton_right, 20,  0, 1);
	D_PUSH_POINT(m_arr_skelton_left, 20,  0, 1); D_PUSH_POINT(m_arr_skelton_right, 20,  0, 1);


	/**

	//
	// first curve

	D_PUSH_POINT(m_arr_skelton_left, 20,-15, 1); D_PUSH_POINT(m_arr_skelton_right, 20,-15, 1);
	D_PUSH_POINT(m_arr_skelton_left, 20, 15, 1); D_PUSH_POINT(m_arr_skelton_right, 20, 15, 1);
	D_PUSH_POINT(m_arr_skelton_left, 20, 15, 1); D_PUSH_POINT(m_arr_skelton_right, 20, 15, 1);
	D_PUSH_POINT(m_arr_skelton_left, 20,-15, 1); D_PUSH_POINT(m_arr_skelton_right, 20,-15, 1);

	// first curve
	//

	// forward (x)
	D_PUSH_POINT(m_arr_skelton_left, 20,  0, 1); D_PUSH_POINT(m_arr_skelton_right, 20,  0, 1);
	D_PUSH_POINT(m_arr_skelton_left, 20,  0, 1); D_PUSH_POINT(m_arr_skelton_right, 20,  0, 1);

	//*/

	/**

	//
	// start of the first "maze"

	// turn right
	D_PUSH_POINT(m_arr_skelton_left,  5, -5, 1); D_PUSH_POINT(m_arr_skelton_right, 15,-15, 1);

	// forward (-y)
	D_PUSH_POINT(m_arr_skelton_left,  0,-10, 1); D_PUSH_POINT(m_arr_skelton_right,  0,-10, 1);
	D_PUSH_POINT(m_arr_skelton_left,  0,-10, 1); D_PUSH_POINT(m_arr_skelton_right,  0,-10, 1);

	// turn left
	D_PUSH_POINT(m_arr_skelton_left, 15,-15, 1); D_PUSH_POINT(m_arr_skelton_right,  5, -5, 1);

	// forward (x)
	D_PUSH_POINT(m_arr_skelton_left, 20,  0, 1); D_PUSH_POINT(m_arr_skelton_right, 20,  0, 1);

	// turn left
	D_PUSH_POINT(m_arr_skelton_left, 15, 15, 1); D_PUSH_POINT(m_arr_skelton_right,  5,  5, 1);

	// forward (y)
	D_PUSH_POINT(m_arr_skelton_left,  0, 30, 1); D_PUSH_POINT(m_arr_skelton_right,  0, 30, 1);
	D_PUSH_POINT(m_arr_skelton_left,  0, 30, 1); D_PUSH_POINT(m_arr_skelton_right,  0, 30, 1);

	// turn right
	D_PUSH_POINT(m_arr_skelton_left,  5,  5, 1); D_PUSH_POINT(m_arr_skelton_right, 15, 15, 1);

	// forward (x)
	D_PUSH_POINT(m_arr_skelton_left, 20,  0, 1); D_PUSH_POINT(m_arr_skelton_right, 20,  0, 1);

	// turn right
	D_PUSH_POINT(m_arr_skelton_left,  5, -5, 1); D_PUSH_POINT(m_arr_skelton_right, 15,-15, 1);

	// forward (-y)
	D_PUSH_POINT(m_arr_skelton_left,  0,-10, 1); D_PUSH_POINT(m_arr_skelton_right,  0,-10, 1);
	D_PUSH_POINT(m_arr_skelton_left,  0,-10, 1); D_PUSH_POINT(m_arr_skelton_right,  0,-10, 1);

	// turn left
	D_PUSH_POINT(m_arr_skelton_left, 15,-15, 1); D_PUSH_POINT(m_arr_skelton_right,  5, -5, 1);

	// forward (x)
	D_PUSH_POINT(m_arr_skelton_left, 20,  0, 1); D_PUSH_POINT(m_arr_skelton_right, 20,  0, 1);
	D_PUSH_POINT(m_arr_skelton_left, 20,  0, 1); D_PUSH_POINT(m_arr_skelton_right, 20,  0, 1);

	// start of the first "maze"
	//

	//*/

    /**

	//
	// looping

	D_PUSH_POINT(m_arr_skelton_left,+40,  0,  1); D_PUSH_POINT(m_arr_skelton_right,+40,  0,  1);
	D_PUSH_POINT(m_arr_skelton_left,+40,  0, 40); D_PUSH_POINT(m_arr_skelton_right,+40,  0, 40);
	D_PUSH_POINT(m_arr_skelton_left,-40, 10, 40); D_PUSH_POINT(m_arr_skelton_right,-40, 10, 40);
	D_PUSH_POINT(m_arr_skelton_left,-40, 10,-40); D_PUSH_POINT(m_arr_skelton_right,-40, 10,-40);
	D_PUSH_POINT(m_arr_skelton_left,+40, 10,-30); D_PUSH_POINT(m_arr_skelton_right,+40, 10,-30);
	D_PUSH_POINT(m_arr_skelton_left,+40,  0,  0); D_PUSH_POINT(m_arr_skelton_right,+40,  0,  0);

	// looping
	//

    //*/

	/**

	{ // road wheels

        float pente = 7;

        float height = 6;
        float inner_radius = 50;
        float outer_radius = 60;

		D_PUSH_POINT(m_arr_skelton_left, 30,  0, pente); D_PUSH_POINT(m_arr_skelton_right, 30,  0, 0);

		//

		for (int ii = 0; ii < 2; ++ii)
		{
		    D_PUSH_POINT(m_arr_skelton_left, outer_radius,outer_radius,height); D_PUSH_POINT(m_arr_skelton_right, inner_radius,inner_radius,height);
		    D_PUSH_POINT(m_arr_skelton_left, -outer_radius,outer_radius,height); D_PUSH_POINT(m_arr_skelton_right, -inner_radius,inner_radius,height);

		    D_PUSH_POINT(m_arr_skelton_left, -outer_radius,-outer_radius,height); D_PUSH_POINT(m_arr_skelton_right, -inner_radius,-inner_radius,height);
		    D_PUSH_POINT(m_arr_skelton_left, outer_radius,-outer_radius,height); D_PUSH_POINT(m_arr_skelton_right, inner_radius,-inner_radius,height);
		}

	    //

        D_PUSH_POINT(m_arr_skelton_left, 40,0,0); D_PUSH_POINT(m_arr_skelton_right, 40,0, pente * 2);

        //

		for (int ii = 0; ii < 2; ++ii)
		{
	        D_PUSH_POINT(m_arr_skelton_left, inner_radius,-inner_radius,height); D_PUSH_POINT(m_arr_skelton_right, outer_radius,-outer_radius,height);
	        D_PUSH_POINT(m_arr_skelton_left, -inner_radius,-inner_radius,height); D_PUSH_POINT(m_arr_skelton_right, -outer_radius,-outer_radius,height);

	        D_PUSH_POINT(m_arr_skelton_left, -inner_radius,inner_radius,height); D_PUSH_POINT(m_arr_skelton_right, -outer_radius,outer_radius,height);
	        D_PUSH_POINT(m_arr_skelton_left, inner_radius,inner_radius,height); D_PUSH_POINT(m_arr_skelton_right, outer_radius,outer_radius,height);
	    }

        //

        D_PUSH_POINT(m_arr_skelton_left, 30,0,0); D_PUSH_POINT(m_arr_skelton_right, 30,0,-pente);

    } // road wheels

    //*/

#undef D_PUSH_POINT

	//
	//
	// concatenate coordinates

	std::vector<float> points1_ex;
	std::vector<float> points2_ex;

	for (int index = 0; index < m_arr_skelton_left.size(); index += 3)
	{
		float x1 = m_arr_skelton_left[index+0];
		float y1 = m_arr_skelton_left[index+1];
		float z1 = m_arr_skelton_left[index+2];

		float x2 = m_arr_skelton_right[index+0];
		float y2 = m_arr_skelton_right[index+1];
		float z2 = m_arr_skelton_right[index+2];

	    if (index > 0)
	    {
	        // concatenate the point

	        x1 += points1_ex[(index-3)+0];
	        y1 += points1_ex[(index-3)+1];
	        z1 += points1_ex[(index-3)+2];

	        x2 += points2_ex[(index-3)+0];
	        y2 += points2_ex[(index-3)+1];
	        z2 += points2_ex[(index-3)+2];
	    }

	    points1_ex.push_back(x1);
	    points1_ex.push_back(y1);
	    points1_ex.push_back(z1);

	    points2_ex.push_back(x2);
	    points2_ex.push_back(y2);
	    points2_ex.push_back(z2);
	}

	// circuit data
	//
	//


	//
	//
	// experimental

#define D_PUSH_VERTEX(container, v1,v2,v3, c1,c2,c3)	\
	(container).push_back(v1);	\
	(container).push_back(v2);	\
	(container).push_back(v3);	\
	(container).push_back(c1);	\
	(container).push_back(c2);	\
	(container).push_back(c3);	\

	m_ShaderColor.initialise();
	GeometryColor::initialise(&m_ShaderColor);

	// {
	// 	std::vector<float>	buffer;
	// 	buffer.reserve(400);
    //
	// 	// std::vector<float>	buffer2;
	// 	// buffer2.reserve(400);
    //
    //     float side_x = 1;
    //     float side_y = 2;
    //     float side_z = 0.5;
    //
    //     float vertices[] = {
    //          side_x,  side_y,  side_z, // 0
    //         -side_x,  side_y,  side_z,
    //          side_x, -side_y,  side_z,
    //         -side_x, -side_y,  side_z,
    //
    //          side_x,  side_y, -side_z, // 4
    //         -side_x,  side_y, -side_z,
    //          side_x, -side_y, -side_z,
    //         -side_x, -side_y, -side_z
    //     };
    //
    //     //
    //
    //     int indices[24] = {
    //         0,1,  1,3,  3,2,  2,0,
    //         4,5,  5,7,  7,6,  6,4,
    //         0,4,  1,5,  3,7,  2,6
    //     };
    //
    //     //
    //
    //     for (int i = 0; i < 24; ++i)
    //     {
    //         int curr_index = indices[i] * 3;
    //
    //         D_PUSH_VERTEX(buffer, vertices[curr_index+0],vertices[curr_index+1],vertices[curr_index+2], 1,0,0);
    //         // D_PUSH_VERTEX(buffer2, vertices[curr_index+0],vertices[curr_index+1],vertices[curr_index+2], 1,1,1);
    //     }
    //
	// 	m_GeometryColor_chassis.initialise();
	// 	m_GeometryColor_chassis.update(&buffer[0], buffer.size() * sizeof(float));
    //
	// 	// m_GeometryColor_chassis_dead.initialise();
	// 	// m_GeometryColor_chassis_dead.update(&buffer2[0], buffer2.size() * sizeof(float));
	// }
    //
	// {
	// 	std::vector<float>	buffer;
	// 	buffer.reserve(400);
    //
	// 	int quality = 16;
	// 	float radius = 0.5f;
	// 	float length = 0.5f;
    //
	// 	std::vector<float>	vertices;
    //
    //     for (int i = 0; i < quality; ++i)
    //     {
    //         float ratio = static_cast<float>(i) / quality;
    //         vertices.push_back(-length / 2);
    //         vertices.push_back(radius * cosf(M_PI * 2 * ratio));
    //         vertices.push_back(radius * sinf(M_PI * 2 * ratio));
    //     }
    //
    //     for (int i = 0; i < quality; ++i)
    //     {
    //         float ratio = static_cast<float>(i) / quality;
    //     	vertices.push_back(+length/2);
    //         vertices.push_back(radius * cosf(M_PI * 2 * ratio));
    //     	vertices.push_back(radius * sinf(M_PI * 2 * ratio));
    //     }
    //
    //     vertices.push_back(0);
    //     vertices.push_back(-length/2);
    //     vertices.push_back(0);
    //
    //     vertices.push_back(0);
    //     vertices.push_back(+length/2);
    //     vertices.push_back(0);
    //
    //     //
    //
	// 	std::vector<int>	indices;
    //
    //     // wheel side 1
    //     for (int i = 0; i < quality; ++i)
    //     {
    //         indices.push_back(i);
    //         indices.push_back((i + 1) % quality);
    //     }
    //
    //     // wheel side 2
    //     for (int i = 0; i < quality; ++i)
    //     {
    //         indices.push_back(quality + i);
    //         indices.push_back(quality + (i + 1) % quality);
    //     }
    //
    //     // inter wheels
    //     // for (int i = 0; i < quality; ++i)
    //     // {
    //     //     indices.push_back(i);
    //     //     indices.push_back(quality+i);
    //     // }
    //     indices.push_back(0);
    //     indices.push_back(quality);
    //
    //     //
    //
    //     for (int i = 0; i < indices.size(); ++i)
    //     {
    //         int curr_index = indices[i] * 3;
    //
    //         D_PUSH_VERTEX(buffer, vertices[curr_index+0],vertices[curr_index+1],vertices[curr_index+2], 1,1,0);
    //     }
    //
	// 	m_GeometryColor_wheel.initialise();
	// 	m_GeometryColor_wheel.update(&buffer[0], buffer.size() * sizeof(float));
	// }

	{
		std::vector<float>	buffer;
		buffer.reserve(400);

        for (int index = 0; index < points1_ex.size(); index += 3)
        {
            D_PUSH_VERTEX(buffer, points1_ex[index+0],points1_ex[index+1],points1_ex[index+2], 0.5,0.5,0.5);
            D_PUSH_VERTEX(buffer, points2_ex[index+0],points2_ex[index+1],points2_ex[index+2], 0.5,0.5,0.5);

            D_PUSH_VERTEX(buffer, points1_ex[index+0],points1_ex[index+1],0, 0.5,0.5,0.5);
            D_PUSH_VERTEX(buffer, points2_ex[index+0],points2_ex[index+1],0, 0.5,0.5,0.5);

            D_PUSH_VERTEX(buffer, points1_ex[index+0],points1_ex[index+1],points1_ex[index+2], 0.5,0.5,0.5);
            D_PUSH_VERTEX(buffer, points1_ex[index+0],points1_ex[index+1],0, 0.5,0.5,0.5);

            D_PUSH_VERTEX(buffer, points2_ex[index+0],points2_ex[index+1],points2_ex[index+2], 0.5,0.5,0.5);
            D_PUSH_VERTEX(buffer, points2_ex[index+0],points2_ex[index+1],0, 0.5,0.5,0.5);

	        if (index >= 3)
	        {
	            D_PUSH_VERTEX(buffer, points1_ex[index-3+0],points1_ex[index-3+1],points1_ex[index-3+2], 0.5,0.5,0.5);
	            D_PUSH_VERTEX(buffer, points1_ex[index+0],points1_ex[index+1],points1_ex[index+2], 0.5,0.5,0.5);
	            D_PUSH_VERTEX(buffer, points2_ex[index-3+0],points2_ex[index-3+1],points2_ex[index-3+2], 0.5,0.5,0.5);
	            D_PUSH_VERTEX(buffer, points2_ex[index+0],points2_ex[index+1],points2_ex[index+2], 0.5,0.5,0.5);

	            D_PUSH_VERTEX(buffer, points1_ex[index-3+0],points1_ex[index-3+1],0, 0.5,0.5,0.5);
	            D_PUSH_VERTEX(buffer, points1_ex[index+0],points1_ex[index+1],0, 0.5,0.5,0.5);
	            D_PUSH_VERTEX(buffer, points2_ex[index-3+0],points2_ex[index-3+1],0, 0.5,0.5,0.5);
	            D_PUSH_VERTEX(buffer, points2_ex[index+0],points2_ex[index+1],0, 0.5,0.5,0.5);
	        }
        }

		m_GeometryColor_circuit_skelton.initialise();
		m_GeometryColor_circuit_skelton.update(&buffer[0], buffer.size() * sizeof(float));
	}

	m_StackRenderer.create();

	{
		m_graphic.m_InstShader.initialise();
		m_graphic.m_InstGeometry_chassis.initialise(&m_graphic.m_InstShader);
		m_graphic.m_InstGeometry_wheel.initialise(&m_graphic.m_InstShader);


		std::vector<float>  arr_buffer;

		// #define D_PUSH_VERTEX(x,y,z, r,g,b) \
		// 	arr_buffer.push_back(x); \
		// 	arr_buffer.push_back(y); \
		// 	arr_buffer.push_back(z); \
		// 	arr_buffer.push_back(r); \
		// 	arr_buffer.push_back(g); \
		// 	arr_buffer.push_back(b);
        //
		// 	D_PUSH_VERTEX(0,0,0, 1,0,0); D_PUSH_VERTEX(20,0,0, 1,0,0);
		// 	D_PUSH_VERTEX(0,0,0, 0,1,0); D_PUSH_VERTEX(0,20,0, 0,1,0);
		// 	D_PUSH_VERTEX(0,0,0, 0,0,1); D_PUSH_VERTEX(0,0,20, 0,0,1);
        //
		// #undef D_PUSH_VERTEX
        //
		// m_graphic.m_InstGeometry.update(&arr_buffer[0], arr_buffer.size() * sizeof(float));

		{
			std::vector<float>	buffer_chassis;
			buffer_chassis.reserve(400);

	        float side_x = 1;
	        float side_y = 2;
	        float side_z = 0.5;

	        float vertices[] = {
	             side_x,  side_y,  side_z, // 0
	            -side_x,  side_y,  side_z,
	             side_x, -side_y,  side_z,
	            -side_x, -side_y,  side_z,

	             side_x,  side_y, -side_z, // 4
	            -side_x,  side_y, -side_z,
	             side_x, -side_y, -side_z,
	            -side_x, -side_y, -side_z
	        };

	        //

	        int indices[24] = {
	            0,1,  1,3,  3,2,  2,0,
	            4,5,  5,7,  7,6,  6,4,
	            0,4,  1,5,  3,7,  2,6
	        };

	        //

	        for (int i = 0; i < 24; ++i)
	        {
	            int curr_index = indices[i] * 3;

	            D_PUSH_VERTEX(buffer_chassis, vertices[curr_index+0],vertices[curr_index+1],vertices[curr_index+2], 1,0,0);
	        }

			// m_GeometryColor_chassis.initialise();
			// m_GeometryColor_chassis.update(&buffer_chassis[0], buffer_chassis.size() * sizeof(float));

			m_graphic.m_InstGeometry_chassis.update(&buffer_chassis[0], buffer_chassis.size() * sizeof(float));
		}

		// arr_buffer.clear();
		// for (int xx = 0; xx < 20; ++xx)
		// for (int yy = 0; yy < 20; ++yy)
		// {
		// 	// arr_buffer.push_back(xx*0.5f);
		// 	// arr_buffer.push_back(yy*0.5f);
        //
		// 	glm::mat4 Transform = glm::mat4(1.0f);
		// 	Transform = glm::translate(Transform, glm::vec3(xx*1.5f, yy*1.5f, 2.0f));
		// 	float* pMatrix = glm::value_ptr(Transform);
        //
		// 	for (int ii = 0; ii < 16; ++ii)
		// 		arr_buffer.push_back(pMatrix[ii]);
		// }
		// m_graphic.m_InstGeometry_chassis.update2(&arr_buffer[0], arr_buffer.size() * sizeof(float));
		// m_graphic.m_InstGeometry_wheel.update2(&arr_buffer[0], arr_buffer.size() * sizeof(float));











		{
			std::vector<float>	buffer_wheel;
			buffer_wheel.reserve(400);

			int quality = 16;
			float radius = 0.5f;
			float length = 0.5f;

			std::vector<float>	vertices;

	        for (int i = 0; i < quality; ++i)
	        {
	            float ratio = static_cast<float>(i) / quality;
	            vertices.push_back(-length / 2);
	            vertices.push_back(radius * cosf(M_PI * 2 * ratio));
	            vertices.push_back(radius * sinf(M_PI * 2 * ratio));
	        }

	        for (int i = 0; i < quality; ++i)
	        {
	            float ratio = static_cast<float>(i) / quality;
	        	vertices.push_back(+length/2);
	            vertices.push_back(radius * cosf(M_PI * 2 * ratio));
	        	vertices.push_back(radius * sinf(M_PI * 2 * ratio));
	        }

	        vertices.push_back(0);
	        vertices.push_back(-length/2);
	        vertices.push_back(0);

	        vertices.push_back(0);
	        vertices.push_back(+length/2);
	        vertices.push_back(0);

	        //

			std::vector<int>	indices;

	        // wheel side 1
	        for (int i = 0; i < quality; ++i)
	        {
	            indices.push_back(i);
	            indices.push_back((i + 1) % quality);
	        }

	        // wheel side 2
	        for (int i = 0; i < quality; ++i)
	        {
	            indices.push_back(quality + i);
	            indices.push_back(quality + (i + 1) % quality);
	        }

	        // inter wheels
	        // for (int i = 0; i < quality; ++i)
	        // {
	        //     indices.push_back(i);
	        //     indices.push_back(quality+i);
	        // }
	        indices.push_back(0);
	        indices.push_back(quality);

	        //

	        for (int i = 0; i < indices.size(); ++i)
	        {
	            int curr_index = indices[i] * 3;

	            D_PUSH_VERTEX(buffer_wheel, vertices[curr_index+0],vertices[curr_index+1],vertices[curr_index+2], 1,1,0);
	        }

			// m_GeometryColor_wheel.initialise();
			// m_GeometryColor_wheel.update(&buffer_wheel[0], buffer_wheel.size() * sizeof(float));
			m_graphic.m_InstGeometry_wheel.update(&buffer_wheel[0], buffer_wheel.size() * sizeof(float));
		}













	}

#undef D_PUSH_VERTEX

	// /experimental
	//
	//

	// initialisePhysic();


	//
	//
	//
	// world

	// m_DebugDraw.setDebugMode(
	// 	BulletDebugDraw::DBG_DrawWireframe |
	// 	BulletDebugDraw::DBG_DrawContactPoints |
	// 	BulletDebugDraw::DBG_NoHelpText |
	// 	BulletDebugDraw::DBG_DrawConstraints |
	// 	BulletDebugDraw::DBG_DrawConstraintLimits |
	// 	BulletDebugDraw::DBG_FastWireframe
	// );
	// m_PhysicWorld.setDebugDrawer(&m_DebugDraw);

	//
	//
	// simulation

	m_pPhysicWrapper = new PhysicWrapper();

	m_pSimulation = new Simulation(m_pPhysicWrapper);

	m_simualtion_step = 1;
}

Data::~Data()
{
	m_StackRenderer.destroy();
}

void	Data::initialise()
{
	std::vector<float>	buffer;
	buffer.reserve(2048);

	std::vector<float>	buffer_walls;
	buffer_walls.reserve(2048*2);

    int physic_index = 1;

	m_pSimulation->initialise(
		// m_arr_skelton_left,
		// m_arr_skelton_right,
		// [&](std::vector<float>& buf, std::vector<int>& indices, bool ground) -> void
		// {
		// 	// std::vector<float>&	current_buffer = (ground ? buffer : buffer_walls);

		// 	// for (int ii = 0; ii < indices.size(); ++ii)
		// 	// {
		// 	// 	int index = indices[ii] * 3;

		// 	// 	current_buffer.push_back( buf[ index+0 ] );
		// 	// 	current_buffer.push_back( buf[ index+1 ] );
		// 	// 	current_buffer.push_back( buf[ index+2 ] );

		// 	// 	if (ground)
		// 	// 	{
		// 	// 		if (index == 0 ||
		// 	// 			index == 3)
		// 	// 		{
		// 	// 			current_buffer.push_back(1.0f);
		// 	// 			current_buffer.push_back(1.0f);
		// 	// 			current_buffer.push_back(1.0f);
		// 	// 		}
		// 	// 		else
		// 	// 		{
		// 	// 			current_buffer.push_back(0.0f);
		// 	// 			current_buffer.push_back(0.0f);
		// 	// 			current_buffer.push_back(1.0f);
		// 	// 		}
		// 	// 	}
		// 	// 	else
		// 	// 	{
		// 	// 		current_buffer.push_back(0.6f);
		// 	// 		current_buffer.push_back(0.6f);
		// 	// 		current_buffer.push_back(0.6f);
		// 	// 	}
		// 	// }
		// },
        "assets/circuits/default.txt",
        [&](const std::vector<float>& buf, const std::vector<int>& indices) -> void
        {
            m_pPhysicWrapper->createGround(buf, indices, physic_index++);

            for (int ii = 0; ii < indices.size(); ++ii)
            {
                int index = indices[ii] * 3;

                buffer.push_back( buf[ index+0 ] );
                buffer.push_back( buf[ index+1 ] );
                buffer.push_back( buf[ index+2 ] );

                if (index == 0 ||
                    index == 3)
                {
                    buffer.push_back(1.0f);
                    buffer.push_back(1.0f);
                    buffer.push_back(1.0f);
                }
                else
                {
                    buffer.push_back(0.0f);
                    buffer.push_back(0.0f);
                    buffer.push_back(1.0f);
                }
            }
        },
        [&](const std::vector<float>& buf, const std::vector<int>& indices) -> void
        {
            m_pPhysicWrapper->createGhostWall(buf, indices);

            for (int ii = 0; ii < indices.size(); ++ii)
            {
                int index = indices[ii] * 3;

                buffer_walls.push_back( buf[ index+0 ] );
                buffer_walls.push_back( buf[ index+1 ] );
                buffer_walls.push_back( buf[ index+2 ] );
                buffer_walls.push_back(0.6f);
                buffer_walls.push_back(0.6f);
                buffer_walls.push_back(0.6f);
            }
        }

	);

	m_GeometryColor_circuit_ground.initialise();
	m_GeometryColor_circuit_ground.update(&buffer[0], buffer.size() * sizeof(float));

	m_GeometryColor_circuit_walls.initialise();
	m_GeometryColor_circuit_walls.update(&buffer_walls[0], buffer_walls.size() * sizeof(float));
}

//

void	Data::create()
{
	if (!m_pInstance)
	{
		m_pInstance = new Data();
		m_pInstance->initialise();
	}
}

void	Data::destroy()
{
	delete m_pInstance, m_pInstance = nullptr;
}

Data*	Data::get()
{
	return m_pInstance;
}

// singleton
//
//
