#pragma once
#include <string>

class Graphics;
class Keyboard;
class Mouse;

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
		virtual void Input(Keyboard& keyboard, Mouse& mouse) {}
		virtual void Update(double dt) {}
		virtual void Draw(Graphics* gfx) {}

		std::string name = "Default";
};

