#shader vertex
#version 310 es

precision highp float;

in vec4 position;
//layout(location = 1) in vec2 texCoord;

//out vec2 v_TexCoord;

uniform mat4 u_MVP;

void main()
{
   gl_Position = u_MVP * position;
   //v_TexCoord = texCoord;
};

#shader fragment
#version 310 es

precision highp float;
#define ITER_MAX 200

out vec4 color;

//in vec2 v_TexCoord;
//this is a comment
uniform vec4 u_Color;
uniform vec2 u_FramebufferSize;
uniform vec2 u_crossHairCoord;
uniform vec2 u_offset;
uniform float u_zoom;
//uniform sampler2D u_Texture;

void main()
{
	if (gl_FragCoord.x == (u_FramebufferSize.x / 2.0f) + 0.5f  && gl_FragCoord.y == (u_FramebufferSize.y / 2.0f) + 0.5f)
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

	//float x1 = ( gl_FragCoord.x +  u_offset.x) / (u_zoom*u_zoom) + (3.0f * u_FramebufferSize.x * u_zoom * u_zoom - 3.0f * u_FramebufferSize.x) / (4.8f * u_zoom * u_zoom) - gl_FragCoord.x;
	//float y1 = ( gl_FragCoord.y +  u_offset.y) / (u_zoom*u_zoom) + (1.5f * u_FramebufferSize.y * u_zoom * u_zoom - 1.5f * u_FramebufferSize.y) / (3.0f * u_zoom * u_zoom) - gl_FragCoord.y;
	
	//vec4 texColor = texture(u_Texture, v_TexCoord);
   //color = texColor;
	//float cx0 = ((((gl_FragCoord.x + x1) / u_FramebufferSize.x)) * 4.8f - 3.0f) * u_zoom; //translate screen coords to -3 to 1.8, 4.8
	//float cy0 = ((((gl_FragCoord.y + u_offset.y) / u_FramebufferSize.y)) * 3.0 - 1.5f) * u_zoom;
	//color = vec4(gl_FragCoord.x/u_FramebufferSize.x, gl_FragCoord.y/u_FramebufferSize.y, 0.0f, 1.0f);
	//float cx = ((((gl_FragCoord.x+ x1) / u_FramebufferSize.x)) * 4.8f - 3.0f) * u_zoom/*+ 2*u_offset.x / u_FramebufferSize.x*/; //translate screen coords to -3 to 1.8, 4.8
	//float cy = ((((gl_FragCoord.y + y1) / u_FramebufferSize.y)) * 3.0 - 1.5f) * u_zoom/*+ 2*u_offset.y / u_FramebufferSize.y*/; //1.5 to -1.5, 3.0
	
																																
	float cx = (((((gl_FragCoord.x /*+ u_offset.x*/) / u_FramebufferSize.x)) * 4.8f - 3.0f) + 0.598333f) * u_zoom;
	float cy = (((((gl_FragCoord.y /*+ u_offset.y*/) / u_FramebufferSize.y)) * 3.0f - 1.5f) + 0.001667f) * u_zoom;
	//Adding 0.598333f and 0.00167f make the starting point equal to 0 and makes zoom scale correctly
	cx += u_offset.x;
	cy += u_offset.y;
		//range of 4.8/3.0 = 1.6, same aspect ratio as window is locked to. 4.0/3.0 resulted in squished image 
	//cy = cy0 - cy;
	//cx = cx0 - cx;
	float x=0.0f; float y = 0.0f; float xtmp = 0.0f;
	int iter;
	for (iter = 0; iter < ITER_MAX; iter++)
	{
		xtmp = x * x - y * y + cx;
		y = 2.0f * x * y + cy;
		if (xtmp * xtmp + y * y > 4.0f)
			break;
		x = xtmp;
	}
	/*if (iter == ITER_MAX )
		color = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	else if (iter == ITER_MAX - 1)
		color = vec4(1.0f, 1.0f, 1.0f, 1.0f);*/
	//else
	float iterf = float(iter);
		color = vec4(0.0f, 0.0f, 1.0f - iterf / float(ITER_MAX), 1.0f);
	
	
};
