#version 430 core
#extension GL_ARB_compute_variable_group_size : enable

layout (local_size_variable)  in;

layout(std430, binding=0) buffer Val{
    float radi[];
};

layout(std430, binding=0) buffer results{
    float o[];
};

uniform float inc;

void main(){
	uint x  = gl_GlobalInvocationID.x;
	uint index = x + (gl_GlobalInvocationID.y * gl_NumWorkGroups.x);
	
	
	float f = radi[index];
    radi[index] = f + inc;
}