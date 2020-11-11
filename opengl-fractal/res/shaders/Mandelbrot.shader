#shader vertex
#version 310 es

precision highp float;

in vec4 position;

uniform mat4 u_MVP;

void main()
{
	//Interpolate position with Model View Projection matrix
	gl_Position = u_MVP * position;
};

#shader fragment
#version 310 es

precision highp float;

#define XMUL 5.0f
#define YMUL 2.8125f
#define XSUBT 2.501302f
#define YSUBT 1.407552f

out vec4 color;

uniform vec2 u_FramebufferSize;
uniform vec2 u_offset;
uniform vec2 u_cVals;
uniform int ITER_MAX;
uniform float u_zoom;

void main()
{
	int iter;
	// XMUL/YMUL must be == screen ratio, otherwise the image will be squished
	float cx = ((gl_FragCoord.x / u_FramebufferSize.x) * XMUL - XSUBT) * u_zoom + u_offset.x;
	float cy = ((gl_FragCoord.y / u_FramebufferSize.y) * YMUL - YSUBT) * u_zoom + u_offset.y;

	float x = 0.0f; float y = 0.0f; float xtmp = 0.0f;
	
	/*This simple for loop is responsible for all the beauty in the mandelbrot set!
	We iterate up to the maximum number of times specified -- if |x + y| is ever > 2, 
	The point is unstable and will go to infinty. If if remains bounded (<=2), it is
	in the set (or so we assume) and we color the pixel black. A higher max iteration
	excludes more points as points closer to the edge take longer and longer to escape*/
	for (iter = 0; iter < ITER_MAX; iter++)
	{
		xtmp = x * x - y * y + cx;
		y = 2.0f * x * y + cy;
		if (xtmp * xtmp + y * y > 4.0f)
			break;
		x = xtmp;
	}

	float iterf = float(iter);

	/*We color the pixel a progressively darker shade, up to completely black,
	based on how long it took to escape*/
	color = vec4(0.0f, 0.0f, (1.0f - iterf / float(ITER_MAX)), 1.0f);

};
