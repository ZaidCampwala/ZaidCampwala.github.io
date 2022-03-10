#include <glew.h>
#include <SDL.h>
#include <iostream>
#include "Debug.h"
#include "Scene2.h"
#include "GameObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MMath.h"
#include "Physics.h"
#include "TrackBall.h"


Scene2::Scene2() : skull(nullptr), eye(nullptr), eye2(nullptr), skullMesh(nullptr), shaderPtr(nullptr), skullTexture(nullptr),
						sphereMesh(nullptr), sphereTexture(nullptr) {

	trackball = nullptr;
	Debug::Info("Created Scene2:" , __FILE__, __LINE__);
}

Scene2::~Scene2() {

}

bool Scene2::OnCreate() {
	trackball = new TrackBall();
	lightSource = Vec3(5.0f, 0.0f, 0.0f);

	projectionMatrix = MMath::perspective(45.0f, (16.0f / 9.0f), 0.5f, 100.0f);
	cameraPos = Vec3(0.0f, 0.0f, 10.0f);
	viewMatrix = MMath::lookAt(cameraPos, Vec3(0.0f, 0.0f, 0.0f), Vec3(0.0f, 1.0f, 0.0f));

	buildSkull();
	skull->setModelMatrix(MMath::rotate(0.0f, Vec3(0.0f, 1.0f, 0.0f)));
	
	buildEye();
	Matrix4 rot = (MMath::rotate(-90.0f, Vec3(0.0f, -4.0f, 0.0f)));
	eye->setModelMatrix(MMath::translate(Vec3(-0.570f, 0.20f, 0.60f)) 
		* (MMath::scale(Vec3(-0.35f, -0.35f, -0.2f))* rot));

	eye2->setModelMatrix(MMath::translate(Vec3(0.570f, 0.20f, 0.60f)) 
		* (MMath::scale(Vec3(-0.35f, -0.35f, -0.2f)) * rot));



	return true;
}

void Scene2::HandleEvents(const SDL_Event& sdlEvent) {
	trackball->HandleEvents(sdlEvent);

}

void Scene2::Update(const float deltaTime) {
	skull->Update(deltaTime);
	eye->Update(deltaTime);

	eye2->Update(deltaTime);
}

void Scene2::OnDestroy() {
	if (skullMesh) delete skullMesh, skullMesh = nullptr;
	if (sphereMesh) delete sphereMesh, sphereMesh = nullptr;
	if (skullTexture) delete skullTexture, skullTexture = nullptr;
	if (sphereTexture) delete sphereTexture, sphereTexture = nullptr;
	if (shaderPtr) delete shaderPtr, shaderPtr = nullptr;
	if (skull) delete skull, skull = nullptr;
	if (eye) delete eye, eye = nullptr;
	if (eye2) delete eye2, eye2 = nullptr;

}

void Scene2::Render() const {
	skull->setModelMatrix(trackball->getMatrix4());

	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
//Draw the Scene

	GLuint program = skull->getShader()->getProgram();
	
	GLuint program1 = eye->getShader()->getProgram();
	GLuint program2 = eye2->getShader()->getProgram();
	glUseProgram(program);
	
	glUseProgram(program1);
	glUseProgram(program2);

	//pass uniforms
	glUniformMatrix4fv(skull->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE , projectionMatrix);
	glUniformMatrix4fv(skull->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, viewMatrix);
	glUniform3fv(skull->getShader()->getUniformID("lightPos"), 1, lightSource);


	skull->Render();
	
	eye->Render();
	eye2->Render();

	glUseProgram(0);

}

bool Scene2::buildSkull() {
	if (ObjLoader::loadOBJ("meshes/Skull.obj") == false) {
		return false;
	}

	skullMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderPtr = new Shader("shaders/texturePhongVert.glsl", "shaders/texturePhongFrag.glsl");
	skullTexture = new Texture();

	if (skullMesh == nullptr || shaderPtr == nullptr || skullTexture == nullptr) {
		Debug::FatalError("Couldn't create object assets", __FILE__, __LINE__);
		return false;

	}
	

	if (skullTexture->LoadImage("textures/skull_texture.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}

	skull = new GameObject(nullptr, skullMesh, shaderPtr, skullTexture);
	if (skull == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	return true;
}

bool Scene2::buildEye() {
	
	if (ObjLoader::loadOBJ("meshes/Sphere.obj") == false) {
		return false;
	}

	sphereMesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shaderPtr = new Shader("shaders/texturePhongVert.glsl", "shaders/texturePhongFrag.glsl");
	sphereTexture = new Texture();

	if (sphereMesh == nullptr || shaderPtr == nullptr || sphereTexture == nullptr) {
		Debug::FatalError("Couldn't create object assets", __FILE__, __LINE__);
		return false;

	}

	if (sphereTexture->LoadImage("textures/evilEye.jpg") == false) {
		Debug::FatalError("Couldn't load texture", __FILE__, __LINE__);
		return false;
	}

	eye = new GameObject(skull, sphereMesh, shaderPtr, sphereTexture);
	if (eye == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}

	eye2 = new GameObject(skull, sphereMesh, shaderPtr, sphereTexture);
	if (eye2 == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}
	return true;
}