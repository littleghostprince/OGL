#version 430 core

in vec2 fragment_uv;

out vec4 color;
uniform sampler2D textureSample;

void main()
{
//multipy color by color toget another color
	vec4 ambient= vec4(1.0,0.0,0.0,1.0);
	color = texture(textureSample, fragment_uv) * ambient;
}
