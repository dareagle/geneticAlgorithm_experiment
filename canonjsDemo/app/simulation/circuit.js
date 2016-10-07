
// create
// render

define(
	[
		  'cannon.min' // in /lib

		, './world.js'

        , '../geometries/geometryColor.js'
	],
	function(
		  unused_CANNON // <- use CANNON

		, world

        , createGeometryColor
	)
{
	function createCircuit(arr_checkpoints)
	{
		//
		//
		// duplicate checkpoints

		// function lerp(val1, val2, ratio)
		// {
		// 	return val1 + (val2 - val1) * ratio;
		// }

		// var	tmp_checkpoints = [];
		// tmp_checkpoints.push(arr_checkpoints[0]);
		// for (var i = 1; i < arr_checkpoints.length; ++i)
		// {
		// 	var l1 = arr_checkpoints[i - 1];
		// 	var l2 = arr_checkpoints[i];

		// 	for (var f = 0.25; f < 1.0; f += 0.25)
		// 	{
		// 		tmp_checkpoints.push([
		// 			  lerp(l1[0], l2[0], f)
		// 			, lerp(l1[1], l2[1], f)
		// 			, lerp(l1[2], l2[2], f)
		// 			, lerp(l1[3], l2[3], f)
		// 			, lerp(l1[4], l2[4], f)
		// 			, lerp(l1[5], l2[5], f)
		// 		]);
		// 	}

		// 	tmp_checkpoints.push(l2);
		// }
		// arr_checkpoints = tmp_checkpoints;

		//
		//
		// polygonise checkpoints

		this._arr_geoms = [];

		var indices = [
			// 0, 1, 2, // triangle 0
			// 1, 3, 2, // triangle 1
			0, 2, 1, // triangle 2
			1, 2, 3, // triangle 3
		];

		var geom_vertices = [];
		var geom_vertices_wall = [];

		var prev_normal = null;

		for (var index = 1; index < arr_checkpoints.length; ++index)
		{
			var c1 = arr_checkpoints[index-1];
			var c2 = arr_checkpoints[index];

			var p11 = [ c1[0], c1[1], c1[2] ];
			var p12 = [ c1[3], c1[4], c1[5] ];
			var p21 = [ c2[0], c2[1], c2[2] ];
			var p22 = [ c2[3], c2[4], c2[5] ];

			var vertices = [
			   p11[0], p11[1], p11[2], // vertex 0
			   p12[0], p12[1], p12[2], // vertex 1
			   p21[0], p21[1], p21[2], // vertex 2
			   p22[0], p22[1], p22[2]  // vertex 3
			];
			var planeShape = new CANNON.Trimesh(vertices, indices);
			var planeBody = new CANNON.Body({mass:0});
			planeBody.addShape(planeShape);
			planeBody.collisionFilterGroup = -1;
			planeBody.collisionFilterMask = world._GROUP_sensor; // <- sensors will collide the floor
			world.addBody(planeBody);





			//
			//
			// calculate the normal

			var v0 = p12;
			var v1 = p21;
			var v2 = p22;

			var px = v1[0] - v0[0];
			var py = v1[1] - v0[1];
			var pz = v1[2] - v0[2];

			var qx = v2[0] - v0[0];
			var qy = v2[1] - v0[1];
			var qz = v2[2] - v0[2];

			var nx = (py * qz) - (pz * qy);
			var ny = (pz * qx) - (px * qz);
			var nz = (px * qy) - (py * qx);

			var normal = [nx, ny, nz];
			var tmp_len = normal[0]*normal[0] + normal[1]*normal[1] + normal[2]*normal[2];
			normal[0] /= tmp_len;
			normal[1] /= tmp_len;
			normal[2] /= tmp_len;

			normal[0] *= 200;
			normal[1] *= 200;
			normal[2] *= 200;

			// calculate the normal
			//
			//


			if (!prev_normal)
				prev_normal = normal;



			//
			// walls

			var vertices_wall1 = [
			   // p11[0], p11[1], p11[2],
			   // p11[0], p11[1], p11[2]+10,
			   // p21[0], p21[1], p21[2],
			   // p21[0], p21[1], p21[2]+10
			   p11[0], p11[1], p11[2],
			   p11[0]+prev_normal[0], p11[1]+prev_normal[1], p11[2]+prev_normal[2],
			   p21[0], p21[1], p21[2],
			   p21[0]+normal[0], p21[1]+normal[1], p21[2]+normal[2]
			];
			var planeShape = new CANNON.Trimesh(vertices_wall1, indices);
			var planeBody = new CANNON.Body({mass:0});
			planeBody.addShape(planeShape);
			planeBody.collisionFilterGroup = world._GROUP_wall;
			planeBody.collisionFilterMask = world._GROUP_sensor;
			world.addBody(planeBody);

			var vertices_wall2 = [
			   // p12[0], p12[1], p12[2],
			   // p12[0], p12[1], p12[2]+10,
			   // p22[0], p22[1], p22[2],
			   // p22[0], p22[1], p22[2]+10
			   p12[0], p12[1], p12[2],
			   p12[0]+prev_normal[0], p12[1]+prev_normal[1], p12[2]+prev_normal[2],
			   p22[0], p22[1], p22[2],
			   p22[0]+normal[0], p22[1]+normal[1], p22[2]+normal[2]
			];
			var planeShape = new CANNON.Trimesh(vertices_wall2, indices);
			var planeBody = new CANNON.Body({mass:0});
			planeBody.addShape(planeShape);
			planeBody.collisionFilterGroup = world._GROUP_wall;
			planeBody.collisionFilterMask = world._GROUP_sensor;
			world.addBody(planeBody);

			prev_normal = normal;

			//
			//


			for (var index2 in indices)
			{
				var vertexi = indices[index2] * 3;

				if (vertexi == 0 || vertexi == 3)
					geom_vertices.push( vertices[vertexi+0], vertices[vertexi+1], vertices[vertexi+2], 1,1,1 );
				else
					geom_vertices.push( vertices[vertexi+0], vertices[vertexi+1], vertices[vertexi+2], 0,0,1 );
			}

			for (var index2 in indices)
			{
				var vertexi = indices[index2] * 3;

				geom_vertices_wall.push( vertices_wall1[vertexi+0], vertices_wall1[vertexi+1], vertices_wall1[vertexi+2], 0,0.5,0.5 );
			}

			for (var index2 in indices)
			{
				var vertexi = indices[index2] * 3;

				geom_vertices_wall.push( vertices_wall2[vertexi+0], vertices_wall2[vertexi+1], vertices_wall2[vertexi+2], 0,0.5,0.5 );
			}
		}

		var geom_cube2 = new createGeometryColor(geom_vertices, gl.TRIANGLES);
		this._arr_geoms.push(geom_cube2);

		var geom_cube2 = new createGeometryColor(geom_vertices_wall, gl.TRIANGLES);
		this._arr_geoms.push(geom_cube2);

	}

	//
	//

	createCircuit.prototype.render = function(shader_color)
	{
        // for (var index in this._arr_geoms)
        //     this._arr_geoms[index].render(shader_color);

        gl.uniform1f(shader_color.uColorApha, 1.0);

        this._arr_geoms[0].render(shader_color);

        gl.uniform1f(shader_color.uColorApha, 0.2);

        this._arr_geoms[1].render(shader_color);

        gl.uniform1f(shader_color.uColorApha, 1.0);
	}

	return createCircuit;
});
