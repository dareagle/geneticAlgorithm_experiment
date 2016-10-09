
define([
        'webgl/WebGLUtils'
    ], function(
        WebGLUtils
    )
{

    // WEBGL

	var canvas = document.getElementById("main-canvas");

    gl = WebGLUtils.setupWebGL(canvas);

    gl.viewportWidth = canvas.clientWidth;
    gl.viewportHeight = canvas.clientHeight;

    // CANVAS 2D

	var canvas_hud = document.getElementById("second-canvas");
    var ctx_hud = canvas_hud.getContext("2d");

    gl.hud = ctx_hud;

    //

    return gl;
});