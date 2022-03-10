# version 460
in vec3 vertNormal;
in vec3 lightDir0;
in vec3 lightDir1;
in vec3 eyeDir;
out vec4 fragColor0;


void main(){
	vec4 ks0 = vec4(0.3, 0.0, 0.9, 0.2);
	vec4 kd0 = vec4(0.4, 0.2, 0.01, 0.0);
	vec4 ka =  0.1 * kd0 ;

	vec4 ks1 = vec4(0.0, 0.9, 0.5, 0.9);
	vec4 kd1 = vec4(0.1, 0.2, 0.01, 0.01);

	float diff0 = max(dot(vertNormal, lightDir0), 0.0);
	float diff1 = max(dot(vertNormal, lightDir1), 0.0);

	vec3 reflection0 = normalize(reflect(-lightDir0, vertNormal));
	vec3 reflection1 = normalize(reflect(-lightDir1, vertNormal));

	float spec0 = max(dot(eyeDir, reflection0), 0.0);
	float spec1 = max(dot(eyeDir, reflection1), 0.0);

	if(diff0 > 0.0){
		spec0 = pow(spec0, 14.0);
}
		if(diff1 > 0.0){
	spec1 = pow(spec1, 14.0);
	}


  
	fragColor0 = ka + ((diff0 * kd0) + (spec0 * ks0 )) + ((diff1 * kd1) + (spec1 * ks1));

}