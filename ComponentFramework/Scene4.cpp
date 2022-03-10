#include <glew.h>
#include <SDL.h>
#include <iostream>
#include "Debug.h"
#include "Scene4.h"
#include "GameObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MMath.h"
#include "Physics.h"
#include "TrackBall.h"
#include "Camera.h"


Scene4::Scene4() :  sphereMesh(nullptr), sphereShader(nullptr), sphereTexture(nullptr),object(nullptr),camera(nullptr){

	Debug::Info("Creating Scene4:", __FILE__ , __LINE__);
	camera = new Camera();
	//Create a new camera and give it a pointer
}

Scene4::~Scene4() {
	if (camera) delete camera;
	
}

bool Scene4::OnCreate() {


	camera->getSkyBox()->loadSkyBox("textures/posx.jpg", "textures/negx.jpg", 
									 "textures/posy.jpg", "textures/negy.jpg",
									  "textures/posz.jpg", "textures/negz.jpg");


	buildSphere();


	return true;
}

void Scene4::HandleEvents(const SDL_Event& sdlEvent) {
	//handle camera events
	Matrix4 move;
	move = object->getModelMatrix();
	if (sdlEvent.type == SDL_KEYDOWN) {

			switch (sdlEvent.key.keysym.sym) {
				

			case SDLK_w:
				object->setModelMatrix(move* MMath::translate(Vec3(0.0f,0.0f,0.5f)));
				break;
			case SDLK_s:
				object->setModelMatrix(move * MMath::translate(Vec3(0.0f, 0.0f, -0.5f)));

				break;
			case SDLK_a:
				object->setModelMatrix(move * MMath::translate(Vec3(0.50f, 0.0f, 0.0f)));
				break;
			case SDLK_d:
				object->setModelMatrix(move* MMath::translate(Vec3(-0.50f, 0.0f, 0.0f)));
				break;
			}
	}

	camera->HandleEvents(sdlEvent);
	object->HandleEvents(sdlEvent);
}

void Scene4::Update(const float deltaTime) {
	object->Update(deltaTime);
	camera->Update(deltaTime);
	
	
}

void Scene4::OnDestroy() {

	if (sphereMesh) delete sphereMesh, sphereMesh = nullptr;
	if (camera) delete camera, camera = nullptr;
	if (sphereShader) delete sphereShader, sphereShader = nullptr;
	if (sphereTexture) delete sphereTexture, sphereTexture = nullptr;
	if (object) delete object, object = nullptr;
}

void Scene4::Render() const {

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	camera->Render();

	GLuint program = object->getShader()->getProgram();

	glUseProgram(program);

	glUniformMatrix4fv(object->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(object->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix()* camera->getCameraRotationMatrix());

	glBindTexture(GL_TEXTURE_CUBE_MAP, camera->getSkyBox()->getTexture());

	object->Render();


	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);



	glUseProgram(0);	

}



bool Scene4::buildSphere() {
	if (ObjLoader::loadOBJ("meshes/Sphere.obj") == false) {
		return false;
	}

	sphereMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	sphereShader = new Shader("shaders/reflectionVert.glsl", "shaders/reflectionFrag.glsl");
	sphereTexture = new Texture();

	if (sphereMesh == nullptr || sphereShader == nullptr|| sphereTexture == nullptr) {
		Debug::FatalError("Couldn't create object assets", __FILE__, __LINE__);
		return false;

	}


	object = new GameObject(nullptr, sphereMesh, sphereShader, nullptr);
	if (object == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}

	object->setModelMatrix( MMath::scale(2.0f, 2.0f, 2.0f));

	
	return true;
}
