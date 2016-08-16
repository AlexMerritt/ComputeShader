#version 430 core
#extension GL_ARB_compute_variable_group_size : enable

layout (local_size_variable)  in;

layout(std430, binding=0) buffer Pos{
    vec4 Positions[];
};

layout(std430, binding=1) buffer V{
	vec4 Verts[];
};

layout(std430, binding=2) buffer I{
	uint Inds[];
};

void main(){
	float size = 0.0025f;
	
	uint x = gl_GlobalInvocationID.x;
	uint y = gl_GlobalInvocationID.y;
	
	uint index = x + (y * gl_NumWorkGroups.x);
	
	//uint index = gl_GlobalInvocationID.x;
	uint vLoc = index * 4;
	uint iLoc = index * 6;
	
	vec4 v = Positions[index];
	
	float xn = v.x - size;
	float yn = v.y - size;
	float xp = v.x + size;
	float yp = v.y + size;
	
	Verts[vLoc + 0] = vec4(xn, yn, v.z, 1.0f);
	Verts[vLoc + 1] = vec4(xn, yp, v.z, 1.0f);
	Verts[vLoc + 2] = vec4(xp, yn, v.z, 1.0f);
	Verts[vLoc + 3] = vec4(xp, yp, v.z, 1.0f);
	
	Inds[iLoc + 0] = vLoc + 0;
	Inds[iLoc + 1] = vLoc + 1;
	Inds[iLoc + 2] = vLoc + 2;
	Inds[iLoc + 3] = vLoc + 1;
	Inds[iLoc + 4] = vLoc + 3;
	Inds[iLoc + 5] = vLoc + 2;
}