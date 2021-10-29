#pragma once
#include "Scene.h"
#include "SceneManager.h"

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
	void Input(Keyboard & keyboard, Mouse & mouse) final;
	void Update(double dt) final;
	void Draw(Graphics * gfx) final;

private:
	SceneManager& currentSceneManager;

};

