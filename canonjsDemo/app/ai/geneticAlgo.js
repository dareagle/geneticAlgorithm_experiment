

define(
    [
		  './ArtificialNeuralNetwork.js'

        // , '../utils/Logger.js'
    ],
    function(
		  createArtificialNeuralNetwork

		// , createLogger
    )
{

	//

	var createGeneticAlgo = function (genome_size, ann_topology)
	{
		this._current_id = 0;

		this._ANNs = [];
		for (var i = 0; i < genome_size; ++i)
			this._ANNs.push( new createArtificialNeuralNetwork( ann_topology ) );

		this._genomes = [];
		for (var i = 0; i < genome_size; ++i)
		{
			this._genomes.push({
				  id: this._current_id++
				, fitness: 0.0
				, weights: this._ANNs[i].getWeights()
			});
		}

		this._current_generation = 1;
		this._best_fitness = -1; // <------------ will force a bad first generation to be accepted
		this._alpha_genome = {fitness: -1}; // <- will force a bad first generation to be accepted

		//

		// this._logger = new createLogger( 'logger' );
	};

	//

	createGeneticAlgo.prototype.BreedPopulation = function()
	{
		if (this._genomes.length == 0)
			return;

		var bestGenomes = this._getBestGenomes(6);

		var children = [];

		if (this._best_fitness < bestGenomes[0].fitness)
		{
			this._best_fitness = bestGenomes[0].fitness;

			// console.log("current_generation=" + this._current_generation + ", best_fitness=" + this._best_fitness);
		}

		if (this._best_fitness > this._alpha_genome.fitness)
		{
			this._alpha_genome = bestGenomes[0];
			this._is_a_great_generation = true;

			console.log("new alpha (" + this._current_generation + ")");
			// this._logger.push_line('generation: ' + this._current_generation + ' => promoted a new genome alpha');
		}
		else
		{
			var	bestDude = {
				  id: this._alpha_genome.id
				, fitness: 0.0
				, weights: []
			};

			for (var index in this._alpha_genome.weights)
				bestDude.weights.push( this._alpha_genome.weights[index] );

			this._mutate(bestDude);
			children.push(bestDude);

			// console.log("alpha reused");
			// this._logger.push_line('generation: ' + this._current_generation + ' => genome alpha is being reused');
		}


		{ // Carry on the best dude.

			var	bestDude = {
				  id: bestGenomes[0].id
				, fitness: 0.0
				, weights: []
			};

			for (var index in bestGenomes[0].weights)
				bestDude.weights.push( bestGenomes[0].weights[index] );

			this._mutate(bestDude);

			children.push(bestDude);

		} // Carry on the best dude.

		// breed best genomes

		for (var i = 0; i < bestGenomes.length; ++i)
			for (var j = i+1; j < bestGenomes.length; ++j)
			{
				var	baby1 = {},
					baby2 = {};

				this._crossBreed(bestGenomes[i], bestGenomes[j], baby1, baby2);
				this._mutate(baby1);
				this._mutate(baby2);

				if (children.length < this._genomes.length) children.push(baby1);
				if (children.length < this._genomes.length) children.push(baby2);
			}

		// For the remainding n population, add some random kiddies.
		var remainingChildren = (this._genomes.length - children.length);

		var total_weights = this._ANNs[0].getWeights().length;

		for (var i = 0; i < remainingChildren; i++)
		{
			var	genome = {
				  id: this._current_id++
				, fitness: 0.0
				, weights: []
			};

			for (var j = 0; j < total_weights; j++)
				genome.weights.push( Math.random() - Math.random() );

			children.push( genome );
		}

		this._genomes = children;
		++this._current_generation;

		// for (var i = 0; i < this._genomes.length; ++i)
		// 	console.log('=>', i, this._genomes[i].weights);

		for (var i = 0; i < this._genomes.length; ++i)
		{
			// console.log(i, ' => ', this._genomes[i].weights);

			this._ANNs[i].setWeights( this._genomes[i].weights );
		}
	}

	//

	createGeneticAlgo.prototype._getBestGenomes = function(totalAsked)
	{
		// realistic total outputed genomes 
		var totalAsked = Math.min(this._genomes.length, totalAsked);

		var out = [];

		var tries = totalAsked; // we try a limited ammount of time

		while (out.length < totalAsked)
		{
			if (!tries--)
				break;

			var bestFitness = 0;
			var bestIndex = -1;

			// console.log(out.length, totalAsked);
			// console.log(this._genomes.length);

			for (var i = 0; i < this._genomes.length; i++)
			{
				// console.log(i);

				if (this._genomes[i].fitness > bestFitness)
				{
					var isUsed = false;

					// no duplicate
					for (var j = 0; j < out.length; j++)
						if (out[j].id == this._genomes[i].id)
						{
							isUsed = true;
							break;
						}

					if (isUsed)
						continue;

					bestIndex = i;
					bestFitness = this._genomes[bestIndex].fitness;
				}
			}

			// console.log('bestIndex', bestIndex);

			if (bestIndex != -1)
				out.push( this._genomes[bestIndex] );
		}

		if (!out.length) // out of tries and nothing? -> push something anyway!
		{
			console.log("stupid generation");
			for (var i = 0; i < totalAsked; ++i)
				out.push( this._genomes[i] );
		}

		return out;
	}

	//

	createGeneticAlgo.prototype._crossBreed = function(g1, g2, baby1, baby2)
	{
		var totalWeights = g1.weights.length;
		var crossover = Math.floor(Math.random() * totalWeights);

		baby1.id = this._current_id++;
		baby1.weights = [];

		baby2.id = this._current_id++;
		baby2.weights = [];

		for (var i = 0; i < crossover; i++)
		{
			baby1.weights[i] = g1.weights[i];
			baby2.weights[i] = g2.weights[i];
		}

		for (var i = crossover; i < totalWeights; i++)
		{
			baby1.weights[i] = g2.weights[i];
			baby2.weights[i] = g1.weights[i];
		}
	}

	//

	createGeneticAlgo.prototype._mutate = function(genome)
	{
		for (var i = 0; i < genome.weights.length; ++i)
			if (Math.random() < 0.1)
				genome.weights[i] += ((Math.random() - Math.random()) * 0.2);
	}

	//

	return createGeneticAlgo;
})
