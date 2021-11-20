#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "CameraManager.h"

// Entity
#include "Player.h"
#include "Enemy.h"

// Map
#include "Map.h"

// GameObjects
#include "DefaultObject.h"
#include "DirectionalLight.h"
#include "SkyBox.h"
#include "Gun.h"

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
		// Managers
		SceneManager& currentSceneManager;
		CameraManager cameraManager;

		// Cameras
		std::shared_ptr<Camera> staticCamera;

		// Entity
		std::unique_ptr<Player> pPlayer;
		std::vector<std::unique_ptr<Enemy>>	pEnemy;

		// Map
		Map lvl1Map;

		// Lighting
		DirectionalLight directionalLight;

		// GameObjects
		std::unique_ptr<DefaultObject> object;
		std::unique_ptr<DefaultObject> sphere;
		std::unique_ptr<SkyBox> skybox;

		float time = 0.0f;
};
