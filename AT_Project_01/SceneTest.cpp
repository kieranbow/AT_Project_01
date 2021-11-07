#include "SceneTest.h"
#include "Model_loader.h"

SceneTest::SceneTest(SceneManager& sceneManager) : currentSceneManager(sceneManager)
{
}

void SceneTest::onCreate(SceneData& sceneData)
{
	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	UINT ied_size = static_cast<UINT>(std::size(ied));

	sceneData.gfx->SetViewMatrix(camera.GetViewMatrix());
	sceneData.gfx->SetProjectionMatrix(camera.GetViewMatrix());

	light.direction = { 0.25f, 0.5f, -1.0f };
	light.ambient = { 0.2f, 0.2f, 0.2f, 1.0f };
	light.diffuse = { 1.0f, 1.0f, 1.0f, 1.0f };

	camera.SetPosition({ 0.0f, 0.0f, -20.0f });
	camera2.SetPosition({ 10.0f, -20.0f, -20.0f });


	spaceMarineHelmet.LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\Helmet_paintable_v2.obj");
	spaceMarineHelmet.LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VertexShader.cso", L"..\\x64\\Debug\\PixelShader.cso", ied, ied_size);
	spaceMarineHelmet.LoadTextures(sceneData.gfx, "Assets\\Texture\\Helmet_V3_Albedo.png");

	sphere.LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\sphere.obj");
	sphere.LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VertexShader.cso", L"..\\x64\\Debug\\PixelShader.cso", ied, ied_size);
	sphere.LoadTextures(sceneData.gfx, "Assets\\Texture\\icon.png");
	sphere.transform.SetPosition(20.0f, 0.0f, -10.0f);

	sky.LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\inner_sphere.obj");
	sky.LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VertexShader.cso", L"..\\x64\\Debug\\PixelShader.cso", ied, ied_size);
	sky.LoadTextures(sceneData.gfx, "Assets\\Texture\\sky.png");
	sky.transform.SetScale(5000.0f, 5000.f, 5000.0f);

	pyramid.LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\pyramid.obj");
	pyramid.LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VertexShader.cso", L"..\\x64\\Debug\\PixelShader.cso", ied, ied_size);
	pyramid.LoadTextures(sceneData.gfx, "Assets\\Texture\\default.png");

	single_cube.LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\cube.obj");
	single_cube.LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VertexShader.cso", L"..\\x64\\Debug\\PixelShader.cso", ied, ied_size);
	single_cube.LoadTextures(sceneData.gfx, "Assets\\Texture\\grass.png");

	//solidCube = std::make_unique<Cube>(sceneData.gfx);
	// liquidCube = std::make_unique<Cube>(sceneData.gfx);

	//solidCube->transform.SetPosition(1.0f, 1.0f, 1.0f);
	//solidCube->transform.SetRotation(5.0f, 10.0f, 0.0f);
	// solidCube->transform.SetScale(5000.0f, 5000.f, 5000.0f);

	//liquidCube->transform.SetPosition(0.0f, 3.0f, 3.0f);
	//liquidCube->transform.SetRotation(34.0f, 10.0f, 70.0f);

	float width = 10.0f;
	float height = 10.0f;

	float distance = 100.0f;

	int amount = 1000;

	float positionX = 0;
	float positionY = 0;
	float positionZ = 0;

	ModelLoader loader("Assets\\Model\\cube_proj.obj");


	for (int i = 0; i < amount; i++)
	{
		std::unique_ptr temp = std::make_unique<Model>();
		temp->LoadMesh(sceneData.gfx, loader.GetVertices(), loader.GetIndices());
		temp->LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VertexShader.cso", L"..\\x64\\Debug\\PixelShader.cso", ied, ied_size);
		temp->LoadTextures(sceneData.gfx, "Assets\\Texture\\1x1.png");

		cube.push_back(std::move(temp));
		cube.at(i)->transform.SetScale(5.0f, 5.0f, 5.0f);
		cube.at(i)->transform.SetPosition(positionX - (distance * 10) / 2, positionY - (distance * 10) / 2, positionZ - (distance * 10) / 2);

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

	//for (int i = 0; i < amount; i++)
	//{

	//	cubepolsion.push_back(std::make_unique<Cube>(sceneData.gfx));
	//	cubepolsion.at(i)->transform.SetScale(5.0f, 5.0f, 5.0f);
	//	cubepolsion.at(i)->transform.SetPosition(positionX - (distance * 10) / 2, positionY - (distance * 10) / 2, positionZ - (distance * 10) / 2);

	//	positionX += distance;

	//	if (positionX >= (distance * width))
	//	{
	//		positionX = 0.0f;
	//		positionY += distance;

	//		if (positionY >= (distance * height))
	//		{
	//			positionY = 0.0f;
	//			positionZ += distance;
	//		}
	//	}
	//}
}

void SceneTest::OnDestroy()
{
}

void SceneTest::OnActivate()
{
	isActive = true;
	camera.EnableCamera(true);
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

	const float speed = 1.0f;

	if (sceneData.mouse->IsLeftBtnDown())
	{
		lookat = true;
	}

	if (sceneData.mouse->IsRightBtnDown())
	{
		
		lookat = false;
		//MouseEvent event = mouse->ReadEvent();
		MouseEvent event = sceneData.mouse->ReadEvent();

		if (event.GetType() == MouseEvent::EventType::Move)
		{
			float newTargetX = static_cast<float>(event.GetPosX());
			float newTargetY = static_cast<float>(event.GetPosY());

			camera.UpdateRotation({ newTargetX * speed, newTargetY * speed, 0.0f, 0.0f });
		}
	}

	if (!sceneData.mouse->IsRightBtnDown())
	{
		
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

	if (sceneData.keyboard->IsKeyPressed('R'))
	{
		camera2.EnableCamera(true);
		camera.EnableCamera(false);
	}

	if (sceneData.keyboard->IsKeyPressed('T'))
	{
		camera2.EnableCamera(false);
		camera.EnableCamera(true);
	}

}

void SceneTest::Update(double dt)
{
	static_cast<float>(dt);

	rot += 0.05f;


	if (lookat)
	{
		camera.SetLookAt({ 0.0f, 0.0f, 0.0f });
	}

	camera.Update(dt);
	camera2.Update(dt);

	//model.transform.SetRotationAxis(2.0f * rot);
	spaceMarineHelmet.Update(dt);
	sphere.Update(dt);
	sky.Update(dt);

	pyramid.transform.SetPosition(15.0f, 15.0f, 0.0f);
	pyramid.transform.SetRotationAxis(0.5f * rot);
	pyramid.Update(dt);

	single_cube.transform.SetRotation(0.0f, 0.0f, 0.5f * rot);
	single_cube.transform.SetPosition(-15.0f, -15.0f, 0.0f);
	single_cube.Update(dt);

	for (auto& cubes : cube)
	{
		cubes->transform.SetRotation(0.14 * rot , 0.2f * rot , 1.0f * rot);
		cubes->Update(dt);
	}

	//solidCube->Update(dt);
	// liquidCube->Update(dt);

	//for (auto& cubes : cubepolsion)
	//{
	//	// cubes->transform.SetRotation(3.14 * rot , 2.0f * rot , 10.0f * rot);
	//	cubes->Update(dt);
	//}
}

void SceneTest::Draw(SceneData& sceneData)
{
	sceneData.gfx->ClearBuffer(0.1f, 0.1f, 0.1f);

	if (camera.IsActive())
	{
		sceneData.gfx->SetViewMatrix(camera.GetViewMatrix());
		sceneData.gfx->SetProjectionMatrix(camera.GetProjectionMatrix());
	}

	if (camera2.IsActive())
	{
		sceneData.gfx->SetViewMatrix(camera2.GetViewMatrix());
		sceneData.gfx->SetProjectionMatrix(camera2.GetProjectionMatrix());
	}


	spaceMarineHelmet.Draw(sceneData.gfx);
	sphere.Draw(sceneData.gfx);
	sky.Draw(sceneData.gfx);
	pyramid.Draw(sceneData.gfx);
	single_cube.Draw(sceneData.gfx);

	for (auto& cubes : cube)
	{
		cubes->Draw(sceneData.gfx);
	}

	//solidCube->Draw(sceneData.gfx);
	// liquidCube->Draw(sceneData.gfx);

	//for (auto& cubes : cubepolsion)
	//{
	//	cubes->Draw(sceneData.gfx);
	//}
}
