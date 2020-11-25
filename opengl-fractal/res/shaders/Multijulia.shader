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
uniform float u_Exp;

void main()
{

	int iter;
	
	float zx = v_Z.x;
	float zy = v_Z.y;

	float xtmp = 0.0f;

	/*The regular Mandelbrot and Julia set shaders are optimized for an exponent we
	  know will be 2 -- when the exponent could be anything we don't have such luxury
	  and have to do more complicated, slower math*/ 
	for (iter = 0; iter < ITER_MAX; iter++)
	{
		xtmp = pow(zx * zx + zy * zy, (u_Exp / 2.0f)) * cos(u_Exp * atan(zy, zx)) + u_cVals.x;
		zy = pow(zx * zx + zy * zy, (u_Exp / 2.0f)) * sin(u_Exp * atan(zy, zx)) + u_cVals.y;
		if (xtmp * xtmp + zy * zy > 4.0f)
			break;
		zx = xtmp;
	}

	float iterf = float(iter);
	color = vec4(0.0f, 0.0f, 1.0f - iterf / float(ITER_MAX), 1.0f);

};
