#include "scene_lvl1.h"
#include "CollisionHandler.h"

Scenelvl1::Scenelvl1(SceneManager& sceneManager) : currentSceneManager(sceneManager)
{
}

void Scenelvl1::onCreate(SceneData& sceneData)
{
	//---------------------------------------------
	// Lighting
	directionalLight.SetLightDirection({ 0.0f, -0.3f, 0.5f });
	directionalLight.SetLightIntensity(1.0f);
	directionalLight.SetLightColor({ 0.98f, 0.95f, 0.85f });
	directionalLight.SetAmbientColor({ 0.36f, 0.49f, 0.76f });

	// Pass scene directional light to graphics so that all objects can use it for rendering
	sceneData.gfx->directionalLight = directionalLight;

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
		enemy->pTransform->SetPosition(positionX - (distance * 10.0f) / 2.0f, 1.0f, 10.0f);
		pEnemy.push_back(std::move(enemy));

		positionX += distance;
		if (positionX >= (distance * width))
		{
			positionX = 0.0f;
		}
		enemy.release();
	}

	//---------------------------------------------
	// Cameras & Camera manager
	staticCamera = std::make_shared<Camera>(sceneData.gfx->GetWindowSize().first, sceneData.gfx->GetWindowSize().second, 45.0f, 0.01f, 10000.0f, false);
	staticCamera->SetPosition({ -10.0f, 20.0f, 10.0f });
	staticCamera->SetRotation({ 1.5f, 0.0f, 0.0f });

	cameraManager.AddCamera(pPlayer->camera, CamID::player_cam);
	cameraManager.AddCamera(staticCamera, CamID::static_cam);
	cameraManager.ChangeCamera(CamID::player_cam);

	sceneData.gfx->SetViewMatrix(cameraManager.GetCurrentCameraViewMatrix());
	sceneData.gfx->SetProjectionMatrix(cameraManager.GetCurrentCameraProjectionMatrix());

	//---------------------------------------------
	// Textures
	std::string skyIR_filepath[6];
	skyIR_filepath[0].append("Assets\\Texture\\cubemap\\px_ir.png"); // East	+X
	skyIR_filepath[1].append("Assets\\Texture\\cubemap\\nx_ir.png"); // West	-X
	skyIR_filepath[2].append("Assets\\Texture\\cubemap\\py_ir.png"); // Up		+Y
	skyIR_filepath[3].append("Assets\\Texture\\cubemap\\ny_ir.png"); // Down	-Y
	skyIR_filepath[4].append("Assets\\Texture\\cubemap\\pz_ir.png"); // North	+Z
	skyIR_filepath[5].append("Assets\\Texture\\cubemap\\nz_ir.png"); // South	-Z

	Texture skyIR(sceneData.gfx);
	skyIR.LoadAndCreateCubeMap(skyIR_filepath, DXGI_FORMAT_R8G8B8A8_UNORM);
	skyIR.SetShaderResource(Bind::Texture::t3, 1u);

	std::string sky_filepath[6];
	sky_filepath[0].append("Assets\\Texture\\cubemap\\px.png"); // East		+X
	sky_filepath[1].append("Assets\\Texture\\cubemap\\nx.png"); // West		-X
	sky_filepath[2].append("Assets\\Texture\\cubemap\\py.png"); // Up		+Y
	sky_filepath[3].append("Assets\\Texture\\cubemap\\ny.png"); // Down		-Y
	sky_filepath[4].append("Assets\\Texture\\cubemap\\pz.png"); // North	+Z
	sky_filepath[5].append("Assets\\Texture\\cubemap\\nz.png"); // South	-Z

	Texture skyHDRI(sceneData.gfx);
	skyHDRI.LoadAndCreateCubeMap(sky_filepath, DXGI_FORMAT_R8G8B8A8_UNORM);
	skyHDRI.SetShaderResource(Bind::Texture::t4, 1u);

	Texture bdrfLut(sceneData.gfx);
	bdrfLut.LoadAndCreateTexture("Assets\\Texture\\integrateBrdf.png", DXGI_FORMAT_R8G8B8A8_UNORM);
	bdrfLut.SetShaderResource(Bind::Texture::t5, 1u);

	//---------------------------------------------
	// Map
	lvl1Map.LoadMap(sceneData.gfx, "Assets\\Levels\\lvl1_layout.txt");

	//---------------------------------------------
	// Game Objects
	object = std::make_unique<DefaultObject>();
	object->model->LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\Helmet_paintable_v2.obj");
	object->model->LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_PBR.cso", sceneData.gfx->inputElemDesc, sceneData.gfx->GetSizeOfInputElemDesc());
	object->model->LoadTextures(sceneData.gfx, "Assets\\Texture\\Helmet_V3_Albedo.png", DXGI_FORMAT_B8G8R8A8_UNORM_SRGB);
	object->model->LoadTextures(sceneData.gfx, "Assets\\Texture\\Helmet_V3_RMAO.png", DXGI_FORMAT_R8G8B8A8_UNORM_SRGB);
	object->model->SetPosition({ 0.0f, 0.0f, -3.0f });
	object->model->SetRotation({ 0.0f, 3.0f, 0.0f });

	skybox = std::make_unique<SkyBox>(sceneData.gfx, "Assets\\Texture\\syferfontein_0d_clear_1k.png");
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

	//if (sceneData.mouse->IsLeftBtnDown())
	//{
	//	cameraManager.ChangeCamera(CamID::player_cam);
	//}
	if (sceneData.mouse->IsRightBtnDown())
	{
		cameraManager.ChangeCamera(CamID::static_cam);
	}
	else
	{
		cameraManager.ChangeCamera(CamID::player_cam);
	}

}

