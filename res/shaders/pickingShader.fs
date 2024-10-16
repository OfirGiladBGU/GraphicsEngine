#version 330 core

in vec3 color0;
in vec3 normal0;
in vec2 texCoord0;
in vec3 position0;

uniform sampler2D sampler;
uniform vec4 lightDirection;
uniform vec4 lightColor;

out vec4 fragColor;

void main()
{
	fragColor = lightColor;
}
