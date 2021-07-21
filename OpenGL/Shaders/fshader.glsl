#version 330

in vec4 vColor;
in vec2 texCoord;

out vec4 color;

uniform sampler2D tex;

void main()
{
	color = texture(tex, texCoord);
}