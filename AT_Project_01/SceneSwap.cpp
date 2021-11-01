#include "SceneSwap.h"

SceneSwap::SceneSwap(SceneManager& sceneManager) : currentSceneManager(sceneManager)
{
}

void SceneSwap::onCreate(Graphics* gfx)
{
}

void SceneSwap::OnDestroy()
{
}

void SceneSwap::OnActivate()
{
	isActive = true;
}

void SceneSwap::OnDeactivate()
{
}

void SceneSwap::Input(std::unique_ptr<Keyboard>& keyboard, std::unique_ptr<Mouse>& mouse)
{
	if (keyboard->IsKeyPressed('S'))
	{
		currentSceneManager.SwitchScene(currentSceneManager.IDList.Testing);
	}
}

void SceneSwap::Update(double dt)
{
}

void SceneSwap::Draw(Graphics* gfx)
{
	
}
