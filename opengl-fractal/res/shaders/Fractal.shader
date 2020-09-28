#shader vertex
#version 100

precision highp float;

attribute vec4 position;
attribute vec2 texCoord;

varying vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP * position;
   v_TexCoord = texCoord;
};

#shader fragment
#version 100

precision highp float;
#define ITER_MAX 200

//varying vec4 color;

varying vec2 v_TexCoord;
//this is a comment
//uniform vec4 u_Color;
uniform vec2 u_FramebufferSize;
uniform vec2 u_crossHairCoord;
uniform vec2 u_offset;
uniform vec2 u_cVals;
uniform float u_zoom;
uniform float u_Exp;
uniform int u_renderToTexture;
uniform bool u_julia;
uniform sampler2D u_Texture;
//uniform sampler2D u_Texture;

void main()
{
	if (u_renderToTexture == 0)
	{	//In glsl version 100 it seems gl_FragCoord is represented by whole steps rather than half steps
		//(x.0f instead of x.5f)
		if (gl_FragCoord.x == (u_FramebufferSize.x / 2.0f) && gl_FragCoord.y == (u_FramebufferSize.y / 2.0f))
		{
			gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
			return;
		}
		else if (gl_FragCoord.x == 5.0f && gl_FragCoord.y == 5.0f)
		{
			gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
			return;
		}
		else if (gl_FragCoord.x == u_crossHairCoord.x - 0.5f || gl_FragCoord.y == u_crossHairCoord.y - 0.5f)
		{
			gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
			return;
		}
		else
		{
			gl_FragColor = texture2D(u_Texture, v_TexCoord);
			return;
		}
	}
	//float x1 = ( gl_FragCoord.x +  u_offset.x) / (u_zoom*u_zoom) + (3.0f * u_FramebufferSize.x * u_zoom * u_zoom - 3.0f * u_FramebufferSize.x) / (4.8f * u_zoom * u_zoom) - gl_FragCoord.x;
	//float y1 = ( gl_FragCoord.y +  u_offset.y) / (u_zoom*u_zoom) + (1.5f * u_FramebufferSize.y * u_zoom * u_zoom - 1.5f * u_FramebufferSize.y) / (3.0f * u_zoom * u_zoom) - gl_FragCoord.y;
	
	//vec4 texgl_FragColor = texture(u_Texture, v_TexCoord);
   //gl_FragColor = texgl_FragColor;
	//float cx0 = ((((gl_FragCoord.x + x1) / u_FramebufferSize.x)) * 4.8f - 3.0f) * u_zoom; //translate screen coords to -3 to 1.8, 4.8
	//float cy0 = ((((gl_FragCoord.y + u_offset.y) / u_FramebufferSize.y)) * 3.0 - 1.5f) * u_zoom;
	//gl_FragColor = vec4(gl_FragCoord.x/u_FramebufferSize.x, gl_FragCoord.y/u_FramebufferSize.y, 0.0f, 1.0f);
	//float cx = ((((gl_FragCoord.x+ x1) / u_FramebufferSize.x)) * 4.8f - 3.0f) * u_zoom/*+ 2*u_offset.x / u_FramebufferSize.x*/; //translate screen coords to -3 to 1.8, 4.8
	//float cy = ((((gl_FragCoord.y + y1) / u_FramebufferSize.y)) * 3.0 - 1.5f) * u_zoom/*+ 2*u_offset.y / u_FramebufferSize.y*/; //1.5 to -1.5, 3.0
	
	int iter;
	if (!u_julia)
	{
		float cx = (((((gl_FragCoord.x /*+ u_offset.x*/) / u_FramebufferSize.x)) * 4.8f - 3.0f) + 0.598333f) * u_zoom;
		float cy = (((((gl_FragCoord.y /*+ u_offset.y*/) / u_FramebufferSize.y)) * 3.0f - 1.5f) + 0.001667f) * u_zoom;
		//Adding 0.598333f and 0.00167f make the starting point equal to 0 and makes zoom scale correctly
		cx += u_offset.x;
		cy += u_offset.y;
			//range of 4.8/3.0 = 1.6, same aspect ratio as window is locked to. 4.0/3.0 resulted in squished image 
		//cy = cy0 - cy;
		//cx = cx0 - cx;
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
		float zx = (((((gl_FragCoord.x /*+ u_offset.x*/) / u_FramebufferSize.x)) * 4.8f - 3.0f) + 0.598333f) * u_zoom;
		float zy = (((((gl_FragCoord.y /*+ u_offset.y*/) / u_FramebufferSize.y)) * 3.0f - 1.5f) + 0.001667f) * u_zoom;
		//Adding 0.598333f and 0.00167f make the starting point equal to 0 and makes zoom scale correctly
		zx += u_offset.x;
		zy += u_offset.y;
		//range of 4.8/3.0 = 1.6, same aspect ratio as window is locked to. 4.0/3.0 resulted in squished image 
		//cy = cy0 - cy;
		//cx = cx0 - cx;
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
		gl_FragColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	else if (iter == ITER_MAX - 1)
		gl_FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);*/
	//else
	float iterf = float(iter);
		gl_FragColor = vec4(0.0f, 0.0f, 1.0f - iterf / float(ITER_MAX), 1.0f);
	
	
};
