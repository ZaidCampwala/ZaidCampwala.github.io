#include <glew.h>
#include <SDL.h>
#include <iostream>
#include "Debug.h"
#include "Scene3.h"
#include "GameObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MMath.h"
#include "Physics.h"
#include "TrackBall.h"
#include "Camera.h"


Scene3::Scene3() : skyboxMesh(nullptr), skyboxShader(nullptr), skyboxTexture(nullptr),camera(nullptr), skybox(nullptr){
	

	Debug::Info("Created Scene2:" , __FILE__, __LINE__);
}

Scene3::~Scene3() {

}

bool Scene3::OnCreate() {
	lightSource = Vec3(5.0f, 0.0f, 0.0f);
	camera = new Camera();


	buildSkyBox();

	return true;
}

void Scene3::HandleEvents(const SDL_Event& sdlEvent) {
	camera->HandleEvents(sdlEvent);
}

void Scene3::Update(const float deltaTime) {
	camera->Update(deltaTime);
	skybox->Update(deltaTime);

}

void Scene3::OnDestroy() {

	if (skyboxMesh) delete skyboxMesh, skyboxMesh = nullptr;
	if (camera) delete camera, camera = nullptr;
	if (skyboxTexture) delete skyboxTexture, skyboxTexture = nullptr;
	if (skyboxShader) delete skyboxShader, skyboxShader = nullptr;
	if (skybox) delete skybox, skybox = nullptr;
}

void Scene3::Render() const {

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
//Draw the Scene

	camera->Render();

	GLuint program1 = skybox->getShader()->getProgram();

	glUseProgram(program1);

	glUniformMatrix4fv(skybox->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(skybox->getShader()->getUniformID("cameraRotationMatrix"), 1, GL_FALSE, camera->getCameraRotationMatrix());

	glBindTexture(GL_TEXTURE_CUBE_MAP, camera->getSkyBox()->getTexture());

	skybox->Render();


	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);



	glUseProgram(0);

}

bool Scene3::buildSkyBox() {
	if (ObjLoader::loadOBJ("meshes/Cube.obj") == false) {
		return false;
	}

	skyboxMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	skyboxShader = new Shader("shaders/skyBoxVert.glsl", "shaders/skyBoxFrag.glsl");
	skyboxTexture = new Texture();
	skybox = new SkyBox();

	if (skyboxMesh == nullptr || skyboxShader == nullptr || skyboxTexture == nullptr || skybox == nullptr) {
		Debug::FatalError("Couldn't create object assets", __FILE__, __LINE__);
		return false;

	}

	if (skybox->loadSkyBox("textures/posx.jpg", "textures/negx.jpg", "textures/posy.jpg", "textures/negy.jpg"
		, "textures/posz.jpg", "textures/negz.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}
	return true;
}

