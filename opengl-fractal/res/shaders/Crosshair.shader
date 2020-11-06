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

out vec4 color;

in vec2 v_TexCoord;

uniform vec2 u_FramebufferSize;
uniform vec2 u_crossHairCoord;
uniform sampler2D u_Texture;

void main()
{
	if (gl_FragCoord.x == (u_FramebufferSize.x / 2.0f) + 0.5f && gl_FragCoord.y == (u_FramebufferSize.y / 2.0f) + 0.5f)
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
};
