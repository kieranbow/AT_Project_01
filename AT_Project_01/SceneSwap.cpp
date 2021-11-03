#include "SceneSwap.h"

SceneSwap::SceneSwap(SceneManager& sceneManager) : currentSceneManager(sceneManager)
{
}

void SceneSwap::onCreate(SceneData& sceneData)
{
	sceneData.gfx->SetViewMatrix(camera.GetViewMatrix());
	sceneData.gfx->SetProjectionMatrix(camera.GetProjectionMatrix());

	cube = std::make_unique<DaCube>(sceneData.gfx->GetDevice(), sceneData.gfx->GetDeviceContext());

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
	if (sceneData.keyboard->IsKeyPressed('S'))
	{
		currentSceneManager.SwitchScene(currentSceneManager.IDList.Testing);
	}
	const float speed = 0.5f;

	if (sceneData.mouse->IsLeftBtnDown())
	{
		OutputDebugStringA("Left button down\n");
	}

	if (sceneData.mouse->IsRightBtnDown())
	{
		OutputDebugStringA("Right button down\n");

		//MouseEvent event = mouse->ReadEvent();
		MouseEvent event = sceneData.mouse->ReadEvent();

		if (event.GetType() == MouseEvent::EventType::Move)
		{
			float newTargetX = static_cast<float>(event.GetPosX());
			float newTargetY = static_cast<float>(event.GetPosY());

			camera.UpdateRotation({ newTargetX * speed, newTargetY * speed, 0.0f, 0.0f });
		}
	}

	if (sceneData.keyboard->IsKeyPressed('W'))
	{
		camera.UpdatePosition({ 0.0f, 0.0f, 1.0f * speed, 0.0f });

		OutputDebugStringA("Camera moving up\n");
	}
	if (sceneData.keyboard->IsKeyPressed('S'))
	{
		camera.UpdatePosition({ 0.0f, 0.0f, -1.0f * speed, 0.0f });

		OutputDebugStringA("Camera moving down\n");
	}
	if (sceneData.keyboard->IsKeyPressed('A'))
	{
		camera.UpdatePosition({ -1.0f * speed, 0.0f, 0.0f, 0.0f });

		OutputDebugStringA("Camera moving left\n");
	}
	if (sceneData.keyboard->IsKeyPressed('D'))
	{
		camera.UpdatePosition({ 1.0f * speed, 0.0f, 0.0f, 0.0f });

		OutputDebugStringA("Camera moving right\n");
	}
	if (sceneData.keyboard->IsKeyPressed(VK_SPACE))
	{
		camera.UpdatePosition({ 0.0f, 1.0f * speed, 0.0f, 0.0f });
	}
	if (sceneData.keyboard->IsKeyPressed('Z'))
	{
		camera.UpdatePosition({ 0.0f, -1.0f * speed, 0.0f, 0.0f });
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


	camera.SetPosition({ 0.0f, 0.0f - 10.0f });

	cube->transform.SetRotation(rot, rot, 0.0f);
	cube->transform.SetPosition(0.0f, 0.0f, 0.0f);
	cube->Update(dt);
}

void SceneSwap::Draw(SceneData& sceneData)
{
	sceneData.gfx->ClearBuffer(0.0f, 1.0f, 1.0f);
	sceneData.gfx->SetViewMatrix(camera.GetViewMatrix());
	sceneData.gfx->SetProjectionMatrix(camera.GetProjectionMatrix());


	cube->Draw(sceneData.gfx);
}
