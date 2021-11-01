#pragma once
#include <string>
#include <memory>

class Graphics;
class Keyboard;
class Mouse;

// Description
// Virtual class that all scenes inherit from.
// It includes all the base functions that scenes need like Update, Input, Draw.
class Scene
{
	public:
		Scene() = default;
		virtual ~Scene() = default;

		virtual void onCreate(Graphics* gfx) {}

		// Destroys the scene
		virtual void OnDestroy() = 0;

		// Called when changing into a scenes
		virtual void OnActivate() {}

		// Called when changing out of a scene
		virtual void OnDeactivate() {}

		// Base game scene functions
		virtual void Input(std::unique_ptr<Keyboard>& keyboard, std::unique_ptr<Mouse>& mouse) {}
		virtual void Update(double dt) {}
		virtual void Draw(Graphics* gfx) {}

		bool isActive = false;
};

