#include "SceneManager.h"

SceneManager::SceneManager() : scenes(0), currentScene(NULL)
{
}

void SceneManager::Input(SceneData& sceneData)
{
	if (currentScene->isSceneActive)
	{
		currentScene->Input(sceneData);
	}
}

void SceneManager::Update(SceneData& sceneData)
{
	if (currentScene->isSceneActive)
	{
		currentScene->Update(sceneData);
	}
}

void SceneManager::Draw(SceneData& sceneData)
{
	if (currentScene->isSceneActive)
	{
		currentScene->Draw(sceneData);
	}
}

scene_ID SceneManager::AddScene(const std::shared_ptr<Scene>& scene, SceneData& sceneData, scene_ID _sceneID)
{
	// Inserts new scene into unordered map
	auto insert = scenes.insert(std::make_pair(_sceneID, scene));

	addSceneID++;

	// Call onCreate to create all the scene data like meshes
	insert.first->second->onCreate(sceneData);

	return addSceneID - 1;
}

void SceneManager::SwitchScene(scene_ID _sceneID)
{
	auto iter = scenes.find(_sceneID);

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

void SceneManager::RemoveScene(scene_ID _sceneID)
{
	auto iter = scenes.find(_sceneID);

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
