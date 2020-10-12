#shader vertex
#version 310 es

precision highp float;

in vec4 position;

uniform mat4 u_MVP;

void main()
{
	gl_Position = u_MVP * position;
};

#shader fragment
#version 310 es

precision highp float;
//#define ITER_MAX 200
#define XMUL 5.0f
#define XSUBT 2.501953f
//#define XSUBT 2.8125f
#define YMUL 2.8125f
#define YSUBT 1.408203f
//#define YSUBT 1.40625f

//#define XADD 0.310547f
//#define YADD -0.001953f

out vec4 color;

uniform vec2 u_FramebufferSize;
uniform vec2 u_offset;
uniform vec2 u_cVals;
uniform int ITER_MAX;
uniform float u_zoom;

void main()
{

	int iter;
	float zx = ((gl_FragCoord.x / u_FramebufferSize.x) * XMUL - XSUBT) * u_zoom;
	float zy = ((gl_FragCoord.y / u_FramebufferSize.y) * YMUL - YSUBT) * u_zoom;

	zx += u_offset.x;
	zy += u_offset.y;

	float xtmp = 0.0f;

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
