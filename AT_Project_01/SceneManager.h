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

struct SceneID
{
	const static scene_ID splash	= 0u;
	const static scene_ID Testing	= 1u;
	const static scene_ID swap		= 2u;
	const static scene_ID lvl_1		= 3u;
	const static scene_ID lvl_3		= 4u;
	const static scene_ID lvl_4		= 5u;
	const static scene_ID ending	= 6u;
	const static scene_ID PBR		= 7u;
};

// Scene Manager uses pass through methods to send data like input, delta time and graphics
// to any scene that is currently active. The class is also responsible for creating the scenes inside the engine.
class SceneManager
{
	public:
		SceneManager();
		~SceneManager() = default;

		SceneManager(const SceneManager&) = delete;
		SceneManager& operator=(const SceneManager&) = delete;

		// Pass data about keyboard and mouse to current scene.
		void Input(SceneData& sceneData);

		// Pass delta time to current scene.
		void Update(SceneData& sceneData);

		// Pass data about graphics to current scene.
		void Draw(SceneData& sceneData);

		// Inserts new scene into the unordered map and calls scene OnCreate function
		// to create the scene data.
		scene_ID AddScene(const std::shared_ptr<Scene>& scene, SceneData& sceneData, scene_ID _sceneID);

		// Switches to a new scene using the param id.
		void SwitchScene(scene_ID _sceneID);

		// Removes and destroys a scene from the unordered map using param id
		void RemoveScene(scene_ID _sceneID);

	private:
		std::unordered_map<scene_ID, std::shared_ptr<Scene>> scenes;
		std::shared_ptr<Scene> currentScene;

		scene_ID addSceneID = 0;
};
