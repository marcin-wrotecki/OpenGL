#version 330

out vec4 FragColor;
in vec3 outcol;


void main()
{
	

	gl_FragColor  = vec4(0.0f,0.0f, 0.0f, 1.0f);
	//gl_FragColor  = vec4(outcol.z+bPos, 0.0f,0.0f, 1.0f);
} 
