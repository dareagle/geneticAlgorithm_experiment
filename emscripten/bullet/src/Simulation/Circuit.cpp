

#include "Circuit.hpp"


#include "Utility/BSpline.hpp"


#include <cmath> // <= sqrtf
#include <iostream>



void calculate_normal(float* v0, float* v1, float* v2, float* normal)
{
    float px = v1[0] - v0[0];
    float py = v1[1] - v0[1];
    float pz = v1[2] - v0[2];

    float qx = v2[0] - v0[0];
    float qy = v2[1] - v0[1];
    float qz = v2[2] - v0[2];

    normal[0] = (py * qz) - (pz * qy);
    normal[1] = (pz * qx) - (px * qz);
    normal[2] = (px * qy) - (py * qx);

    // std::cout << "v0=" << v0[0] << "/" << v0[1] << "/" << v0[2] << std::endl;
    // std::cout << "v1=" << v1[0] << "/" << v1[1] << "/" << v1[2] << std::endl;
    // std::cout << "v2=" << v2[0] << "/" << v2[1] << "/" << v2[2] << std::endl;

    float tmp_len = sqrtf(normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2]);

    // std::cout << "tmp_len=" << tmp_len << std::endl;

    normal[0] /= tmp_len;
    normal[1] /= tmp_len;
    normal[2] /= tmp_len;

    // return normal;

} // function calculate_normal(v0, v1, v2)



