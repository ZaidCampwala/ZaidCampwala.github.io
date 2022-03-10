#ifndef SCENE3_H
#define SCENE3_H
#include "Scene.h"
#include "Vector.h"
#include "Matrix.h"
#include "TrackBall.h"
#include "SkyBox.h"

using namespace MATH;

//Forward Declarations

union SDL_Event;
class GameObject;
class Mesh;
class Shader;
class Texture;
class TrackBall;
class SkyBox;
class Camera;

class Scene3 : public Scene {
private:

	Vec3 lightSource;
	Mesh* skyboxMesh;
	Shader* skyboxShader;
	Texture* skyboxTexture;

	//TrackBall *trackball;
	Camera* camera;
	SkyBox* skybox;

	bool buildSkyBox();
public:
	explicit Scene3();
	virtual ~Scene3();

	virtual bool OnCreate() override;
	virtual void OnDestroy() override;
	virtual void Update(const float deltaTime) override;
	virtual void Render()const override;
	virtual void HandleEvents(const SDL_Event& sdlEvent) override;
};
#endif // !SCENE3_H
