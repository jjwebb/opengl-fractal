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
	//vec4 texColor = texture(u_Texture, v_TexCoord);
   //color = texColor;
   float x=0.0f;
   for (int i = 0; i < 1000; i++)
   {
      x = pow(x, 2.0f);
   }
	//color = texture(u_Texture, v_TexCoord);
  color = vec4(0.0f, x / 255.0f, 0.0f, 1.0f);
};
