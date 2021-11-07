#pragma once
#include "Scene.h"
#include "SceneManager.h"

#include "Camera.h"

class SceneSwap : public Scene
{
public:
	SceneSwap(SceneManager& sceneManager);
	~SceneSwap() override = default;

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

	Camera camera;
};
