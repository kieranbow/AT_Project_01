#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include "CameraManager.h"

#include "Player.h"
#include "Enemy.h"
#include "DefaultObject.h"

#include "Model.h"

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

		float time = 0.0f;

		// Objects
		Model skyBox;
		Model texelCube;

		std::vector<std::unique_ptr<Model>> objects;

		std::unique_ptr<DefaultObject> object;
};
