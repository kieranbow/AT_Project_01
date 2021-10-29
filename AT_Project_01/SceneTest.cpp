#include "SceneTest.h"

SceneTest::SceneTest(SceneManager& sceneManager) : currentSceneManager(sceneManager)
{
}

void SceneTest::onCreate(Graphics* gfx)
{
	cube = std::make_unique<DaCube>(gfx->GetDevice(), gfx->GetDeviceContext());
}

void SceneTest::OnDestroy()
{
}

void SceneTest::OnActivate()
{
	isActive = true;
}

void SceneTest::OnDeactivate()
{
}

void SceneTest::Input(Keyboard& keyboard, Mouse& mouse)
{
	if (keyboard.IsKeyPressed('K'))
	{
		OutputDebugStringA("Hello");
	}
	if (keyboard.IsKeyPressed('W'))
	{
		currentSceneManager.SwitchScene(currentSceneManager.IDList.swap);
	}
}

void SceneTest::Update(double dt)
{
	std::string time;
	time.append("Delta Time: ");
	time.append(std::to_string(dt) + "\n");
	//OutputDebugStringA(time.c_str());
}

void SceneTest::Draw(Graphics* gfx)
{
	cube->Draw(gfx->GetDeviceContext());
}
