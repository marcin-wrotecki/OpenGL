#version 330

out vec4 FragColor;
in vec2 texcoord0;
uniform sampler2D tex0;

void main()
{
     //gl_FragColor=vec4(0.0f,1.0f,0.0f,1.0f);
	//lub
	FragColor  = texture( tex0, texcoord0 );
	
} 
