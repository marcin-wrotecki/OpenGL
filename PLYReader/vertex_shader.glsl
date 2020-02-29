#version 330 

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 incol;
layout (location = 2) in vec3 offset;
layout (location = 3) in float minZ;
layout (location = 4) in float maxZ;

out float bPos;
uniform mat4 MVP;

out vec3 outcol;
out float miniZ;
out float maxiZ;
void main()
{
	bPos=aPos.z;
	miniZ=minZ;
	maxiZ=maxZ;
    outcol=incol;
    gl_Position = MVP*vec4(aPos.x, aPos.y, aPos.z, 1.0);
}