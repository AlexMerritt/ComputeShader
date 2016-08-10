#version 400

in layout(location=0) vec4 inputPosition;

uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;

void main(void)
{
	gl_Position = ViewMatrix * inputPosition;
	gl_Position = ProjectionMatrix * gl_Position;
}