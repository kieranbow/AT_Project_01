#pragma once
#include "Scene.h"
#include "SceneManager.h"

#include "DaCube.h"
#include "Camera.h"

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

		bool lookat = false;

		Camera camera;
		float rot = 0.0f;
		std::unique_ptr<Cube> solidCube;
		std::unique_ptr<Cube> liquidCube;

		std::vector<std::unique_ptr<Cube>> cubepolsion;
};
