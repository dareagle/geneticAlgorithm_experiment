
'use strict'

define([],function()
{

	//

	var createArtificialIntelligence = function ()
	{
	};

	//

	createArtificialIntelligence.prototype.process = function(arr_input)
	{
		// arr_input[0-5] // low
		// arr_input[5-10] // middle
		// arr_input[10-15] // high

		var low_left2	= arr_input[0];
		var low_left	= arr_input[1];
		var low_middle	= arr_input[2];
		var low_right	= arr_input[3];
		var low_right2	= arr_input[4];

		var mid_left2	= arr_input[5];
		var mid_left	= arr_input[6];
		var mid_middle	= arr_input[7];
		var mid_right	= arr_input[8];
		var mid_right2	= arr_input[9];

		var high_left2	= arr_input[10];
		var high_left	= arr_input[11];
		var high_middle	= arr_input[12];
		var high_right	= arr_input[13];
		var high_right2	= arr_input[14];

		var curr_speed	= arr_input[15];

		// process

		// console.log(curr_speed);

		// /process

		var output = [];
		// output[0] = 0;// steer value
		// output[1] = 0.5;// speed value

		if (high_left < high_right)
			output[0] = -(high_right*0.25);// steer value
		else if (high_left > high_right)
			output[0] = +(high_left*0.25);// steer value
		else
			output[0] = 0;// steer value


		if (curr_speed < 0.6)
			output[1] = 1.0;// speed value
		else
			output[1] = 0.0;// speed value

		return output;
	};

	//

	return createArtificialIntelligence;
})
