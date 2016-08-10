#version 430 core
#extension GL_ARB_compute_variable_group_size : enable

layout (local_size_variable)  in;

layout(std430, binding=0) buffer Pos{
    vec4 Positions[];
};

layout(std430, binding=1) buffer Speed{
	vec4 Speeds[];
};

void main(){
	uint index = gl_GlobalInvocationID.x;
	uint vLoc = index * 4;
	uint iLoc = index * 6;
	
	vec4 v = Positions[index];
	vec4 s = Speeds[index];
	
	Positions[index] = v + s;
}