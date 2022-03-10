#version 460

in vec3 vertNormal;
in vec3 vertDir;

out vec4 fragColor;

uniform samplerCube skyboxTexture;



vec4 reflection(){

	vec3 vReflection = -reflect(vertDir , vertNormal);///use reflect function to reflect vertDir around vertNormal
	vReflection.x *= -1.0;
	return texture(skyboxTexture, vReflection);

}

vec4 refraction(){
	vec3 vRefraction = refract(-vertDir , vertNormal, 1.0/1.52);
	vRefraction.x *= -1;
	return texture(skyboxTexture, vRefraction);

	//vec3 vRefraction, use refraction function of opengl using -vertDir, vertNormal and a float (1/1.52)
	//return texture (the cube map, the refraction vector, and the ratio of coefficients of planes )

}

void main(){

//fragColor = reflection ();
fragColor = refraction();


}



