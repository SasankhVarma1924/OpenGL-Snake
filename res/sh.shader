#shader vertex1
#version 440 core

layout(location=0) in vec3 vpos;
layout(location=1) in vec3 vcolor;

uniform mat4 transform;

out vec3 fcolor;

void main()
{
    fcolor=vcolor;
    gl_Position = transform * vec4(vpos,1.0);
    //gl_Position = vec4(vpos,1.0);
}

#shader fragment1
#version 440 core

out vec4 color;
in vec3 fcolor;

void main()
{
    color = vec4(fcolor,1.0);
}