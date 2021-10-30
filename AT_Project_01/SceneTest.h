#pragma once
#include "Scene.h"
#include "SceneManager.h"
#include <random>

#include "DaCube.h"
#include "Camera.h"

class SceneTest : public Scene
{
	public:
	SceneTest(SceneManager& sceneManager);
	~SceneTest() override = default;

	void onCreate(Graphics* gfx) final;
	void OnDestroy() final;
	void OnActivate() final;
	void OnDeactivate() final;

	// Base game scene functions
	void Input(Keyboard& keyboard, Mouse& mouse) final;
	void Update(double dt) final;
	void Draw(Graphics* gfx) final;

	private:
		SceneManager& currentSceneManager;

		std::unique_ptr<DaCube> solidCube;
		std::unique_ptr<DaCube> liquidCube;

		std::vector<std::unique_ptr<DaCube>> cubepolsion;
};
