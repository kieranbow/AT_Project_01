#include "SceneTest.h"

SceneTest::SceneTest(SceneManager& sceneManager) : currentSceneManager(sceneManager)
{
}

void SceneTest::onCreate(Graphics* gfx)
{
	gfx->SetViewMatrix(camera.GetViewMatrix());
	gfx->SetProjectionMatrix(camera.GetViewMatrix());

	solidCube = std::make_unique<DaCube>(gfx->GetDevice(), gfx->GetDeviceContext());
	liquidCube = std::make_unique<DaCube>(gfx->GetDevice(), gfx->GetDeviceContext());

	solidCube->transform.SetPosition(1.0f, 1.0f, 1.0f);
	solidCube->transform.SetRotation(5.0f, 10.0f, 0.0f);

	liquidCube->transform.SetPosition(0.0f, 3.0f, 3.0f);
	liquidCube->transform.SetRotation(34.0f, 10.0f, 70.0f);

	float width = 10.0f;
	float height = 10.0f;

	float distance = 10.0f;

	int amount = 1000;

	float positionX = 0;
	float positionY = 0;
	float positionZ = 0;

	for (int i = 0; i < amount; i++)
	{

		cubepolsion.push_back(std::make_unique<DaCube>(gfx->GetDevice(), gfx->GetDeviceContext()));
		cubepolsion.at(i)->transform.SetScale(1.5f, 1.5f, 1.5f);
		cubepolsion.at(i)->transform.SetPosition(positionX, positionY, positionZ);

		positionX += distance;

		if (positionX >= (distance * width))
		{
			positionX = 0.0f;
			positionY += distance;

			if (positionY >= (distance * height))
			{
				positionY = 0.0f;
				positionZ += distance;
			}
		}
	}
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

void SceneTest::Input(std::unique_ptr<Keyboard>& keyboard, std::unique_ptr<Mouse>& mouse)
{
	if (keyboard->IsKeyPressed('F'))
	{
		currentSceneManager.SwitchScene(currentSceneManager.IDList.swap);
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

void SceneTest::Update(double dt)
{
	static_cast<float>(dt);

	camera.Update(dt);

	solidCube->Update(dt);
	liquidCube->Update(dt);

	for (auto& cubes : cubepolsion)
	{
		cubes->Update(dt);
	}
}

void SceneTest::Draw(Graphics* gfx)
{
	gfx->SetViewMatrix(camera.GetViewMatrix());
	gfx->SetProjectionMatrix(camera.GetProjectionMatrix());


	solidCube->Draw(gfx);
	liquidCube->Draw(gfx);

	for (auto& cubes : cubepolsion)
	{
		cubes->Draw(gfx);
	}
}
