#ifndef SCENE4_H
#define SCENE4_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "TrackBall.h"
#include "SkyBox.h"

using namespace MATH;

//Forward Declarations

union SDL_Event;

class Scene4 : public Scene {
private:

	class Camera* camera;
	class GameObject* object;
	class Mesh* sphereMesh;
	class Shader* sphereShader;
	class Texture* sphereTexture;

public:
	explicit Scene4();
	virtual ~Scene4();
	bool buildSphere();
	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render()const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};
#endif // !SCENE4_H
