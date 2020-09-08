#shader vertex
#version 310 es

precision mediump float;

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

precision mediump float;

out vec4 color;

in vec2 v_TexCoord;
//this is a comment
uniform vec4 u_Color;
uniform sampler2D u_Texture;

void main()
{
	vec4 texColor = texture(u_Texture, v_TexCoord);
   color = texColor;
};
