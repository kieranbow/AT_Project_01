#include "SceneTest.h"

SceneTest::SceneTest(SceneManager& sceneManager) : currentSceneManager(sceneManager)
{
}

void SceneTest::onCreate(SceneData& sceneData)
{
	sceneData.gfx->SetViewMatrix(camera.GetViewMatrix());
	sceneData.gfx->SetProjectionMatrix(camera.GetViewMatrix());

	solidCube = std::make_unique<DaCube>(sceneData.gfx->GetDevice(), sceneData.gfx->GetDeviceContext());
	liquidCube = std::make_unique<DaCube>(sceneData.gfx->GetDevice(), sceneData.gfx->GetDeviceContext());

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

		cubepolsion.push_back(std::make_unique<DaCube>(sceneData.gfx->GetDevice(), sceneData.gfx->GetDeviceContext()));
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

void SceneTest::Input(SceneData& sceneData)
{
	if (sceneData.keyboard->IsKeyPressed('F'))
	{
		currentSceneManager.SwitchScene(currentSceneManager.IDList.swap);
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

void SceneTest::Draw(SceneData& sceneData)
{
	sceneData.gfx->SetViewMatrix(camera.GetViewMatrix());
	sceneData.gfx->SetProjectionMatrix(camera.GetProjectionMatrix());


	solidCube->Draw(sceneData.gfx);
	liquidCube->Draw(sceneData.gfx);

	for (auto& cubes : cubepolsion)
	{
		cubes->Draw(sceneData.gfx);
	}
}
