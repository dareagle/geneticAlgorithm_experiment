

define([],function()
{

	//

	var createRenderer = function (elem_id)
	{
		this._canvas = document.getElementById( elem_id );

		if (!this._canvas)
			throw new Error("failed to get the canvas");

		this._ctx = this._canvas.getContext("2d");

		this._center_x = 0;
		this._center_y = 0;
	}

	//

	createRenderer.prototype.clear = function(in_x, in_y, in_w, in_h)
	{
		// this._ctx.clearRect(0,0,800,600);

		this._ctx.fillStyle="black";

		var x = in_x || 0;
		var y = in_y || 0;
		var w = in_w || 800;
		var h = in_h || 600;

		// this._ctx.fillRect(0,0,800,600);
		this._ctx.fillRect(x,y,w,h);
 	};

	//

	createRenderer.prototype.setCenter = function(x, y)
	{
		this._center_x = x;
		this._center_y = y;
 	};


	//

	createRenderer.prototype.drawLine = function(x1, y1, x2, y2, color, size)
	{
		this._ctx.strokeStyle = color;
		this._ctx.lineWidth= size || 1;
		this._ctx.beginPath();
		this._ctx.moveTo(this._center_x+x1, this._center_y+y1);
		this._ctx.lineTo(this._center_x+x2, this._center_y+y2);
		this._ctx.stroke();
	};

	//

	createRenderer.prototype.drawThickLine = function(x1, y1, x2, y2, color)
	{
		this._ctx.strokeStyle = color;
		this._ctx.lineWidth=4;
		this._ctx.beginPath();
		this._ctx.moveTo(this._center_x+x1, this._center_y+y1);
		this._ctx.lineTo(this._center_x+x2, this._center_y+y2);
		this._ctx.stroke();
	};

	//

	createRenderer.prototype.drawLineStrip = function(arr_lines, color)
	{
		this._ctx.strokeStyle = color;
		this._ctx.lineWidth=1;
		this._ctx.beginPath();

		this._ctx.moveTo(this._center_x+arr_lines[0].x, this._center_y+arr_lines[0].y);
		for (var i = 1; i < arr_lines.length; ++i)
			this._ctx.lineTo(this._center_x+arr_lines[i].x, this._center_y+arr_lines[i].y);

		this._ctx.stroke();
	};

	//

	createRenderer.prototype.drawLines = function(arr_lines, color)
	{
		this._ctx.strokeStyle = color;
		this._ctx.lineWidth=1;
		this._ctx.beginPath();

		for (var i = 0; i < arr_lines.length; ++i)
		{
			this._ctx.moveTo(this._center_x+arr_lines[i].p1.x, this._center_y+arr_lines[i].p1.y);
			this._ctx.lineTo(this._center_x+arr_lines[i].p2.x, this._center_y+arr_lines[i].p2.y);
		}

		this._ctx.stroke();
	};

	//

	createRenderer.prototype.drawPoint = function(x, y, color)
	{
		var size = 10;
		this.drawLine(x-size, y-size, x+size, y+size, color);
		this.drawLine(x-size, y+size, x+size, y-size, color);
	};

	//

	createRenderer.prototype.drawCircle = function(x, y, radius, color)
	{
		this._ctx.beginPath();
		this._ctx.arc(this._center_x+x, this._center_y+y, radius, 0, 2 * Math.PI, false);
		this._ctx.fillStyle = color;
		this._ctx.fill();
		// this._ctx.lineWidth = size||1;
		// this._ctx.strokeStyle = color;
		// this._ctx.stroke();
	};

	//

	return createRenderer;
})
