#include "SceneSwap.h"

SceneSwap::SceneSwap(SceneManager& sceneManager) : currentSceneManager(sceneManager)
{
}

void SceneSwap::onCreate(Graphics* gfx)
{
	gfx->SetViewMatrix(camera.GetViewMatrix());
	gfx->SetProjectionMatrix(camera.GetProjectionMatrix());

	cube = std::make_unique<DaCube>(gfx->GetDevice(), gfx->GetDeviceContext());

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
	const float speed = 0.5f;

	if (mouse->IsLeftBtnDown())
	{
		OutputDebugStringA("Left button down\n");
	}

	if (mouse->IsRightBtnDown())
	{
		OutputDebugStringA("Right button down\n");

		//MouseEvent event = mouse->ReadEvent();
		MouseEvent event = mouse->ReadEvent();

		if (event.GetType() == MouseEvent::EventType::Move)
		{
			float newTargetX = static_cast<float>(event.GetPosX());
			float newTargetY = static_cast<float>(event.GetPosY());

			camera.UpdateRotation({ newTargetX * speed, newTargetY * speed, 0.0f, 0.0f });
		}
	}

	if (keyboard->IsKeyPressed('W'))
	{
		camera.UpdatePosition({ 0.0f, 0.0f, 1.0f * speed, 0.0f });

		OutputDebugStringA("Camera moving up\n");
	}
	if (keyboard->IsKeyPressed('S'))
	{
		camera.UpdatePosition({ 0.0f, 0.0f, -1.0f * speed, 0.0f });

		OutputDebugStringA("Camera moving down\n");
	}
	if (keyboard->IsKeyPressed('A'))
	{
		camera.UpdatePosition({ -1.0f * speed, 0.0f, 0.0f, 0.0f });

		OutputDebugStringA("Camera moving left\n");
	}
	if (keyboard->IsKeyPressed('D'))
	{
		camera.UpdatePosition({ 1.0f * speed, 0.0f, 0.0f, 0.0f });

		OutputDebugStringA("Camera moving right\n");
	}
	if (keyboard->IsKeyPressed(VK_SPACE))
	{
		camera.UpdatePosition({ 0.0f, 1.0f * speed, 0.0f, 0.0f });
	}
	if (keyboard->IsKeyPressed('Z'))
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

void SceneSwap::Draw(Graphics* gfx)
{
	gfx->ClearBuffer(0.0f, 1.0f, 1.0f);
	gfx->SetViewMatrix(camera.GetViewMatrix());
	gfx->SetProjectionMatrix(camera.GetProjectionMatrix());


	cube->Draw(gfx);
}