void Scenelvl1::Update(SceneData& sceneData)
{
	time += 0.05f;

	//---------------------------------------------
	// Entity
	pPlayer->Update(sceneData.dt);

	for (auto& enemy : pEnemy)
	{
		enemy->LookAt(pPlayer->pTransform->GetPosition());
		//enemy->MoveTo(pPlayer->model.transform.GetPosition(), sceneData.dt);
		enemy->Update(sceneData.dt);

		//if (intersect(enemy.get(), pPlayer.get()))
		//{
		//	pPlayer->speed *= -1.0f;
		//	// https://happycoding.io/tutorials/processing/collision-detection
		//	// https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-resolution
		//}
		//else
		//{
		//	//pPlayer->speed = 0.1f;
		//}

		if (CollisionHandler::DetectAABB(enemy->collision.get(), pPlayer->pCollision.get()))
		{
			//float new_velX = pPlayer->pRigidBody->GetVelocity().x * -1.0f;
			//float new_velY = pPlayer->pRigidBody->GetVelocity().y * -1.0f;
			//float new_velZ = pPlayer->pRigidBody->GetVelocity().z * -1.0f;

			DirectX::XMFLOAT3 something = enemy->pTransform->GetPosition();
			DirectX::XMFLOAT3 anotherThing = pPlayer->pTransform->GetPosition();

			DirectX::XMVECTOR enemyPos = DirectX::XMLoadFloat3(&something);
			DirectX::XMVECTOR playerPos = DirectX::XMLoadFloat3(&anotherThing);
			DirectX::XMVECTOR subtract = DirectX::XMVectorSubtract(enemyPos, playerPos);
			DirectX::XMVECTOR length = DirectX::XMVector3Length(subtract);

			float distance = 0.0f;
			DirectX::XMStoreFloat(&distance, length);
			OutputDebugStringA(std::to_string(distance).c_str());

			//pPlayer->pRigidBody->SetVelocity({new_velX, new_velY, new_velZ});
		}
	}

	//---------------------------------------------
	// Map
	lvl1Map.Update(sceneData.dt);

	//---------------------------------------------
	// Game Objects
	object->Update(sceneData.dt);
	skybox->Update(sceneData.dt);

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
	// Map
	lvl1Map.Draw(sceneData.gfx);

	//---------------------------------------------
	// Game Objects
	object->Draw(sceneData.gfx);
	skybox->Draw(sceneData.gfx);

	//---------------------------------------------
	// Camera manager
	cameraManager.Draw(sceneData.gfx);
}
