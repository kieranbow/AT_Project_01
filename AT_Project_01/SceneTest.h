#pragma once
#include "Scene.h"
#include "SceneManager.h"

#include "CameraManager.h"

#include "Camera.h"
#include "ModelComponent.h"

#include "Player.h"
#include "DefaultObject.h"

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

		std::unique_ptr<Player> pPlayer;

		bool lookat = false;

		float rot = 0.0f;

		ModelComponent spaceMarineHelmet;
		ModelComponent sphere;
		ModelComponent sky;
		ModelComponent pyramid;
		ModelComponent single_cube;
		
		std::vector< std::unique_ptr<DefaultObject>> cube;
		std::shared_ptr<Camera> staticCamera;
};
