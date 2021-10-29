#include "SceneManager.h"

SceneManager::SceneManager() : scenes(0), currentScene(NULL)
{
}

void SceneManager::Input(Keyboard& keyboard, Mouse& mouse)
{
	if (currentScene->isActive)
	{
		currentScene->Input(keyboard, mouse);
	}
}

void SceneManager::Update(double dt)
{
	if (currentScene->isActive)
	{
		currentScene->Update(dt);
	}
}

void SceneManager::Draw(Graphics* gfx)
{
	if (currentScene->isActive)
	{
		currentScene->Draw(gfx);
	}
}

scene_ID SceneManager::AddScene(const std::shared_ptr<Scene>& scene, Graphics* gfx)
{
	// Inserts new scene into unordered map
	auto insert = scenes.insert(std::make_pair(addSceneID, scene));

	addSceneID++;

	// Call onCreate to create all the scene data like meshes
	insert.first->second->onCreate(gfx);

	return addSceneID - 1;
}

void SceneManager::SwitchScene(scene_ID id)
{
	auto iter = scenes.find(id);

	// Loops through the map finding the scene id that matches search id.
	if (iter != scenes.end())
	{
		// Deactive the last scene
		if (currentScene)
		{
			currentScene->OnDeactivate();
		}

		// Assigns new scene to current scene
		currentScene = iter->second;

		// Activiate that current scene
		currentScene->OnActivate();
	}
}

void SceneManager::RemoveScene(scene_ID id)
{
	auto iter = scenes.find(id);

	// Loops through the map finding the scene id that matches search id.
	if (iter != scenes.end())
	{
		// Removes scene's pointer 
		if (currentScene == iter->second)
		{
			currentScene = NULL;
		}
		// Destroys the contents of the scene
		iter->second->OnDestroy();

		// Erases scene from the map
		scenes.erase(iter);
	}
}
