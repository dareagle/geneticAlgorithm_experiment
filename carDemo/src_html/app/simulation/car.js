
'use strict'

var utils = require('../utils/collision.js');

//

var createCar = function (elem_id)
{
	this._position = {x:0, y:0};
	this._angle = 0;
	this._alive = true;

	this._fitness = 0;

	this._min_updates = 50;
	this._total_update = 0;

	this._trail = [];

	this._current_checkpoint = 0;

	this._updateSensors();
};

//

createCar.prototype.update = function(step, walls, ann)
{
	if (!this._alive)
		return;


	//
	// min update

	if (this._min_updates > 0)
		--this._min_updates;
	if (this._min_updates == 0)
	{
		this._min_updates = 50;
		this._alive = false;
	}

	// min update
	//
	//


	//

	var input = [];

	for (var i = 0; i < this._sensors.length; ++i)
		input.push( this._sensors[i].value );


	// console.log("input,", input);

	var output = ann.process( input );

	var leftTheta = output[0];
	var rightTheta = output[1];

	if (isNaN(leftTheta))	leftTheta = 0;
	if (isNaN(rightTheta))	rightTheta = 0;

	// console.log("leftTheta=" + leftTheta);
	// console.log("rightTheta=" + rightTheta);

	leftTheta = Math.min(Math.PI/32.0, Math.max(-Math.PI/32.0, leftTheta));
	// rightTheta = Math.min(Math.PI/32.0, Math.max(-Math.PI/32.0, rightTheta));

	// this._angle += (leftTheta - rightTheta) * step;
	this._angle += leftTheta * step;

	// console.log(this._angle);

	// var speed = ((Math.abs(leftTheta + rightTheta)) / 2) * 160.0;

	var speed = rightTheta * 30.0;

	// speed = Math.min(15.0, Math.max(10.0, speed));
	speed = Math.min(15.0, Math.max(-10.0, speed));

	//

	// var speed = 10.0;

	this._position.x += (speed * Math.cos(this._angle)) * step;
	this._position.y += (speed * Math.sin(this._angle)) * step;

	this._trail.push( {x:this._position.x, y:this._position.y} );

	//

	this._collideWalls( walls );
	this._updateSensors();
	this._collideSensors( walls );
	this._collideCheckpoints();

	++this._total_update;
};

//

createCar.prototype._collideWalls = function(walls)
{
	for (var i = 0; i < walls.length; ++i)
		if (utils.CollisionSegmentCercle(walls[i].p1, walls[i].p2, this._position, 10.0))
		{
			this._alive = false;
			break;
		}
}

//

createCar.prototype._updateSensors = function(walls)
{
	// initialise

	// var angles = [
	// 	-Math.PI/2.0,
	// 	-Math.PI/4.0,
	// 	0.0,
	// 	Math.PI/4.0,
	// 	Math.PI/2.0
	// ];

	var angles = [
		-Math.PI/4.0,
		-Math.PI/8.0,
		0.0,
		Math.PI/8.0,
		Math.PI/4.0
	];

	this._sensors = [];
	for (var i = 0; i < angles.length; ++i)
	{
		this._sensors.push({
			line: {
				  p1: {x:0, y:0}
				, p2: {
					  x: 200.0 * Math.cos(angles[i])
					, y: 200.0 * Math.sin(angles[i])
				}
			}
			, value: 1.0
		});
	}

	// translate/rotate

	for (var i = 0; i < this._sensors.length; ++i)
	{
		var sensor = this._sensors[i];

		sensor.line.p1.x += this._position.x;
		sensor.line.p2.x += this._position.x;
		sensor.line.p1.y += this._position.y;
		sensor.line.p2.y += this._position.y;

		sensor.line.p1 = utils.rotateVec2(sensor.line.p1, this._position, this._angle);
		sensor.line.p2 = utils.rotateVec2(sensor.line.p2, this._position, this._angle);
	}
}

//

createCar.prototype._collideSensors = function(walls)
{
	for (var i = 0; i < this._sensors.length; ++i)
	{
		var sensor = this._sensors[i];

		sensor.value = 1.0;

		for (var j = 0; j < walls.length; ++j)
		{
			var wall = walls[j];

			var v = utils.CollisionSegSeg_partial(sensor.line.p1, sensor.line.p2, wall.p1, wall.p2);

			if (v >= 0.0 && v < 1.0 && sensor.value > v)
				sensor.value = v;
		}
	}
}

//

createCar.prototype._collideCheckpoints = function()
{
	if (this._current_checkpoint < this._checkpoints.length)
	{
		var checkpoint = this._checkpoints[this._current_checkpoint];

		if (utils.CollisionSegmentCercle(checkpoint.p1, checkpoint.p2, this._position, 5.0))
		{
			this._min_updates = 50;
			++this._fitness;
			++this._current_checkpoint;
		}
	}

	if (this._current_checkpoint >= this._checkpoints.length)
	{
		// this line reward a faster car once the circuit is completed
		this._fitness += (1000.0 / this._total_update);

		this._alive = false;
	}
}

//

module.exports = createCar;
