

define(
    [
          './circuit.js'
        , './car.js'
        , '../ai/geneticAlgo.js'

		, '../ai/ArtificialNeuralNetwork.js'
    ],
    function(
		  createCircuit
		, createCar
		, createGeneticAlgo

		, createArtificialNeuralNetwork
    )
{

	//

	var createSimulation = function (elem_id)
	{
		// circuit
		this._circuit = new createCircuit(elem_id);

		var genome_size = 30;

		this._geneticAlgo = new createGeneticAlgo( genome_size );

		//

		// cars
		this._cars = [];
		for (var i = 0; i < genome_size; ++i)
		{
			var car = new createCar();

			this._geneticAlgo._genomes[i].car = car;

			car._position = {
				  x: this._circuit._start_position.x
				, y: this._circuit._start_position.y
			};

			car._angle = this._circuit._start_angle;

			car._checkpoints = [];
			for (var j = 0; j < this._circuit._checkpoints.length; ++j)
				car._checkpoints.push( this._circuit._checkpoints[j] );

			this._cars.push(car);
		}

		this._trails = [];

		this._alpha_car = null;

		this._start_to_stop_sens = true;
	};

	//

	createSimulation.prototype.update = function(step)
	{
		var someone_is_alive = false;

		for (var i = 0; i < this._cars.length; ++i)
		{
			if (!this._cars[i]._alive)
				continue;

			this._cars[i].update(step, this._circuit._walls, this._geneticAlgo._ANNs[i]);

			someone_is_alive = true;
		}


		//
		// alpha car

		if (this._alpha_car)
			this._alpha_car.update(step, this._circuit._walls, this._alpha_car._myANN);

		// alpha car
		//


		// end of the current generation?

		if (someone_is_alive)
			return; // no

		// rate the genome

		for (var i = 0; i < this._cars.length; ++i)
			this._geneticAlgo._genomes[i].fitness = this._cars[i]._fitness;

		this._geneticAlgo.BreedPopulation();

		// save the best trail

		if (this._geneticAlgo._is_a_great_generation)
		{
			this._geneticAlgo._is_a_great_generation = false;

			this._trails.push( this._geneticAlgo._alpha_genome.car._trail );
			if (this._trails.length > 5)
				this._trails.splice(0, 1);
		}


		//
		//
		//
		// alpha car

		this._alpha_car = new createCar();
		this._alpha_car._position = {
			  x: this._circuit._start_position.x
			, y: this._circuit._start_position.y
		};

		this._alpha_car._angle = this._circuit._start_angle;

		this._alpha_car._checkpoints = [];
		for (var j = 0; j < this._circuit._checkpoints.length; ++j)
			this._alpha_car._checkpoints.push( this._circuit._checkpoints[j] );


		this._alpha_car._myANN = new createArtificialNeuralNetwork([5, 4, 2]);

		var weights = [];
		for (index in this._geneticAlgo._alpha_genome.weights)
			weights.push( this._geneticAlgo._alpha_genome.weights[index] );

		this._alpha_car._myANN.setWeights( weights );

		// this._alpha_car._myANN.setWeights( this._geneticAlgo._alpha_genome.weights );

		// alpha car
		//
		//
		//


		// reset the cars

		this._start_to_stop_sens = !this._start_to_stop_sens;

		if (this._start_to_stop_sens)
		{
			for (var i = 0; i < this._cars.length; ++i)
			{
				var car = this._cars[i];

				this._geneticAlgo._genomes[i].car = car;

				car._position = {
					  x: this._circuit._start_position.x
					, y: this._circuit._start_position.y
				};

				car._angle = this._circuit._start_angle;

				car._checkpoints = [];
				for (j in this._circuit._checkpoints)
					car._checkpoints.push( this._circuit._checkpoints[j] );

				car._alive = true;
				car._fitness = 0;
				car._total_update = 0;
				car._trail = [];
			}
		}
		else
		{
			for (var i = 0; i < this._cars.length; ++i)
			{
				var car = this._cars[i];

				this._geneticAlgo._genomes[i].car = car;

				car._position = {
					  x: this._circuit._stop_position.x
					, y: this._circuit._stop_position.y
				};

				car._angle = this._circuit._stop_angle;

				car._checkpoints = [];
				for (j in this._circuit._checkpoints)
					car._checkpoints.push( this._circuit._checkpoints[j] );

				car._alive = true;
				car._fitness = 0;
				car._total_update = 0;
				car._trail = [];
			}
		}

	};

	//

	return createSimulation;
})
