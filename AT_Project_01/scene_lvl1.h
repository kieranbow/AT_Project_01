#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "CameraManager.h"

#include "Player.h"

class Scenelvl1 : public Scene
{
	public:
		Scenelvl1(SceneManager& sceneManager);
		~Scenelvl1() override = default;

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
};
