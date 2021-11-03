#pragma once
#include "Scene.h"
#include "SceneManager.h"

#include "DaCube.h"
#include "Camera.h"

class SceneSwap : public Scene
{
public:
	SceneSwap(SceneManager& sceneManager);
	~SceneSwap() override = default;

	void onCreate(Graphics * gfx) final;
	void OnDestroy() final;
	void OnActivate() final;
	void OnDeactivate() final;

	// Base game scene functions
	void Input(std::unique_ptr<Keyboard>& keyboard, std::unique_ptr<Mouse>& mouse) final;
	void Update(double dt) final;
	void Draw(Graphics * gfx) final;

private:
	SceneManager& currentSceneManager;

	Camera camera;

	std::unique_ptr<DaCube> cube;
};

