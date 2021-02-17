#shader vertex
#version 310 es

precision highp float;

in vec4 position;

uniform mat4 u_MVP;
uniform vec2 u_offset;

out vec2 v_C;

void main()
{
	gl_Position = u_MVP * position;
	v_C = position.xy + u_offset;
};

#shader fragment
#version 310 es

precision highp float;

in vec2 v_C;

out vec4 color;

uniform int ITER_MAX;
uniform float u_Exp;

void main()
{
	int iter;

	float cx = v_C.x;
	float cy = v_C.y;

	float x = 0.0f; float y = 0.0f; float y2 = 0.0f; float xy = 0.0f; float xtmp = 0.0f;

	/*The regular Mandelbrot and Julia set shaders are optimized for an exponent we
	  know will be 2 -- when the exponent could be anything we don't have such luxury
	  and have to do more complicated, slower math*/
	for (iter = 0; iter < ITER_MAX; iter++)
	{
		xtmp = pow(x * x + y * y, (u_Exp / 2.0f)) * cos(u_Exp * atan(y, x)) + cx;
		y = pow(x * x + y * y, (u_Exp / 2.0f)) * sin(u_Exp * atan(y, x)) + cy;
		if (xtmp * xtmp + y * y > 4.0f)
			break;
		x = xtmp;
	}

	float iterf = float(iter);
	color = vec4(0.0f, 0.0f, 1.0f - iterf / float(ITER_MAX), 1.0f);

};
