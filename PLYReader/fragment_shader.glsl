#version 330

out vec4 FragColor;
in vec3 outcol;
in float bPos;
in float miniZ;
in float maxiZ;

void main()
{
	
	float temp = abs(maxiZ-miniZ);
	float decider=(abs(bPos-maxiZ)/temp);
	
		if(decider<0.2)
					gl_FragColor  = vec4(mod(decider,0.2)*5, 0.0f,0, 1.0f);
		else if(decider<0.4)
							gl_FragColor  = vec4(1.0f, mod(decider,0.2)*5,0, 1.0f);		
		else if(decider<0.6)
					gl_FragColor  = vec4(1.0-mod(decider,0.2)*5, 1.0f, 0.0, 1.0f);
					
		else if(decider<0.8)
					gl_FragColor  = vec4(0.0f, 1.0f,mod(decider,0.2)*5,1.0f);
		else
					gl_FragColor  = vec4(0.0f, 1.0-mod(decider,0.2)*5,1.0f,1.0f);

	
	
	
	/*else if(decider<2)
		gl_FragColor  = vec4(0.0f, decider-1.0,0.0f, 1.0f);
	else
		gl_FragColor  = vec4(decider-2.0,0.0f, 0.0f, 1.0f);
	//gl_FragColor  = vec4(outcol.z+bPos, 0.0f,0.0f, 1.0f);*/
} 
