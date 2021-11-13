#include "scene_lvl1.h"

Scenelvl1::Scenelvl1(SceneManager& sceneManager) : currentSceneManager(sceneManager)
{
}

void Scenelvl1::onCreate(SceneData& sceneData)
{
	//---------------------------------------------
	// Entity
	pPlayer = std::make_unique<Player>(sceneData.gfx);
	
	float width = 10.0f;
	float height = 10.0f;
	float distance = 10.0f;
	float positionX = 0;

	for (int e = 0; e < 10; e++)
	{
		std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(sceneData.gfx);
		enemy->model.transform.SetPosition(positionX - (distance * 10.0f) / 2.0f, 0.0f, 0.0f);
		pEnemy.push_back(std::move(enemy));

		positionX += distance;
		if (positionX >= (distance * width))
		{
			positionX = 0.0f;
		}
	}

	//---------------------------------------------
	// Cameras
	staticCamera = std::make_shared<Camera>(sceneData.gfx->GetWindowSize().first, sceneData.gfx->GetWindowSize().second, 90.0f, 0.01f, 10000.0f, false);
	staticCamera->SetPosition({ 0.0f, 0.0f, 30.0f });
	staticCamera->SetRotation({ 0.0f, 110.0f, 0.0f });

	//---------------------------------------------
	// Camera manager
	cameraManager.AddCamera(pPlayer->camera, CamID::player_cam);
	cameraManager.AddCamera(staticCamera, CamID::static_cam);
	cameraManager.ChangeCamera(CamID::player_cam);

	sceneData.gfx->SetViewMatrix(cameraManager.GetCurrentCameraViewMatrix());
	sceneData.gfx->SetProjectionMatrix(cameraManager.GetCurrentCameraProjectionMatrix());

	//---------------------------------------------
	// Objects
	skyBox.LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\inner_sphere.obj");
	skyBox.LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_unlit.cso", sceneData.gfx->inputElemDesc, sceneData.gfx->GetSizeOfInputElemDesc());
	skyBox.LoadTextures(sceneData.gfx, "Assets\\Texture\\syferfontein_0d_clear_1k.png");
	skyBox.transform.SetScale(5000.0f, 5000.f, 5000.0f);

	floor.LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\Plane.obj");
	floor.LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_Floor.cso", sceneData.gfx->inputElemDesc, sceneData.gfx->GetSizeOfInputElemDesc());
	floor.LoadTextures(sceneData.gfx, "Assets\\Texture\\default.png");
	floor.transform.SetPosition(0.0f, -1.0f, 0.0f);
	floor.transform.SetScale(1000.0f, 0.0f, 1000.0f);

	texelCube.LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\cube_proj.obj");
	texelCube.LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_BlinnPhong.cso", sceneData.gfx->inputElemDesc, sceneData.gfx->GetSizeOfInputElemDesc());
	texelCube.LoadTextures(sceneData.gfx, "Assets\\Texture\\default.png");
	texelCube.transform.SetPosition(0.0f, 0.0f, -4.0f);
}

void Scenelvl1::OnDestroy()
{
}

void Scenelvl1::OnActivate()
{
	isSceneActive = true;
}

void Scenelvl1::OnDeactivate()
{
}

void Scenelvl1::Input(SceneData& sceneData)
{
	pPlayer->Input(sceneData.keyboard, sceneData.mouse);
}

void Scenelvl1::Update(SceneData& sceneData)
{
	time += 0.05f;

	//---------------------------------------------
	// Entity
	pPlayer->Update(sceneData.dt);

	for (auto& enemy : pEnemy)
	{
		enemy->LookAt(pPlayer->model.transform.GetPosition());
		// enemy->MoveTo(pPlayer->model.transform.GetPosition(), sceneData.dt);
		enemy->Update(sceneData.dt);
	}

	//---------------------------------------------
	// Objects
	skyBox.Update(sceneData.dt);
	floor.Update(sceneData.dt);
	texelCube.Update(sceneData.dt);

	//---------------------------------------------
	// Camera manager
	sceneData.gfx->currentCamera.SetPosition(cameraManager.GetCurrentCamera()->GetPosition());
	cameraManager.Update(sceneData.dt);
}

void Scenelvl1::Draw(SceneData& sceneData)
{
	sceneData.gfx->ClearBuffer(0.1f, 0.1f, 0.1f);
	
	//---------------------------------------------
	// Entity
	pPlayer->Draw(sceneData.gfx);

	for (auto& enemy : pEnemy)
	{
		enemy->Draw(sceneData.gfx);
	}

	//---------------------------------------------
	// Objects
	skyBox.Draw(sceneData.gfx);
	floor.Draw(sceneData.gfx);
	texelCube.Draw(sceneData.gfx);

	//---------------------------------------------
	// Camera manager
	cameraManager.Draw(sceneData.gfx);
}
