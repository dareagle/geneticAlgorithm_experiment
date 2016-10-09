

define(
    [
          './world.js'
        , './circuit.js'
        , './car.js'
        , '../ai/geneticAlgo.js'

        , 'BSpline' // in /lib

        , '../geometries/geometryColor.js'
    ],
    function(
		  world
		, createCircuit
		, createCar
		, createGeneticAlgo

        , BSpline

        , createGeometryColor
    )
{

	//

	var createSimulation = function ()
	{
		// circuit
		// this._circuit = new createCircuit(elem_id);

	    var arr_checkpoints = [];
	    // var arr_checkpoints = [
	    //     [-15, 0,  5,   -15,10,  5],
	    //     [  0, 0,  5,     0,10,  5],

	    //     [ 15, 0,  5,    15,10,  5],
	    //     [ 30,15,  5,    30,25,  5],
	    //     [ 45, 5,  5,    45,15,  5],
	    //     [ 60, 0,  5,    60,10,  5],
	    //     [ 75, 0,  8,    75,10,  8],
	    //     [ 90, 0, 16,    90,10, 16],
	    //     [105, 0, 20,   105,10, 20],
	    //     [120, 0, 20,   120,10, 20],
	    //     [135, 0, 16,   135,10, 16],
	    //     [150, 0,  8,   150,10,  8],
	    //     [165, 0,  5,   165,10,  5],
	    //     [180, 0,  2,   180,10,  2],
	    //     [195, 0, -6,   195,10, -6],
	    //     [210, 0,-10,   210,10,-10],
	    //     [235, 0,-10,   235,10,-10],
	    //     [250, 0, -6,   250,10, -6],
	    //     [265, 0,  2,   265,10,  2],
	    //     [280, 0,  5,   280,10,  5],
	    //     [295, 0,  5,   295,10,  5],

	    //     // [ 15, 0,15,   15,10,15]

	    //     // [ 15, 0, 5,   15,10, 5],
	    //     // [ 30, 0,10,   30,10, 5],
	    //     // [ 45, 0,15,   45, 0, 5]
	    // ];





	    {
	        var tmp_checkpoint = [];

	        // var points = [[0,0,0],[20,0,0],[40,20,5],[60,-20,0],[80,0,0],[80,0,20]];
	        // var points2 = [[0,10,0],[20,10,0],[40,30,5],[60,-10,0],[80,10,0],[80,10,20]];

	        var points = [];
	        var points2 = [];
	        var colors = [];

	        //
	        // circuit

				// points.push([0,0,0]); points2.push([0,15,0]);
				// points.push([40,0,0]); points2.push([40,0,0]);



				// { // turned looping

	   //              points.push([20,0,0]); points2.push([20,0,0]);
	   //              points.push([20,0,20]); points2.push([20,0,20]);
	   //              points.push([-20,0,20]); points2.push([-20,0,20]);

	   //              points.push([-20,30,-15]); points2.push([-20,20,0]);
	   //              points.push([10,30,0]); points2.push([0,30,-10]);
	   //              // points.push([0,20,0]); points2.push([0,20,0]);

				// } // turned looping



    //             points.push([10,10,0]); points2.push([20,20,0]);


				// { // looping

				// 	var r = 40;
				// 	var d = 10;

				// 	points.push([r,0,0]); points2.push([r,0,0]);
				// 	points.push([r,0,r]); points2.push([r,0,r]);
				// 	points.push([-r,d,r]); points2.push([-r,d,r]);
				// 	points.push([-r,d,-r]); points2.push([-r,d,-r]);
				// 	points.push([r,d,-r]); points2.push([r,d,-r]);
				// 	points.push([r,0,0]); points2.push([r,0,0]);

				// 	points.push([20,0,0]); points2.push([20,0,0]);
				// 	points.push([20,0,0]); points2.push([20,0,0]);

				// } // looping




    //             { // infinity loop

    //                 var pente = 7;

    //                 points.push([30,0,+pente]); points2.push([30,0,0]);

    //                 for (var i = 0; i < 1; ++i)
    //                 {
    //                     var height = +3.5;
    //                     var inner_radius = 25;
    //                     var outer_radius = 35;

    //                     points.push([outer_radius,outer_radius,height]); points2.push([inner_radius,inner_radius,height]);
    //                     points.push([-outer_radius,outer_radius,height]); points2.push([-inner_radius,inner_radius,height]);
    //                     points.push([-outer_radius,-outer_radius,height]); points2.push([-inner_radius,-inner_radius,height]);
    //                     points.push([outer_radius,-outer_radius,height]); points2.push([inner_radius,-inner_radius,height]);
    //                 }

    //                 points.push([30,0,0]); points2.push([30,0,+pente*2]);

    //                 for (var i = 0; i < 1; ++i)
    //                 {
    //                     var height = +3.5;
    //                     var inner_radius = 35;
    //                     var outer_radius = 25;

    //                     points.push([outer_radius,-outer_radius,height]); points2.push([inner_radius,-inner_radius,height]);
    //                     points.push([-outer_radius,-outer_radius,height]); points2.push([-inner_radius,-inner_radius,height]);
    //                     points.push([-outer_radius,outer_radius,height]); points2.push([-inner_radius,inner_radius,height]);
    //                     points.push([outer_radius,outer_radius,height]); points2.push([inner_radius,inner_radius,height]);
    //                 }

    //                 points.push([30,0,0]); points2.push([30,0,-pente]);

    //             } // infinity loop







				// points.push([20,0,0]); points2.push([20,0,0]);
				// points.push([20,30,0]); points2.push([20,30,0]);
				// points.push([20,-30,0]); points2.push([20,-30,0]);
				// points.push([20,-30,0]); points2.push([20,-30,0]);
				// points.push([20,30,0]); points2.push([20,30,0]);
				// points.push([20,0,0]); points2.push([20,0,0]);

				// // points.push([20,0,0]); points2.push([20,0,0]);

				// // points.push([20,0,0]); points2.push([20,0,0]);

				// points.push([20,0,5]); points2.push([20,0,-5]);
				// points.push([20,30,-10]); points2.push([20,30,10]);
				// points.push([20,-30,5]); points2.push([20,-30,-5]);
				// points.push([20,-30,5]); points2.push([20,-30,-5]);
				// points.push([20,30,-5]); points2.push([20,30,5]);
				// points.push([20,0,0]); points2.push([20,0,0]);

				// points.push([20,0,0]); points2.push([20,0,0]);



                points.push([0,0,0]); points2.push([0,15,0]); colors.push([0,0,1]);

                points.push([90,0,0]); points2.push([90,0,0]); colors.push([0,0,1]);

                { // turned looping

                    var color = [0,0.5,0]

                    points.push([30,0,30]); points2.push([30,0,30]); colors.push(color);
                    points.push([-30,0,30]); points2.push([-30,0,30]); colors.push(color);

                    points.push([-30,30,-15]); points2.push([-30,15,0]); colors.push(color);
                    points.push([20,30,0]); points2.push([0,30,-15]); colors.push(color);

                    points.push([20,20,0]); points2.push([35,35,0]); colors.push(color);
                    points.push([40,0,0]); points2.push([45,0,0]); colors.push(color);

                } // turned looping


                points.push([30,0,0]); points2.push([30,0,0]); colors.push([1,1,1]);


                { // looping

                    var color = [0,0,1]

                    var r = 40;
                    var d = 20;

                    points.push([r,0,0]); points2.push([r,0,0]); colors.push(color);
                    points.push([r,0,r]); points2.push([r,0,r]); colors.push(color);
                    points.push([-r,d,r]); points2.push([-r,d,r]); colors.push(color);
                    points.push([-r,d,-r]); points2.push([-r,d,-r]); colors.push(color);
                    points.push([r,d,-r]); points2.push([r,d,-r]); colors.push(color);
                    points.push([r,0,0]); points2.push([r,0,0]); colors.push(color);

                    points.push([20,0,0]); points2.push([20,0,0]); colors.push(color);
                    points.push([20,0,0]); points2.push([20,0,0]); colors.push(color);

                } // looping


                points.push([30,0,0]); points2.push([30,0,0]); colors.push([1,1,1])


                { // infinity loop

                    var color = [0,0.5,0];

                    var pente = 7;

                    var height = 6;
                    var inner_radius = 35;
                    var outer_radius = 60;


                    points.push([30,0,+pente]); points2.push([30,0,0]); colors.push(color)

                    for (var i = 0; i < 1; ++i)
                    {
                        points.push([outer_radius,outer_radius,height]); points2.push([inner_radius,inner_radius,height]); colors.push(color)
                        points.push([-outer_radius,outer_radius,height]); points2.push([-inner_radius,inner_radius,height]); colors.push(color)

                        points.push([-outer_radius,-outer_radius,height]); points2.push([-inner_radius,-inner_radius,height]); colors.push(color)
                        points.push([outer_radius,-outer_radius,height]); points2.push([inner_radius,-inner_radius,height]); colors.push(color)
                    }

                    points.push([30,0,0]); points2.push([30,0,+pente*2]); colors.push(color)
 
                    for (var i = 0; i < 1; ++i)
                    {
                        points.push([inner_radius,-inner_radius,height]); points2.push([outer_radius,-outer_radius,height]); colors.push(color)
                        points.push([-inner_radius,-inner_radius,height]); points2.push([-outer_radius,-outer_radius,height]); colors.push(color)

                        points.push([-inner_radius,inner_radius,height]); points2.push([-outer_radius,outer_radius,height]); colors.push(color)
                        points.push([inner_radius,inner_radius,height]); points2.push([outer_radius,outer_radius,height]); colors.push(color)
                    }

                    points.push([30,0,0]); points2.push([30,0,-pente]); colors.push(color)

                } // infinity loop




	        // circuit
	        //

	        var points_ex = [];
	        var points2_ex = [];

	        for (var index = 0; index < points.length; ++index)
	        {
	            var curr1 = points[index];
	            var curr2 = points2[index];

	            if (index > 0)
	            {
	                // concatenate the point

	                var prev1 = points_ex[index-1];
	                var prev2 = points2_ex[index-1];

	                for (var prop in curr1)
	                {
	                    curr1[prop] += prev1[prop];
	                    curr2[prop] += prev2[prop];
	                }
	            }

	            points_ex.push(curr1);
	            points2_ex.push(curr2);
	        }

	        //

	        { // geom

	        	var geom_vertices = [];

				function lerp(val1, val2, ratio)
				{
					return val1 + (val2 - val1) * ratio;
				}

				for (var index = 0; index < points_ex.length; ++index)
				{
					var vertex1 = points_ex[index];
					var vertex2 = points2_ex[index];

					// geom_vertices.push(vertex1[0], vertex1[1], vertex1[2], 1,1,1);
					// geom_vertices.push(vertex2[0], vertex2[1], vertex2[2], 1,1,1);

					var v1 = [ lerp(vertex1[0], vertex2[0], -0.5), lerp(vertex1[1], vertex2[1], -0.5), lerp(vertex1[2], vertex2[2], -0.5) ];
					var v2 = [ lerp(vertex1[0], vertex2[0], +1.5), lerp(vertex1[1], vertex2[1], +1.5), lerp(vertex1[2], vertex2[2], +1.5) ];
					geom_vertices.push(v1[0], v1[1], v1[2], 1,1,1);
					geom_vertices.push(v2[0], v2[1], v2[2], 1,1,1);

					if (index > 0)
					{
						geom_vertices.push(points_ex[index-1][0], points_ex[index-1][1], points_ex[index-1][2], 1,1,1);
						geom_vertices.push(points_ex[index][0], points_ex[index][1], points_ex[index][2], 1,1,1);

						geom_vertices.push(points2_ex[index-1][0], points2_ex[index-1][1], points2_ex[index-1][2], 1,1,1);
						geom_vertices.push(points2_ex[index][0], points2_ex[index][1], points2_ex[index][2], 1,1,1);
					}
				}

	        	// console.log(geom_vertices);

				this._geom_skelton = new createGeometryColor(geom_vertices, gl.LINES);

	        } // geom

	        var spline = new BSpline(points_ex,3); //making BSpline
	        var spline2 = new BSpline(points2_ex,3); //making BSpline
	        var spline_color = new BSpline(colors,3); //making BSpline

	        for (var t = 0; t <= 1; t += 0.001)
	        {
	            var p = spline.calcAt(t);
	            var p2 = spline2.calcAt(t);
	            var color = spline_color.calcAt(t);

	            // console.log(p, p2);

	            var curr = [ p[0], p[1], p[2], p2[0], p2[1], p2[2], color[0],color[1],color[2] ];

	            if (tmp_checkpoint.length > 0)
	            {
	                // validate the point

	                var prev = tmp_checkpoint[tmp_checkpoint.length-1];

	                var diff = [
	                    curr[0]-prev[0],
	                    curr[1]-prev[1],
	                    curr[2]-prev[2],
	                    curr[3]-prev[3],
	                    curr[4]-prev[4],
	                    curr[5]-prev[5],
	                ];

	                var len1 = Math.sqrt(diff[0]*diff[0] + diff[1]*diff[1] + diff[2]*diff[2]);
	                var len2 = Math.sqrt(diff[3]*diff[3] + diff[4]*diff[4] + diff[5]*diff[5]);

	                if (len1 < 2 && len2 < 2)
	                // if (len1 < 4 && len2 < 4)
	                    continue;
	            }

	            tmp_checkpoint.push(curr);
	        }

	        arr_checkpoints = tmp_checkpoint;
	    }





	    this._circuit = new createCircuit( arr_checkpoints );



		var genome_size = 30;
		// var genome_size = 0;
		// this._ann_topology = [5, 4, 3, 2];
		// this._ann_topology = [15, 4, 3, 2];
		// this._ann_topology = [15, 4, 2];
		this._ann_topology = [16, 4, 2];

		this._geneticAlgo = new createGeneticAlgo( genome_size, this._ann_topology );

		//

		// cars
		this._cars = [];
		for (var i = 0; i < genome_size; ++i)
		{
			var car = new createCar();

			this._geneticAlgo._genomes[i].car = car;

			car.reset();

		// 	car._position = {
		// 		  x: this._circuit._start_position.x
		// 		, y: this._circuit._start_position.y
		// 	};

		// 	car._angle = this._circuit._start_angle;

		// 	car._checkpoints = [];
		// 	for (var j = 0; j < this._circuit._checkpoints.length; ++j)
		// 		car._checkpoints.push( this._circuit._checkpoints[j] );

			this._cars.push(car);
		}

		this._playable_car = new createCar();

		this._playMode = false;

        var self = this;
        document.addEventListener('keydown', function(event)
        {
			if (event.keyCode == 80) // <- P
				self._playMode = !self._playMode;
        });
	};

	//

	createSimulation.prototype.update = function(time)
	{
        world._my_update(time);

        if (this._playMode == true)
        {
        	this._playable_car.processEvent();
        	this._playable_car.update();
        	return;
        }


		var someone_is_alive = false;

		for (var i = 0; i < this._cars.length; ++i)
		{
			if (!this._cars[i]._alive)
				continue;

			this._cars[i].processANN( this._geneticAlgo._ANNs[i] );
			this._cars[i].update();

			someone_is_alive = true;
		}


		// end of the current generation?

		if (someone_is_alive)
			return; // no

		// rate the genome

		for (var i = 0; i < this._cars.length; ++i)
			this._geneticAlgo._genomes[i].fitness = this._cars[i]._fitness;

		this._geneticAlgo.BreedPopulation();

		// // save the best trail

		// if (this._geneticAlgo._is_a_great_generation)
		// {
		// 	this._geneticAlgo._is_a_great_generation = false;
		// }

		// reset the cars

		for (var i = 0; i < this._cars.length; ++i)
		{
			var car = this._cars[i];

			this._geneticAlgo._genomes[i].car = car;

			car.reset();

		// 	car._position = {
		// 		  x: this._circuit._start_position.x
		// 		, y: this._circuit._start_position.y
		// 	};

		// 	car._angle = this._circuit._start_angle;

		// 	car._checkpoints = [];
		// 	for (j in this._circuit._checkpoints)
		// 		car._checkpoints.push( this._circuit._checkpoints[j] );

		// 	car._alive = true;
		// 	car._fitness = 0;
		// 	car._total_update = 0;
		// 	car._trail = [];
		// 	car._min_updates = 100;
		}
	};

	//

	createSimulation.prototype.render = function(in_shader_color, in_viewMatrix, car_index)
	{
		if (this._playMode)
		{
	        gl.uniform1f(in_shader_color.uColorApha, 1.0);
			this._playable_car.render(in_shader_color, in_viewMatrix);

	        gl.uniformMatrix4fv(in_shader_color.uMVMatrix, false, in_viewMatrix); // lookAt only

	        gl.uniform1f(in_shader_color.uColorApha, 0.5);
			this._playable_car.render_sensors(in_shader_color);
		}
		else
		{
			for (var i = 0; i < this._cars.length; ++i)
				this._cars[i].render(in_shader_color, in_viewMatrix);

	        gl.uniformMatrix4fv(in_shader_color.uMVMatrix, false, in_viewMatrix); // lookAt only

	        if (car_index >= 0)
	        {
		        gl.uniform1f(in_shader_color.uColorApha, 0.5);

				// for (var i = 0; i < this._cars.length; ++i)
				// 	this._cars[i].render_sensors(in_shader_color);

				this._cars[car_index].render_sensors(in_shader_color);

				this._cars[car_index].render_vision();
	        }
		}

        // gl.uniformMatrix4fv(in_shader_color.uMVMatrix, false, in_viewMatrix); // lookAt only

        this._geom_skelton.render(in_shader_color, false);

        this._circuit.render(in_shader_color);
	}


	return createSimulation;
})