void	Circuit::initialise(
	IPhysicWrapper* pPhysicWrapper,
	const std::vector<float>& arr_left,
	const std::vector<float>& arr_right,
	Circuit::t_geometry_callback callback)
{

	// TODO : check here if the array size are equal

	//
	//
	// concatenate coordinates

	std::vector<float> points1_ex;
	std::vector<float> points2_ex;

	for (int index = 0; index < arr_left.size(); index += 3)
	{
		float x1 = arr_left[index+0];
		float y1 = arr_left[index+1];
		float z1 = arr_left[index+2];

		float x2 = arr_right[index+0];
		float y2 = arr_right[index+1];
		float z2 = arr_right[index+2];

	    if (index > 0)
	    {
	        // concatenate the point

	        float prev_x1 = points1_ex[(index-3)+0];
	        float prev_y1 = points1_ex[(index-3)+1];
	        float prev_z1 = points1_ex[(index-3)+2];

	        float prev_x2 = points2_ex[(index-3)+0];
	        float prev_y2 = points2_ex[(index-3)+1];
	        float prev_z2 = points2_ex[(index-3)+2];

	        x1 += prev_x1;
	        y1 += prev_y1;
	        z1 += prev_z1;

	        x2 += prev_x2;
	        y2 += prev_y2;
	        z2 += prev_z2;
	    }

	    points1_ex.push_back(x1);
	    points1_ex.push_back(y1);
	    points1_ex.push_back(z1);

	    points2_ex.push_back(x2);
	    points2_ex.push_back(y2);
	    points2_ex.push_back(z2);
	}

    //
    //
    // smooth the circuit

    std::vector<float> arr_spline1;
    std::vector<float> arr_spline2;

    BSpline	spline1(points1_ex, 3, 3);
    BSpline	spline2(points2_ex, 3, 3);

	for (float t = 0.0f; t <= 1.0f; t += 0.001f) // small steps
	{
		float	arr_res1[3];
		float	arr_res2[3];

		spline1.calcAt(t, arr_res1);
		spline2.calcAt(t, arr_res2);

		if (!arr_spline1.empty())
		{
			unsigned int index = arr_spline1.size() - 3;

			//

			float diff_x1 = arr_res1[0] - arr_spline1[index + 0];
			float diff_y1 = arr_res1[1] - arr_spline1[index + 1];
			float diff_z1 = arr_res1[2] - arr_spline1[index + 2];

			float diff_x2 = arr_res2[0] - arr_spline2[index + 0];
			float diff_z2 = arr_res2[1] - arr_spline2[index + 1];
			float diff_y2 = arr_res2[2] - arr_spline2[index + 2];

            float len1 = sqrtf(diff_x1*diff_x1 + diff_y1*diff_y1 + diff_z1*diff_z1);
            float len2 = sqrtf(diff_x2*diff_x2 + diff_y2*diff_y2 + diff_z2*diff_z2);

            // do not add steps that are too close from each other
            if (len1 < 2 && len2 < 2)
                continue;
		}

		arr_spline1.push_back(arr_res1[0]);
		arr_spline1.push_back(arr_res1[1]);
		arr_spline1.push_back(arr_res1[2]);

		arr_spline2.push_back(arr_res2[0]);
		arr_spline2.push_back(arr_res2[1]);
		arr_spline2.push_back(arr_res2[2]);
	}

    //
    //
    // generate circuit

    int geometry_step = 3;

	float pPreviousNormal[3];

    for (int index = 3; index < arr_spline1.size(); index += 3 * geometry_step)
    {

		std::vector<float>	vertices_ground;
		std::vector<float>	vertices_walls1;
		std::vector<float>	vertices_walls2;

		std::vector<int> 	indices;

		//
		//
		// prepare the indices

		indices.push_back(0);
		indices.push_back(1);
		indices.push_back(2);
		indices.push_back(0);
		indices.push_back(3);
		indices.push_back(2);

        //
        //
        // prepare the vertices

    	{ // push ground

#define D_PUSH_VERTEX(in_container, in_x, in_y, in_z)	\
	(in_container).push_back( (in_x) );	\
	(in_container).push_back( (in_y) );	\
	(in_container).push_back( (in_z) );

			int index2 = 0;

	        for (int index5 = index; index5 < arr_spline1.size() && index5 < index + geometry_step * 3; index5 += 3)
	        {

				if (index2 > 0)
				{
					int step = index2 * 4;

					indices.push_back(indices[0]+step);
					indices.push_back(indices[1]+step);
					indices.push_back(indices[2]+step);

					indices.push_back(indices[3]+step);
					indices.push_back(indices[4]+step);
					indices.push_back(indices[5]+step);
				}

				++index2;


				float x1 = arr_spline1[index5+0];
				float y1 = arr_spline1[index5+1];
				float z1 = arr_spline1[index5+2];

				float x2 = arr_spline2[index5+0];
				float y2 = arr_spline2[index5+1];
				float z2 = arr_spline2[index5+2];

				float prev_x1 = arr_spline1[(index5-3)+0];
				float prev_y1 = arr_spline1[(index5-3)+1];
				float prev_z1 = arr_spline1[(index5-3)+2];

				float prev_x2 = arr_spline2[(index5-3)+0];
				float prev_y2 = arr_spline2[(index5-3)+1];
				float prev_z2 = arr_spline2[(index5-3)+2];


				D_PUSH_VERTEX(vertices_ground,  x1, y1, z1);
				D_PUSH_VERTEX(vertices_ground,  x2, y2, z2);
				D_PUSH_VERTEX(vertices_ground,  prev_x2, prev_y2, prev_z2);
				D_PUSH_VERTEX(vertices_ground,  prev_x1, prev_y1, prev_z1);


				float pNormal[3];
				calculate_normal(
					&arr_spline2[(index5-3)],
					&arr_spline1[(index5-3)],
					&arr_spline2[index5],
					pNormal
				);

				// std::cout
				// 	<< pNormal[0] << "/"
				// 	<< pNormal[1] << "/"
				// 	<< pNormal[2]
				// 	<< std::endl;

				pNormal[0] *= 10.0f;
				pNormal[1] *= 10.0f;
				pNormal[2] *= 10.0f;

				if (index5 == 3)
				{
					pPreviousNormal[0] = pNormal[0];
					pPreviousNormal[1] = pNormal[1];
					pPreviousNormal[2] = pNormal[2];
				}

				D_PUSH_VERTEX(vertices_walls1,  prev_x1, prev_y1, prev_z1);
				D_PUSH_VERTEX(vertices_walls1,  prev_x1+pPreviousNormal[0], prev_y1+pPreviousNormal[1], prev_z1+pPreviousNormal[2]);
				D_PUSH_VERTEX(vertices_walls1,  x1+pNormal[0], y1+pNormal[1], z1+pNormal[2]);
				D_PUSH_VERTEX(vertices_walls1,  x1, y1, z1);

				D_PUSH_VERTEX(vertices_walls2,  prev_x2, prev_y2, prev_z2);
				D_PUSH_VERTEX(vertices_walls2,  prev_x2+pPreviousNormal[0], prev_y2+pPreviousNormal[1], prev_z2+pPreviousNormal[2]);
				D_PUSH_VERTEX(vertices_walls2,  x2+pNormal[0], y2+pNormal[1], z2+pNormal[2]);
				D_PUSH_VERTEX(vertices_walls2,  x2, y2, z2);

				pPreviousNormal[0] = pNormal[0];
				pPreviousNormal[1] = pNormal[1];
				pPreviousNormal[2] = pNormal[2];

			} // for (int index5 = index; index5 < arr_spline1.size() && index5 < index + geometry_step * 3; index5 += 3)


			pPhysicWrapper->createGround(vertices_ground, indices, index);
			pPhysicWrapper->createGhostWall(vertices_walls1, indices);
			pPhysicWrapper->createGhostWall(vertices_walls2, indices);

			if (callback)
			{
				callback(vertices_ground, indices, true);
				callback(vertices_walls1, indices, false);
				callback(vertices_walls2, indices, false);
			}

#undef D_PUSH_VERTEX

    	} // push ground

    } // for (int index = 3; index < arr_spline1.size(); index += 3 * geometry_step)
}


