#include "SceneTest.h"

SceneTest::SceneTest(SceneManager& sceneManager) : currentSceneManager(sceneManager)
{
}

void SceneTest::onCreate(Graphics* gfx)
{
	std::mt19937 random_engine(
		static_cast<float>(std::random_device{}()));

	camera.SetPosition(0.0f, 0.0f, 0.0f);

	gfx->SetViewMatrix(camera.GetViewMatrix());
	gfx->SetProjectionMatrix(camera.GetViewMatrix());

	solidCube = std::make_unique<DaCube>(gfx->GetDevice(), gfx->GetDeviceContext());
	liquidCube = std::make_unique<DaCube>(gfx->GetDevice(), gfx->GetDeviceContext());

	solidCube->SetPosition(1.0f, 1.0f, 1.0f);
	solidCube->SetRotation(5.0f, 10.0f, 0.0f);
	solidCube->SetScale(0.5f, 0.5f, 0.5f);

	liquidCube->SetPosition(0.0f, 3.0f, 3.0f);
	liquidCube->SetRotation(34.0f, 10.0f, 70.0f);

	for (int i = 0; i < 10; i++)
	{
		float rand_x = random_engine() % 10;
		float rand_y = random_engine() % 5;
		float rand_z = random_engine() % 2;

		float rot_x = random_engine() % 2;
		float rot_y = random_engine() % 5;
		float rot_z = random_engine() % 10;

		cubepolsion.push_back(std::make_unique<DaCube>(gfx->GetDevice(), gfx->GetDeviceContext()));
		cubepolsion.at(i)->SetPosition(rand_x, rand_y, rand_z);
		cubepolsion.at(i)->SetRotation(rot_x, rot_y, rot_z);
		cubepolsion.at(i)->SetScale(rand_z, rand_z, rand_z);
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

void SceneTest::Input(Keyboard& keyboard, Mouse& mouse)
{
	if (keyboard.IsKeyPressed('Q'))
	{
		currentSceneManager.SwitchScene(currentSceneManager.IDList.swap);
	}

	if (mouse.IsRightBtnDown())
	{
		Mouse::MouseEvent event = mouse.ReadEvent();

		if (event.GetType() == Mouse::MouseEvent::EventType::Move)
		{
			camera.UpdateRotation(
				static_cast<float>(event.GetPosX()) * 0.01f,
				static_cast<float>(event.GetPosY()) * 0.01f,
				0.0f);
		}
		OutputDebugStringA("Camera moving with mouse\n");
	}

	const float cameraSpeed = 0.02f;

	if (keyboard.IsKeyPressed('W'))
	{
		//camera.UpdatePosition(camera.GetFowardVector() * cameraSpeed);
		camera.backforward += cameraSpeed;
		OutputDebugStringA("Camera moving up\n");
	}
	if (keyboard.IsKeyPressed('S'))
	{
		//camera.UpdatePosition(camera.GetBackwardVector() * cameraSpeed);
		camera.backforward -= cameraSpeed;
		OutputDebugStringA("Camera moving down\n");
	}
	if (keyboard.IsKeyPressed('A'))
	{
		//camera.UpdatePosition(camera.GetLeftVector() * cameraSpeed);
		camera.strife -= cameraSpeed;
		OutputDebugStringA("Camera moving left\n");
	}
	if (keyboard.IsKeyPressed('D'))
	{
		//camera.UpdatePosition(camera.GetRightVector() * cameraSpeed);
		camera.strife += cameraSpeed;
		OutputDebugStringA("Camera moving right\n");
	}
	if (keyboard.IsKeyPressed(VK_SPACE))
	{
		//camera.UpdatePosition(DirectX::XMVectorSet(0.0f, cameraSpeed, 0.0f, 0.0f));
	}
	if (keyboard.IsKeyPressed('Z'))
	{
		//camera.UpdatePosition(DirectX::XMVectorSet(0.0f, -cameraSpeed, 0.0f, 0.0f));
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
	gfx->SetProjectionMatrix(camera.GetViewMatrix());


	solidCube->Draw(gfx);
	liquidCube->Draw(gfx);

	for (auto& cubes : cubepolsion)
	{
		cubes->Draw(gfx);
	}
}
