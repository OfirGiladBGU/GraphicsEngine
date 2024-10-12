#version 330

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
	fragColor = texture(sampler, texCoord0) * vec4(color0, 1.0) * lightColor; //you must have gl_FragColor
}
