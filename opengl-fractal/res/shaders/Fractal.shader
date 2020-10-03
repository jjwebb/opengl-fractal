#shader vertex
#version 310 es

precision highp float;

in vec4 position;
in vec2 texCoord;

out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP * position;
   v_TexCoord = texCoord;
};

#shader fragment
#version 310 es

precision highp float;
#define ITER_MAX 200
#define XMUL 5.0f
#define XSUBT 2.501953f
//#define XSUBT 2.8125f
#define YMUL 2.8125f
#define YSUBT 1.408203f
//#define YSUBT 1.40625f

//#define XADD 0.310547f
//#define YADD -0.001953f

out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform vec2 u_FramebufferSize;
uniform vec2 u_crossHairCoord;
uniform vec2 u_offset;
uniform vec2 u_cVals;
uniform float u_zoom;
uniform float u_Exp;
uniform int u_renderToTexture;
uniform bool u_julia;
uniform sampler2D u_Texture;

void main()
{
	if (u_renderToTexture == 0)
	{
		if (gl_FragCoord.x == (u_FramebufferSize.x / 2.0f) + 0.5f && gl_FragCoord.y == (u_FramebufferSize.y / 2.0f) + 0.5f)
		{
			color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
			return;
		}
		else if (gl_FragCoord.x == 5.5f && gl_FragCoord.y == 5.5f)
		{
			color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
			return;
		}
		else if (gl_FragCoord.x == u_crossHairCoord.x || gl_FragCoord.y == u_crossHairCoord.y)
		{
			color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
			return;
		}
		else
		{
			color = texture(u_Texture, v_TexCoord);
			return;
		}
	}

	int iter;
	if (!u_julia)
	{
		// XMUL/YMUL must be == screen ratio, otherwise the image will be squished
		float cx = ((gl_FragCoord.x / u_FramebufferSize.x) * XMUL - XSUBT) * u_zoom;
		float cy = ((gl_FragCoord.y / u_FramebufferSize.y) * YMUL - YSUBT) * u_zoom;

		cx += u_offset.x;
		cy += u_offset.y;

		float x=0.0f; float y = 0.0f; float xtmp = 0.0f;

		if(u_Exp == 2.0f)
		{
			for (iter = 0; iter < ITER_MAX; iter++)
			{
				xtmp = x * x - y * y + cx;
				y = 2.0f * x * y + cy;
				if (xtmp * xtmp + y * y > 4.0f)
					break;
				x = xtmp;
			}
		}
		else
		{
			for (iter = 0; iter < ITER_MAX; iter++)
			{
				xtmp = pow(x * x + y * y, (u_Exp / 2.0f)) * cos(u_Exp * atan(y, x)) + cx;
				y = pow(x * x + y * y, (u_Exp / 2.0f)) * sin(u_Exp * atan(y, x)) + cy;
				if (xtmp * xtmp + y * y > 4.0f)
					break;
				x = xtmp;
			}
		}
	}
	else if (u_julia)
	{
		float zx = ((gl_FragCoord.x / u_FramebufferSize.x) * XMUL - XSUBT) * u_zoom;
		float zy = ((gl_FragCoord.y / u_FramebufferSize.y) * YMUL - YSUBT) * u_zoom;

		zx += u_offset.x;
		zy += u_offset.y;

		float x = 0.0f; float y = 0.0f; float xtmp = 0.0f;

		if (u_Exp == 2.0f)
		{
			for (iter = 0; iter < ITER_MAX; iter++)
			{
				xtmp = zx * zx - zy * zy + u_cVals.x;
				zy = 2.0f * zx * zy + u_cVals.y;
				if (xtmp * xtmp + zy * zy > 4.0f)
					break;
				zx = xtmp;
			}
		}
		else
		{
			for (iter = 0; iter < ITER_MAX; iter++)
			{
				xtmp = pow(zx * zx + zy * zy, (u_Exp / 2.0f)) * cos(u_Exp * atan(zy, zx)) + u_cVals.x;
				zy = pow(zx * zx + zy * zy, (u_Exp / 2.0f)) * sin(u_Exp * atan(zy, zx)) + u_cVals.y;
				if (xtmp * xtmp + zy * zy > 4.0f)
					break;
				zx = xtmp;
			}
		}
	}
	/*if (iter == ITER_MAX )
		color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	else if (iter == ITER_MAX - 1)
		color = vec4(1.0f, 1.0f, 1.0f, 1.0f);*/
	//else
	float iterf = float(iter);
	color = vec4(0.0f, 0.0f, 1.0f - iterf / float(ITER_MAX), 1.0f);
	
};
