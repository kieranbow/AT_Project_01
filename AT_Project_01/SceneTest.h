#pragma once
#include "Scene.h"
#include "SceneManager.h"

//#include "Cube.h"
#include "Camera.h"

#include "Model.h"

struct Light
{
	Light()
	{
		ZeroMemory(this, sizeof(Light));
	}
	DirectX::XMFLOAT3 direction;
	float padding = 0;
	DirectX::XMFLOAT4 ambient;
	DirectX::XMFLOAT4 diffuse;
};


class SceneTest : public Scene
{
	public:
	SceneTest(SceneManager& sceneManager);
	~SceneTest() override = default;

	void onCreate(SceneData& sceneData) final;
	void OnDestroy() final;
	void OnActivate() final;
	void OnDeactivate() final;

	// Base game scene functions
	void Input(SceneData& sceneData) final;
	void Update(double dt) final;
	void Draw(SceneData& sceneData) final;

	private:
		SceneManager& currentSceneManager;

		Light light;

		bool lookat = false;

		Camera camera;
		Camera camera2;
		float rot = 0.0f;
		//std::unique_ptr<Cube> solidCube;
		//std::unique_ptr<Cube> liquidCube;

		Model spaceMarineHelmet;
		Model sphere;
		Model sky;
		Model pyramid;
		Model single_cube;
		
		std::vector< std::unique_ptr<Model>> cube;

		//std::vector<std::unique_ptr<Cube>> cubepolsion;
};
