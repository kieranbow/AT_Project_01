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
}

void SceneTest::Update(double dt)
{
}

void SceneTest::Draw(Graphics* gfx)
{
	cube->Draw(gfx->GetDeviceContext());
}
