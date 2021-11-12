#pragma once
#include "Scene.h"
#include "SceneManager.h"

#include "CameraManager.h"

#include "Camera.h"
#include "Model.h"

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
	void Update(SceneData& sceneData) final;
	void Draw(SceneData& sceneData) final;

	private:
		SceneManager& currentSceneManager;
		CameraManager cameraManager;

		Light light;

		bool lookat = false;

		float rot = 0.0f;

		Model spaceMarineHelmet;
		Model sphere;
		Model sky;
		Model pyramid;
		Model single_cube;
		
		std::vector< std::unique_ptr<Model>> cube;


		std::shared_ptr<Camera> playerCamera;
		std::shared_ptr<Camera> staticCamera;
};
