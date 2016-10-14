
'use strict'

define(
	[
          'cannon.min' // in /lib
    ],
    function(
          unused_CANNON // <- use CANNON
    )
{
    var world = new CANNON.World();
    world.gravity.set(0, 0, -9.82); // m/s²

    world._GROUP_sensor = 1 << 0;
    world._GROUP_wall = 1 << 1;

    //
    //

    //
    //

    var fixedTimeStep = 1.0 / 60.0; // seconds
    var maxSubSteps = 3;
    var lastTime;

    world._my_update = function (time)
    {
        if (lastTime !== undefined)
        {
            var dt = (time - lastTime) / 1000;
            world.step(fixedTimeStep, dt, maxSubSteps);
        }
        lastTime = time;
    }

    //
    //

    //
    //

    return world;
});