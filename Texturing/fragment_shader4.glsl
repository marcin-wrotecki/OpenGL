#version 330

out vec4 FragColor;
in vec2 texcoord0;
uniform sampler2D tex0;

in float trans;
void main()
{
     gl_FragColor=vec4(0,0,0,1);
	//lub
	//FragColor  = texture( tex0, texcoord0 );
	
} 
