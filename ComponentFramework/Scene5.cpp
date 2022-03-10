#include <glew.h>
#include <SDL.h>
#include <iostream>
#include "Debug.h"
#include "Scene5.h"
#include "GameObject.h"
#include "ObjLoader.h"
#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"
#include "MMath.h"
#include "Physics.h"
#include "TrackBall.h"
#include "Noise.h"
#include "Camera.h"


Scene5::Scene5(): camera(nullptr), object(nullptr), mesh(nullptr), shader(nullptr)
{	
	Debug::Info("Creating Scene5:", __FILE__, __LINE__);

	camera = new Camera();
}

Scene5::~Scene5() {
	if (camera)delete camera;

}

bool Scene5::OnCreate() {
	
	camera->getSkyBox()->loadSkyBox("textures/StarSkyboxPosx.png", "textures/StarSkyboxNegx.png",
		"textures/StarSkyboxPosy.png", "textures/StarSkyboxNegy.png",
		"textures/StarSkyboxPosz.png", "textures/StarSkyboxNegz.png");

	buildSphere();

	noiseTextureID = CreateNoise3D();

	return true;
}

void Scene5::HandleEvents(const SDL_Event& sdlEvent) {
	camera->HandleEvents(sdlEvent);

}

void Scene5::Update(const float deltaTime) {
	offset.x += 0.001f;
	offset.y += 0.001f;
	offset.z -= 0.005f;

	static float rotate;
	rotate += 1.0f;
	Matrix4 rot = (MMath::rotate(rotate, Vec3(0.0f, 0.05f, 0.0f)));

	object->setModelMatrix(MMath::translate(Vec3(-0.570f, 0.20f, 0.60f))
		* (MMath::scale(Vec3(2.0f, 2.0f, 2.0f)) * rot));

	Physics::RigidBodyRotation(*object, deltaTime);
	object->Update(deltaTime);

	
}

void Scene5::Render() const {
	
	
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	camera->Render();

	GLuint program = object->getShader()->getProgram();

	glUseProgram(program);

	glUniformMatrix4fv(object->getShader()->getUniformID("projectionMatrix"), 1, GL_FALSE, camera->getProjectionMatrix());
	glUniformMatrix4fv(object->getShader()->getUniformID("viewMatrix"), 1, GL_FALSE, camera->getViewMatrix());
	glUniformMatrix4fv(object->getShader()->getUniformID("offset"), 1, GL_FALSE, offset);

	glBindTexture(GL_TEXTURE_3D, noiseTextureID);

	object->Render();

	glBindTexture(GL_TEXTURE_3D, 0);

	////I was not able to free textures
	
	/// 
	///free();
	/// 
	glUseProgram(0);



}

void Scene5::OnDestroy() {
	if (mesh) delete mesh, mesh = nullptr;
	if (camera) delete camera, camera = nullptr;
	if (shader) delete shader, shader = nullptr;
	if (object) delete object, object = nullptr;
	DeleteNoise3D();
}

bool Scene5::buildSphere() {
	if (ObjLoader::loadOBJ("meshes/Sphere.obj") == false) {
		return false;
	}

	mesh = new Mesh(GL_TRIANGLES, ObjLoader::vertices, ObjLoader::normals, ObjLoader::uvCoords);
	shader = new Shader("shaders/noiseVert.glsl", "shaders/noiseFrag.glsl");
	

	if (mesh == nullptr || shader == nullptr) {
		Debug::FatalError("Couldn't create object assets", __FILE__, __LINE__);
		return false;

	}
	

	object = new GameObject(nullptr, mesh, shader, nullptr);
	if (object == nullptr) {
		Debug::FatalError("GameObject could not be created", __FILE__, __LINE__);
		return false;
	}



	return true;
}
