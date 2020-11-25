#shader vertex
#version 310 es

precision highp float;

in vec4 position;

uniform mat4 u_MVP;
uniform vec2 u_offset;

out vec2 v_Z;

void main()
{
	gl_Position = u_MVP * position;
	v_Z = position.xy + u_offset;
};

#shader fragment
#version 310 es

precision highp float;

in vec2 v_Z;

out vec4 color;

uniform vec2 u_cVals;
uniform int ITER_MAX;

void main()
{

	int iter;

	float zx = v_Z.x;
	float zy = v_Z.y;

	float xtmp = 0.0f;

	/*The same idea as the mandelbrot loop -- except our constant C is the point selected
	from the mandelbrot set and Z0 is the screen coordinate that would have been our value C
	if we were rendering the Mandelbrot set.*/
	for (iter = 0; iter < ITER_MAX; iter++)
	{
		xtmp = zx * zx - zy * zy + u_cVals.x;
		zy = 2.0f * zx * zy + u_cVals.y;
		if (xtmp * xtmp + zy * zy > 4.0f)
			break;
		zx = xtmp;
	}
	
	float iterf = float(iter);
	color = vec4(0.0f, 0.0f, 1.0f - iterf / float(ITER_MAX), 1.0f);

};
