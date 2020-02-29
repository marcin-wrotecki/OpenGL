#version 330

out vec4 FragColor;

in vec3 Normal;  
in vec3 FragPos;  
  
uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
in vec2 texcoord0;
uniform sampler2D tex0;

in float bPos;
in float miniZ;
in float maxiZ;
in float strengthOfLight;
void main()
{

		float temp = abs(maxiZ-miniZ);
		float decider=(abs(bPos-maxiZ)/temp);
		vec3 color=vec3(0,0,0);
	
		if(decider<0.2)
					color  = vec3(mod(decider,0.2)*5, 0.0f,0);
		else if(decider<0.4)
					color = vec3(1.0f, mod(decider,0.2)*5,0);		
		else if(decider<0.6)
					color  = vec3(1.0-mod(decider,0.2)*5, 1.0f, 0.0);
					
		else if(decider<0.8)
					color  = vec3(0.0f, 1.0f,mod(decider,0.2)*5);
		else
					color  = vec3(0.0f, 1.0-mod(decider,0.2)*5,1.0f);

    // ambient
    float ambientStrength = strengthOfLight;
    vec3 ambient = ambientStrength * lightColor;
  	
    // diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
  
    float specularStrength = strengthOfLight;
	vec3 viewDir = normalize(viewPos-FragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),32);
	vec3 specular = specularStrength * spec * lightColor;
	//color=vec3(1.0,0,0);
    vec3 result = (ambient + diffuse + specular ) * color;
	gl_FragColor  = vec4(result,1.0);
	//	gl_FragColor  = vec4(result*texture( tex0, texcoord0),1.0);


} 