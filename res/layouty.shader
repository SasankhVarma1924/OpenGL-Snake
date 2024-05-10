#shader layouty
#version 440 core

uniform float y;

void main()
{
	if (gl_VertexID == 0)
		gl_Position = vec4(-0.8, 0.9f-y, 0.0f, 1.0);
	else if (gl_VertexID == 1)
		gl_Position = vec4(0.8f, 0.9f-y, 0.0f, 1.0);
}

#shader frag2
#version 440 core

out vec4 color;

void main()
{
	color = vec4(0.74, 0.83, 0.25, 1.0);
}