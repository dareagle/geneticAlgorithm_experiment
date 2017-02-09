
'use strict'

define(
    [
          'requestAnimationFrame' // in /lib
        , 'fpsmeter' // in /lib

        , './graphic/Renderer.js'

        , './utils/collision.js'

        , './simulation/simulation.js'
    ],
    function(
          unused_requestAnimationFrame // <- use window.requestAnimFrame
        , unused_fpsmeter // <- use window.FPSMeter

        , createRenderer

        , utils

        , createSimulation
    )
{


    var Renderer = new createRenderer("main-canvas");

    var simulation = new createSimulation("circuit-data");




    //
    // FPS METER

    var myFpsmeter_elem = document.getElementById('canvasesdiv');
    var myFpsmeter = new window.FPSMeter(
        myFpsmeter_elem,
        window.FPSMeter.theme.transparent
    );

    // FPS METER
    //


    var center_x = 0;
    var center_y = 0;


    ///

    tick();

    function tick(in_event) {


        myFpsmeter.tickStart();


        window.requestAnimFrame( tick );

        ///


        //
        //
        //
        // UPDATE

        for (var i = 0; i < 3; ++i)
            simulation.update(0.25);

        // /UPDATE
        //
        //
        //



        //
        //
        // CAMERA

        {
            var target_x = 0;
            var target_y = 0;

            if (simulation._cars && simulation._cars.length > 0)
            {
                var index = 0;
                for (; index < simulation._cars.length; ++index)
                    if (simulation._cars[index]._alive)
                        break;

                if (index < simulation._cars.length)
                {
                    target_x = simulation._cars[index]._position.x;
                    target_y = simulation._cars[index]._position.y;
                }
            }

            var diff_x = target_x - (center_x);
            var diff_y = target_y - (center_y);

            center_x = center_x + diff_x * 0.1;
            center_y = center_y + diff_y * 0.1;

            Renderer.setCenter(-center_x+500, -center_y+300);
        }

        // CAMERA
        //
        //



        //
        //
        //
        // RENDER


            Renderer.clear();


            //
            // render circuit

            Renderer.drawLines( simulation._circuit._checkpoints, "#ffffff", 3 );
            Renderer.drawLines( simulation._circuit._walls, "#0000ff", 2 );

            Renderer.drawPoint(
                  simulation._circuit._start_position.x
                , simulation._circuit._start_position.y
                , "#ffffff"
                , 2
            );

            Renderer.drawLine(
                  simulation._circuit._start_position.x
                , simulation._circuit._start_position.y
                , simulation._circuit._start_position.x+Math.cos(simulation._circuit._start_angle) * 50.0
                , simulation._circuit._start_position.y+Math.sin(simulation._circuit._start_angle) * 50.0
                , "#ffffff"
                , 2
            );

            // render circuit
            //

            //
            // render car

            var cars = simulation._cars
            for (index in cars)
            {
                var car = cars[index];

                render_car(car, "#00ff00", true);
            }

            //

            function render_car(car, alive_color, show_sensors) {

                var position = car._position;
                var angle = car._angle;

                var size_h = 25.0;
                var size_v = 12.5;

                var positions = [
                    {x:position.x - size_h, y:position.y - size_v},
                    {x:position.x + size_h, y:position.y - size_v},
                    {x:position.x + size_h, y:position.y + size_v},
                    {x:position.x - size_h, y:position.y + size_v}
                ];

                for (var i = 0; i < 4; ++i)
                    positions[i] = utils.rotateVec2(positions[i], position, angle);

                var color = (car._alive ? alive_color :  "#ff0000");

                Renderer.drawLine( positions[0].x, positions[0].y, positions[1].x, positions[1].y, color, 6);
                Renderer.drawLine( positions[1].x, positions[1].y, positions[2].x, positions[2].y, color, 6);
                Renderer.drawLine( positions[2].x, positions[2].y, positions[3].x, positions[3].y, color, 6);
                Renderer.drawLine( positions[3].x, positions[3].y, positions[0].x, positions[0].y, color, 6);

                if (!car._alive || !show_sensors) // render sensors only if the car is alive
                    return;

                var sensors = car._sensors;

                for (var i = 0; i < sensors.length; ++i)
                {
                    var sensor = sensors[i];

                    var pos = {
                          x: sensor.line.p1.x + (sensor.line.p2.x - sensor.line.p1.x) * sensor.value
                        , y: sensor.line.p1.y + (sensor.line.p2.y - sensor.line.p1.y) * sensor.value
                    };

                    Renderer.drawLine( sensor.line.p1.x, sensor.line.p1.y, pos.x, pos.y, "#8888ff", 2);
                    // Renderer.drawLine( pos.x, pos.y, sensor.line.p2.x, sensor.line.p2.y, "#ff8888", 2);

                    Renderer.drawPoint(pos.x, pos.y, "#ffff00", 2);
                }
            }

            // render car
            //

            //
            // render trails

            var trail_colors = [ "#ffffff", "#00ff00", "#00cc00", "#009900", "#006600", "#003300" ];
            var trail_colors_index = 0;

            var trails = simulation._trails;
            for (var index = (trails.length - 1); index >= 0; --index) // from last to first
            {
                var trail = trails[index];
                var color = trail_colors[trail_colors_index++];

                Renderer.drawLineStrip(trail, color);
            }

            // render trails
            //


        // RENDER
        //
        //
        //

        //
        // HUD

        {
            var tmp_weights = null;

            var geonmes = simulation._geneticAlgo._genomes;

            if (geonmes.length > 0)
            {
                var index = 0;
                for (; index < geonmes.length; ++index)
                    if (geonmes[index].car._alive)
                        break;

                if (index < geonmes.length)
                    tmp_weights = geonmes[index].weights;
            }

            //
            //

            if (tmp_weights)
            {
                Renderer.setCenter(50, 150);

                var windex = 0;

                var topology = simulation._ann_topology

                for (var index = 1; index < topology.length; ++index)
                {
                    var prev_layer = topology[index-1];
                    var curr_layer = topology[index];

                    var prev_dec_y = (1.0 - (prev_layer / 5)) * 185;
                    var curr_dec_y = (1.0 - (curr_layer / 5)) * 185;

                    for (var pindex = 0; pindex < prev_layer; ++pindex)
                        for (var cindex = 0; cindex < curr_layer; ++cindex)
                        {
                            var curr_x = (index-1) * 75;

                            var wvalue = tmp_weights[windex++];

                            var ratio = wvalue * 10;
                            if (ratio < 0)  ratio = -ratio;
                            if (ratio < 1)  ratio = 1;

                            if (wvalue > 0)
                                Renderer.drawLine( curr_x, prev_dec_y+pindex*75, curr_x+75, curr_dec_y+cindex*75, "#ff0000", ratio);
                            else
                                Renderer.drawLine( curr_x, prev_dec_y+pindex*75, curr_x+75, curr_dec_y+cindex*75, "#0000ff", ratio);
                        }
                }

                for (var index = 0; index < topology.length; ++index)
                {
                    var curr_layer = topology[index];

                    var curr_dec_y = (1.0 - (curr_layer / 5)) * 185;

                    for (var cindex = 0; cindex < curr_layer; ++cindex)
                    {
                        var curr_x = (index) * 75;

                        Renderer.drawCircle(curr_x, curr_dec_y+cindex*75, 13, "#ffff00");
                        Renderer.drawCircle(curr_x, curr_dec_y+cindex*75, 10, "#00ff00");
                    }
                }

            }

        }

        // HUD
        //


        myFpsmeter.tick();


    }

});
