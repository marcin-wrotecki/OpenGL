#version 330 

layout (location = 0) in vec3 aPos;

layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 vtexcoord0;
layout (location = 3) in float minZ;
layout (location = 4) in float maxZ;
layout (location = 5) in float lightPower;



out vec3 FragPos;
out vec3 Normal;

uniform mat4 MVP;

out vec2 texcoord0;
out float miniZ;
out float maxiZ;
out float bPos;
out float strengthOfLight;

void main()
{
    FragPos = aPos;
    Normal =  aNormal;  
	texcoord0=vtexcoord0;
	bPos=aPos.z;
	miniZ=minZ;
	maxiZ=maxZ;
	strengthOfLight=lightPower;
    gl_Position = MVP*vec4(aPos, 1.0);
}