#version 430 core

layout  (local_size_variable)  in;

layout(std430, binding=0) buffer Pos{
    vec4 Positions[];
};

layout(std430, binding=1) buffer V{
	vec4 Verts[];
};


void main(){
	float size = 0.025f;
	uint index = gl_GlobalInvocationID.x;
	uint vLoc = index * 4;
	uint iLoc = index * 6;
	
	vec4 v = Positions[gl_GlobalInvocationID.x];
	
	float xn = v.x - size;
	float yn = v.y - size;
	float xp = v.x + size;
	float yp = v.y + size;
	
	/*
	Verts[vLoc + 0] = vec4(xn, yn, v.z, v.w);
	Verts[vLoc + 1] = vec4(xp, yp, v.z, v.w);
	Verts[vLoc + 2] = vec4(xp, yn, v.z, v.w);
	*/
	
	Verts[vLoc + 0] = vec4(xn, yn, 0.0f, 1.0f);
	Verts[vLoc + 1] = vec4(xn, yp, 0.0f, 1.0f);
	Verts[vLoc + 2] = vec4(xp, yn, 0.0f, 1.0f);
	Verts[vLoc + 3] = vec4(xp, yp, 0.0f, 1.0f);
	
	//Verts[vLoc + 3] = vec4(xp, yp, v.z, v.w);
	
	Inds[iLoc + 0] = vLoc + 0;
	Inds[iLoc + 1] = vLoc + 1;
	Inds[iLoc + 2] = vLoc + 2;
	Inds[iLoc + 3] = vLoc + 1;
	Inds[iLoc + 4] = vLoc + 3;
	Inds[iLoc + 5] = vLoc + 2;
}