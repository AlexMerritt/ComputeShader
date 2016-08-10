#version 430

layout(local_size_x=256) in;
layout(std430, binding=0) buffer pblock { vec4 positions[]; };
layout(std430, binding=1) buffer oblock { vec4 orientations[]; };
layout(std430, binding=2) buffer vblock { vec4 velocities[]; };
layout(std430, binding=3) buffer avblock { vec4 angularVelocities[]; };
uniform int numPoints;

float hash( float n ) { return fract(sin(n)*43758.5453); }

void main()
{
    uint index = gl_GlobalInvocationID.x;
    if (index >= numPoints)
        return;
    float fi = float(index+1) / float(numPoints);

    positions[index] = vec4(hash(fi), hash(fi*3.), hash(fi*17.), 1.);

    orientations[index] = vec4(hash(fi*11.), hash(fi*5.), hash(fi*7.), hash(fi*19.));

    vec3 randv = vec3(hash(fi*19.), hash(fi*17.), hash(fi*7.));
    vec3 motiondir = 2.*(vec3(-.5) + randv); // isotropic, go in all directions
    motiondir = pow(length(motiondir), 7.) * normalize(motiondir);
    velocities[index] = vec4(.05*motiondir, 0.);

    vec3 randav = vec3(hash(fi*11.), hash(fi*5.), hash(fi*7.));
    vec3 rotangle = 2.*(vec3(-.5) + randav); // isotropic, go in all directions
    rotangle = pow(rotangle, vec3(13.));
    angularVelocities[index] = normalize(vec4(rotangle, 30.));
}