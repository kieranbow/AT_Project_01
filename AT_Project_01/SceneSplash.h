#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "CameraManager.h"

#include "DefaultObject.h"

class SceneSplash : public Scene
{
	public:
		SceneSplash(SceneManager& sceneManager);
		~SceneSplash() override = default;

		void onCreate(SceneData & sceneData) final;
		void OnDestroy() final;
		void OnActivate() final;
		void OnDeactivate() final;

		// Base game scene functions
		void Input(SceneData & sceneData) final;
		void Update(SceneData & sceneData) final;
		void Draw(SceneData & sceneData) final;

	private:
		// Managers
		SceneManager& currentSceneManager;
		CameraManager cameraManager;

		// Cameras
		std::shared_ptr<Camera> staticCamera;

		std::unique_ptr<DefaultObject> splashscreen;
};
