#version 330 core

in vec3 position;
in vec3 color;
in vec3 normal;
in vec2 texCoords;

uniform mat4 MVP;
uniform mat4 Normal;

out vec2 texCoord0;
out vec3 normal0;
out vec3 color0;
out vec3 position0;

void main()
{
	texCoord0 = texCoords;
	color0 = color;
	normal0 = (Normal * vec4(normal, 0.0)).xyz;
	position0 = vec3(Normal * vec4(position, 1.0));
	gl_Position = MVP * Normal * vec4(position, 1.0); //you must have gl_Position
}