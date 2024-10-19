#version 330 core

in vec3 color0;
in vec3 normal0;
in vec2 texCoord0;
in vec3 position0;

uniform sampler2D sampler;
uniform vec4 lightColor;

void main()
{
	gl_FragColor = texture(sampler, texCoord0) * vec4(color0, 1.0) * lightColor; //you must have gl_FragColor
}