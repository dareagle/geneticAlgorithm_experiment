

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

		// for (var i = 0; i < genome_size; ++i)
		// 	this._ANNs[i].setWeights( [-0.7027577366271448,-0.22782184064915234,-0.09084531654241779,0.16327766885148712,-0.2785293416495598,-0.12001759664899847,-0.2486676470330128,0.6708137520591878,-0.6409108357187512,-0.6652075955939886,0.36812584486024125,0.22485597478778296,-0.3737043782135187,-0.15761434821250142,0.9167879530817763,0.5645802294067814,-0.21133278731189528,-0.5957178605458302,0.25466712912413175,0.35408454494590724,0.5744402921134885,0.16502646820451605,0.26873738145704307,-0.4887484715297513,0.16849853033987694,-0.13766027807332593,0.11278119144525556,-0.2859823792467536,0.41772438687612534,0.530366216787487,-0.2596011057579044,0.3608346778159667,-0.04519865340551932,0.15000300638867958,-0.11971460651126509,0.02347905600105815,0.8214632367292252,0.9717929835903372,-0.03957575531262654,0.7267598631326475,0.2521560017561968,0.195223505762618,0.17138124879349542,-0.5932792543305971,-0.39496912988826854,-0.3871709856156541,0.09864346900515353,-0.6585621430508843,0.2593184208257112,-0.3551656780176379,-0.02660515459707169,0.43989867537809935,-0.14012052159729294,0.45767587310551505,-0.07245341043949782,-0.48693083075991084,-0.4939786454196238,0.3204774210313045,-0.09002898078637887,0.7634447108091276,-0.1052725156304899,-0.22718208787843652,0.12597790350292415,-0.43835612238473454,-0.16330367101574694,0.09073582042590578,-0.6174709051580941,0.05732561563043248] );

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

			console.log("new alpha");
			console.log("generation=" + this._current_generation);
			console.log("fitness=" + this._alpha_genome.fitness);
			console.log("weights=" + JSON.stringify(this._alpha_genome.weights));

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
