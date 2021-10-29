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

class SceneManager
{
	public:
		SceneManager();
		~SceneManager() = default;

		void Input(Keyboard& keyboard, Mouse& mouse);
		void Update(double dt);
		void Draw(Graphics* gfx);

		scene_ID Add(const std::shared_ptr<Scene>& scene, Graphics* gfx);

		void SwitchScene(scene_ID id);
		void RemoveScene(scene_ID id);

		void SetExit(bool enable);

		struct IDList
		{
			scene_ID Testing = 0;
		}IDList;


	private:
		std::unordered_map<unsigned int, std::shared_ptr<Scene>> scenes;
		std::shared_ptr<Scene> currentScene;

		scene_ID addSceneID = 0;
};
