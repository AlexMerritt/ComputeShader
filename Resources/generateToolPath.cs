#version 430 core
#extension GL_ARB_compute_variable_group_size : enable

layout (local_size_variable)  in;

layout(std430, binding=0) buffer Pos{
    float radi[];
};

layout(std430, binding=1) buffer Out{
	vec4 points[];
};

uniform float levelHeight;
uniform float numDegree;

void main()
{
	const float tao = 6.2820f;
	uint x = gl_GlobalInvocationID.x;
	uint y = gl_GlobalInvocationID.y;
	
	uint index = x + (y * gl_NumWorkGroups.x);
	
	float pcent =  x / numDegree;
	float angle = pcent * tao;
	float radius = radi[index];
	float fy = y;
	
	//Path
	vec4 o = vec4(cos(angle) * radius, (pcent * levelHeight) + (y * levelHeight), sin(angle) * radius, 0.0f);
	// Concenteric circles
	//vec4 o = vec4(cos(angle) * radius, (y * levelHeight), sin(angle) * radius, 0.0f);
	
    points[index] = o;
}