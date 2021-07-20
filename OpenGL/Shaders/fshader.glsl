#version 330

in vec4 vColor;

out vec4 color;

void main()
{
	color = vec4(0.53, 0.0, 0.69, 1.0);
	color = vec4(0.75, 0.72, 1.0, 1.0);
	color = vColor;
}