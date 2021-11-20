#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "CameraManager.h"

#include "DefaultObject.h"
#include "SkyBox.h"

class ScenePBRShowcase : public Scene
{
	public:
		ScenePBRShowcase(SceneManager& sceneManager);
		~ScenePBRShowcase() override = default;

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

		// Lighting
		DirectionalLight directionalLight;

		// Objects
		std::unique_ptr<SkyBox> skybox;
		std::vector<std::unique_ptr<DefaultObject>> spheres;

};
