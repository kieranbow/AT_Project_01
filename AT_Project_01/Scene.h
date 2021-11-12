#pragma once
#include <string>
#include <memory>

class Graphics;
class Keyboard;
class Mouse;

// Description
// Data struct that contrains pointers to each class which includes:
// Keyboard, Mouse & Graphics.
struct SceneData
{
	Keyboard* keyboard;
	Mouse* mouse;
	Graphics* gfx;
	float dt;
};

// Description
// Virtual class that all scenes inherit from.
// It includes all the base functions that scenes need like Update, Input, Draw.
class Scene
{
	public:
		Scene() = default;
		virtual ~Scene() = default;

		virtual void onCreate(SceneData& sceneData) {}

		// Destroys the scene
		virtual void OnDestroy() = 0;

		// Called when changing into a scenes
		virtual void OnActivate() {}

		// Called when changing out of a scene
		virtual void OnDeactivate() {}

		// Base game scene functions
		virtual void Input(SceneData& sceneData) {}
		virtual void Update(SceneData& sceneData) {}
		virtual void Draw(SceneData& sceneData) {}

		bool isSceneActive = false;
};
