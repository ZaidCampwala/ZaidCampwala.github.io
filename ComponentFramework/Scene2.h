#ifndef SCENE2_H
#define SCENE2_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "TrackBall.h"

using namespace MATH;

//Forward Declarations

union SDL_Event;
class GameObject;
class Mesh;
class Shader;
class Texture;
class TrackBall;


class Scene2 : public Scene {
private:
	GameObject* skull;
	GameObject* eye;
	GameObject* eye2;
	Vec3 lightSource;
	Mesh* skullMesh;
	Mesh* sphereMesh;
	Shader* shaderPtr;
	Texture* skullTexture;
	Texture* sphereTexture;
	Matrix4 projectionMatrix;
	Matrix4 viewMatrix;
	Vec3 cameraPos;
	TrackBall *trackball;
	bool buildSkull();
	bool buildEye();
public:
	explicit Scene2();
	virtual ~Scene2();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render()const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};
#endif // !SCENE2_H
