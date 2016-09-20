

define(
    [
          './world.js'
        , './circuit.js'
        , './car.js'
        , '../ai/geneticAlgo.js'
    ],
    function(
		  world
		, createCircuit
		, createCar
		, createGeneticAlgo
    )
{

	//

	var createSimulation = function ()
	{
		// circuit
		// this._circuit = new createCircuit(elem_id);

	    var arr_checkpoints = [
	        [-15, 0,  5,   -15,10,  5],
	        [  0, 0,  5,     0,10,  5],

	        [ 15, 0,  5,    15,10,  5],
	        [ 30,15,  5,    30,25,  5],
	        [ 45, 5,  5,    45,15,  5],
	        [ 60, 0,  5,    60,10,  5],
	        [ 75, 0,  8,    75,10,  8],
	        [ 90, 0, 16,    90,10, 16],
	        [105, 0, 20,   105,10, 20],
	        [120, 0, 20,   120,10, 20],
	        [135, 0, 16,   135,10, 16],
	        [150, 0,  8,   150,10,  8],
	        [165, 0,  5,   165,10,  5],
	        [180, 0,  2,   180,10,  2],
	        [195, 0, -6,   195,10, -6],
	        [210, 0,-10,   210,10,-10],
	        [235, 0,-10,   235,10,-10],
	        [250, 0, -6,   250,10, -6],
	        [265, 0,  2,   265,10,  2],
	        [280, 0,  5,   280,10,  5],
	        [295, 0,  5,   295,10,  5],

	        // [ 15, 0,15,   15,10,15]

	        // [ 15, 0, 5,   15,10, 5],
	        // [ 30, 0,10,   30,10, 5],
	        // [ 45, 0,15,   45, 0, 5],

	    ];

	    this._circuit = new createCircuit( arr_checkpoints );



		var genome_size = 30;
		// this._ann_topology = [5, 4, 3, 2];
		this._ann_topology = [15, 4, 3, 2];

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
	};

	//

	createSimulation.prototype.update = function(time)
	{

        world._my_update(time);


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
		for (var i = 0; i < this._cars.length; ++i)
			this._cars[i].render(in_shader_color, in_viewMatrix);

        gl.uniformMatrix4fv(in_shader_color.uMVMatrix, false, in_viewMatrix); // lookAt only

        if (car_index >= 0)
        {
	        gl.uniform1f(in_shader_color.uColorApha, 0.5);

			// for (var i = 0; i < this._cars.length; ++i)
			// 	this._cars[i].render_sensors(in_shader_color);

			this._cars[car_index].render_sensors(in_shader_color);
        }

        this._circuit.render(in_shader_color);
	}


	return createSimulation;
})
