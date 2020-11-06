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
//#define XSUBT 2.501953f
//#define XSUBT 2.502604f
//#define XSUBT 2.8125f
#define YMUL 2.8125f
//#define YSUBT 1.408203f
//#define YSUBT 1.40625f
#define XSUBT 2.501302f
#define YSUBT 1.407552f

//#define XADD 0.310547f
//#define YADD -0.001953f

out vec4 color;

uniform vec2 u_FramebufferSize;
uniform vec2 u_offset;
uniform vec2 u_cVals;
uniform int ITER_MAX;
uniform float u_zoom;
uniform float u_Exp;

void main()
{
	int iter;
	// XMUL/YMUL must be == screen ratio, otherwise the image will be squished
	float cx = ((gl_FragCoord.x / u_FramebufferSize.x) * XMUL - XSUBT) * u_zoom;
	float cy = ((gl_FragCoord.y / u_FramebufferSize.y) * YMUL - YSUBT) * u_zoom;

	cx += u_offset.x;
	cy += u_offset.y;

	float x = 0.0f; float y = 0.0f; float xtmp = 0.0f;
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
