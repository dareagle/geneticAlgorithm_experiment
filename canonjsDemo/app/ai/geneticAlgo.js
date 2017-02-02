
'use strict'

define(
    [
        './ArtificialNeuralNetwork.js'
    ],
    function(
        createArtificialNeuralNetwork
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
        //  // this._ANNs[i].setWeights( [0.27838726936178526,-0.6018063629839614,-0.5500745614338742,-0.04404686842224241,-0.30271111366493647,-0.2125636737717197,0.5416251781093948,-0.4089826935991054,0.5227332006228389,-0.11577145455014141,-0.2068278944557666,-0.8140088174158846,-0.1515642635310986,1.078936900290007,0.4009201648597412,-0.1597875836373975,0.37544784619854676,0.10020281324060107,-0.01983771367675731,-0.30427919435502,-0.17142636002881756,0.3747213038982994,-1.0648982868465438,0.9195250624518974,-0.7918366781457389,0.3992126403714203,0.6256072021561074,0.2449075767952824,0.6540219902630725,0.4221926681818433,-0.41981840135069043,-0.7347272829735817,-0.8402300820273292,-0.4085975010101612,-0.4197273134585566,0.19113972500651832,-0.43758585889159035,-0.3111735112788165,0.92888343924149,-0.13781952101449851,0.38973492120031394,0.2647473248456039,0.09468970462730283,-0.06012336935097115,0.2668809013464862,-0.05944258305640059,-0.5872098752683212,-0.7447160110794607,0.510897018407497,0.05112953793509323,-0.42658460076056964,-0.2622466105473467,0.559273957182758,-0.2815320532041989,-1.0301726777133031,-0.10301063864515192,0.2735551196056757,-0.028597732095406614,0.07816742355292561,-0.4786341649888134,0.4807370893090127,0.01636964299637074,0.18151323998866392,0.14035996058904762,-1.1042077904203,-0.37369589949201026,0.10700733539144784,-0.7895198769198509,-0.3550942783722505,0.2544797059094202,-0.47614938745292545,0.5422266914785815] );
        //  this._ANNs[i].setWeights( [-0.07907482378906146,-0.8450656397585629,-0.5386366156609739,-0.14581290969331243,-0.24702649832373824,-0.22248722468232474,0.6190684580173218,-0.3774274511767726,0.854148672260596,-0.1598759860118532,-0.67239887550108,-1.0254119241576278,-0.18444680275046887,0.9487132325154412,0.31468232560685466,-0.07204578491330055,0.36099315965285605,-0.16930410234884427,-0.06002065535006765,-0.10067472538564515,0.06832236012193804,0.2445591682902653,-1.3345497475681614,0.932175480396832,-0.7999634107037212,0.21336299970584474,0.9934477626507041,0.234525694574977,0.39691555073641804,0.6929497716876816,-0.5023801707374473,-1.4102800133197329,-0.9510138943914076,-0.21825902626269403,-0.6106542865659255,0.20379840189315115,-0.5951700101769288,-0.5615597269273993,0.60612351836662,-0.1598499491183153,-0.0792939406507264,0.3317167090909284,0.20183004688683504,-0.2654522676242276,0.35670849447557107,-0.09792692147686773,-0.6076220857003125,-0.798693586223624,0.120649335981865,0.006289091064703208,-0.29775668794645427,-0.20562949593415156,0.595961177449107,-0.42619533734129433,-1.0819779826945997,-0.27536798808041546,0.39441681066203127,0.21567539901056806,-0.0608548842977201,-0.35748130640504683,0.40076548695731085,0.12739672122117107,0.5783751382366432,0.4356919233739345,-1.540722652908353,-0.74101316351093,0.5627224314064889,-0.9128003585841358,-0.16530520776652347,0.4967201329542126,-0.47987075556728437,0.5377944377639976] );
        //  // this._ANNs[i].setWeights( [-0.07907482378906146,-0.7926722323678428,-0.5386366156609739,-0.14581290969331243,-0.24702649832373824,-0.22248722468232474,0.6190684580173218,-0.3774274511767726,0.854148672260596,-0.1598759860118532,-0.67239887550108,-1.0254119241576278,-0.18444680275046887,0.9487132325154412,0.31468232560685466,-0.07204578491330055,0.38855795300305074,-0.16930410234884427,-0.06002065535006765,-0.10067472538564515,0.06832236012193804,0.2445591682902653,-1.3345497475681614,0.932175480396832,-0.7999634107037212,0.21336299970584474,0.9934477626507041,0.234525694574977,0.39691555073641804,0.6929497716876816,-0.5023801707374473,-1.4102800133197329,-0.9510138943914076,-0.21825902626269403,-0.6106542865659255,0.20379840189315115,-0.4664705642084145,-0.5615597269273993,0.60612351836662,-0.1598499491183153,-0.0792939406507264,0.3317167090909284,0.2145188747541135,-0.2654522676242276,0.2948585705924372,-0.09792692147686773,-0.7168316852150325,-0.798693586223624,0.120649335981865,0.006289091064703208,-0.29775668794645427,-0.20562949593415156,0.595961177449107,-0.42619533734129433,-1.0819779826945997,-0.27536798808041546,0.39441681066203127,0.11689970477098718,-0.0608548842977201,-0.35748130640504683,0.40076548695731085,0.12739672122117107,0.5783751382366432,0.4356919233739345,-1.540722652908353,-0.74101316351093,0.5627224314064889,-0.9128003585841358,-0.16530520776652347,0.4967201329542126,-0.4846683896377428,0.5377944377639976] );
        // this._ANNs[0].setWeights( [-0.07907482378906146,-0.8450656397585629,-0.5386366156609739,-0.14581290969331243,-0.24702649832373824,-0.22248722468232474,0.6190684580173218,-0.3774274511767726,0.854148672260596,-0.1598759860118532,-0.67239887550108,-1.0254119241576278,-0.18444680275046887,0.9487132325154412,0.31468232560685466,-0.07204578491330055,0.36099315965285605,-0.16930410234884427,-0.06002065535006765,-0.10067472538564515,0.06832236012193804,0.2445591682902653,-1.3345497475681614,0.932175480396832,-0.7999634107037212,0.21336299970584474,0.9934477626507041,0.234525694574977,0.39691555073641804,0.6929497716876816,-0.5023801707374473,-1.4102800133197329,-0.9510138943914076,-0.21825902626269403,-0.6106542865659255,0.20379840189315115,-0.5951700101769288,-0.5615597269273993,0.60612351836662,-0.1598499491183153,-0.0792939406507264,0.3317167090909284,0.20183004688683504,-0.2654522676242276,0.35670849447557107,-0.09792692147686773,-0.6076220857003125,-0.798693586223624,0.120649335981865,0.006289091064703208,-0.29775668794645427,-0.20562949593415156,0.595961177449107,-0.42619533734129433,-1.0819779826945997,-0.27536798808041546,0.39441681066203127,0.21567539901056806,-0.0608548842977201,-0.35748130640504683,0.40076548695731085,0.12739672122117107,0.5783751382366432,0.4356919233739345,-1.540722652908353,-0.74101316351093,0.5627224314064889,-0.9128003585841358,-0.16530520776652347,0.4967201329542126,-0.47987075556728437,0.5377944377639976] );

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
            this._best_fitness = bestGenomes[0].fitness;

        if (this._best_fitness > this._alpha_genome.fitness)
        {
            this._alpha_genome = bestGenomes[0];
            this._is_a_great_generation = true;

            console.log("new alpha");
            console.log("generation=" + this._current_generation);
            console.log("fitness=" + this._alpha_genome.fitness);
            console.log("weights=" + JSON.stringify(this._alpha_genome.weights));
        }
        else
        {
            var bestDude = {
                  id: this._alpha_genome.id
                , fitness: 0.0
                , weights: []
            };

            for (var index in this._alpha_genome.weights)
                bestDude.weights.push( this._alpha_genome.weights[index] );

            this._mutate(bestDude);
            children.push(bestDude);
        }


        { // Carry on the best dude.

            var bestDude = {
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
                var baby1 = {},
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
            var genome = {
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

        for (var i = 0; i < this._genomes.length; ++i)
            this._ANNs[i].setWeights( this._genomes[i].weights );
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
