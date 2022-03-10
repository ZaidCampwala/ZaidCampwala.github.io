#version 460
layout (location = 0) in vec4 vVertex;

out vec3 vertDir;


uniform mat4 projectionMatrix; 
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform sampler3D noiseTexture;
uniform vec3 offset;

void main(){
	vertDir = vec3(viewMatrix * modelMatrix * vVertex );////Not sure if this is correct
	vec3 vertex = vec3(vVertex); 
	vec4 noiseVec = texture(noiseTexture, (vertex * 0.25)+ (0.1, offset));

	gl_Position = projectionMatrix* viewMatrix* modelMatrix* (vVertex + noiseVec) ;
}