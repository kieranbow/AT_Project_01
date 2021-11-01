#pragma once
#include <memory>
#include <unordered_map>
#include <map>
#include <string>

#include "Scene.h"
#include "Graphics.h"
#include "Keyboard.h"
#include "Mouse.h"

using scene_ID = unsigned int;

// Description
// Scene Manager uses pass through methods to send data like input, delta time and graphics
// to any scene that is currently active. 
// The class is also responsible for creating the scenes inside the engine.
class SceneManager
{
	public:
		SceneManager();
		~SceneManager() = default;

		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;

		// Description
		// Pass data about keyboard and mouse to current scene.
		void Input(std::unique_ptr<Keyboard>& keyboard, std::unique_ptr<Mouse>& mouse);

		// Description
		// Pass delta time to current scene.
		void Update(double dt);

		// Description
		// Pass data about graphics to current scene.
		void Draw(Graphics* gfx);

		// Description
		// Inserts new scene into the unordered map and calls scene OnCreate function
		// to create the scene data.
		scene_ID AddScene(const std::shared_ptr<Scene>& scene, Graphics* gfx);

		// Description
		// Switches to a new scene using the param id.
		void SwitchScene(scene_ID id);

		// Description
		// Removes and destroys a scene from the unordered map using param id
		void RemoveScene(scene_ID id);

		struct IDList
		{
			scene_ID Testing	= 0;
			scene_ID splash	= 0;
			scene_ID ingame	= 0;
			scene_ID swap	= 0;

		}IDList;

	private:
		std::unordered_map<unsigned int, std::shared_ptr<Scene>> scenes;
		std::shared_ptr<Scene> currentScene;

		scene_ID addSceneID = 0;
};
