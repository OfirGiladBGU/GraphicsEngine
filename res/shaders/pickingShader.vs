#version 330 core

in vec3 position;
in vec3 color;
in vec3 normal;
in vec2 texCoords;

uniform mat4 MVP;
uniform mat4 Normal;

//out vec2 texCoord0;
//out vec3 normal0;
//out vec3 color0;
//out vec3 position0;

void main()
{
	gl_Position = MVP * Normal * vec4(position, 1.0);
}