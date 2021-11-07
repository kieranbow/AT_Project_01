#include "SceneSwap.h"

SceneSwap::SceneSwap(SceneManager& sceneManager) : currentSceneManager(sceneManager)
{
}

void SceneSwap::onCreate(SceneData& sceneData)
{
	sceneData.gfx->SetViewMatrix(camera.GetViewMatrix());
	sceneData.gfx->SetProjectionMatrix(camera.GetProjectionMatrix());

	camera.EnableCamera(true);
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

void SceneSwap::Input(SceneData& sceneData)
{
	if (sceneData.keyboard->IsKeyPressed('G'))
	{
		currentSceneManager.SwitchScene(currentSceneManager.IDList.Testing);
	}
	const float speed = 0.5f;

	if (sceneData.keyboard->IsKeyPressed('W'))
	{
		camera.UpdatePosition(camera.GetDirection().v_forward * speed);
	}
	if (sceneData.keyboard->IsKeyPressed('S'))
	{
		camera.UpdatePosition(camera.GetDirection().v_backward * speed);
	}
	if (sceneData.keyboard->IsKeyPressed('A'))
	{
		camera.UpdatePosition(camera.GetDirection().v_left * speed);
	}
	if (sceneData.keyboard->IsKeyPressed('D'))
	{
		camera.UpdatePosition(camera.GetDirection().v_right * speed);
	}
	if (sceneData.keyboard->IsKeyPressed(VK_SPACE))
	{
		camera.UpdatePosition({ 0.0f, 1.0f * speed, 0.0f, 0.0f });
	}
	if (sceneData.keyboard->IsKeyPressed('Z'))
	{
		camera.UpdatePosition({ 0.0f, -1.0f * speed, 0.0f, 0.0f });
	}

	if (sceneData.keyboard->IsKeyPressed('Q'))
	{
		camera.UpdateRotation({ 0.0f, -0.05f, 0.0f, 0.0f });
	}
	if (sceneData.keyboard->IsKeyPressed('E'))
	{
		camera.UpdateRotation({ 0.0f, 0.05f, 0.0f, 0.0f });
	}
}

void SceneSwap::Update(double dt)
{
	float rot = 0.0f;
	
	rot += 0.05f;

	if (rot > 6.28f)
	{
		rot = 0.0f;
	}

	camera.Update(dt);
}

void SceneSwap::Draw(SceneData& sceneData)
{
	sceneData.gfx->ClearBuffer(0.0f, 1.0f, 1.0f);
	sceneData.gfx->SetViewMatrix(camera.GetViewMatrix());
	sceneData.gfx->SetProjectionMatrix(camera.GetProjectionMatrix());

}
