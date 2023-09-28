#shader snakev
#version 440 core

layout(location = 0) in vec3 s_pos;

uniform mat4 move;
uniform vec3 col;

out vec3 fcol;

void main()
{
	fcol = col;
	gl_Position = move * vec4(s_pos,1.0);
}


#shader snakef
#version 440 core

out vec4 color;
in vec3 fcol;

void main()
{
	color = vec4(fcol, 1.0);					
}

#shader ballv
#version 440 core

layout(location = 0) in vec2 b_pos;

uniform mat4 ran;

void main()
{
	gl_Position = ran * vec4(b_pos, 0.0, 1.0);
}

#shader ballf
#version 440 core

out vec4 color;

void main()
{
	color = vec4(0.86,0.62,0.86, 1.0);
}