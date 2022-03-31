#include "scene_lvl1.h"
#include "CollisionHandler.h"

#include <iostream>

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

	// Pass the scene's directional light to graphics so that all objects can use it for rendering
	sceneData.gfx->directionalLight = directionalLight;

	//---------------------------------------------
	// Entity
	pPlayer = std::make_unique<Player>(sceneData.gfx);
	pPlayer->pTransform->SetPosition(0.0f, 0.0f, -10.0f);
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
	object->model->LoadTextures(sceneData.gfx, "Assets\\Texture\\Helmet_V3_RMAO.png", DXGI_FORMAT_R8G8B8A8_UNORM);
	object->model->SetPosition({ 0.0f, 0.0f, -3.0f });
	object->model->SetRotation({ 0.0f, 3.0f, 0.0f });


	Material_PBR blue_rubber;
	blue_rubber.ambientOcculsion = 1.0f;
	blue_rubber.baseColor = { 0.1f, 0.1f, 1.0f, 1.0f };
	blue_rubber.metallic = 0.0f;
	blue_rubber.roughness = 0.7f;

	Material_PBR gold;
	gold.ambientOcculsion = 1.0f;
	gold.baseColor = { 1.0f, 0.766f, 0.336f, 1.0f };
	gold.metallic = 1.0f;
	gold.roughness = 0.1f;

	sphere = std::make_unique<DefaultObject>();
	sphere->model->SetPBRMaterial(blue_rubber);
	sphere->model->LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\sphere.obj");
	sphere->model->LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_PBRMaterial.cso", sceneData.gfx->inputElemDesc, sceneData.gfx->GetSizeOfInputElemDesc());

	sphere->model->SetPosition({ 0.0f, 0.0f, -30.0f });

	skybox = std::make_unique<SkyBox>(sceneData.gfx, "Assets\\Texture\\syferfontein_0d_clear_1k.png");

	float sphere_width = 5.0f;
	float sphere_distance = 2.0f;
	float sphere_positionX = 0;
	float sphere_positionY = 0;

	Material_PBR aluminum;
	aluminum.ambientOcculsion = 1.0f;
	aluminum.baseColor = { 0.913f, 0.921f, 0.925f, 1.0f };
	aluminum.metallic = 0.0f;
	aluminum.roughness = 0.0f;

	float metallic = 0.0f;
	float roughness = 0.0f;

	for (int i = 0; i < 5; i++)
	{
		std::unique_ptr<DefaultObject> sphere = std::make_unique<DefaultObject>();

		//blue_rubber.metallic = metallic;
		blue_rubber.roughness = roughness;

		sphere->model->SetPBRMaterial(blue_rubber);
		sphere->model->LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\sphere.obj");
		sphere->model->LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_PBRMaterial.cso", sceneData.gfx->inputElemDesc, sceneData.gfx->GetSizeOfInputElemDesc());
		sphere->transform->SetPosition(sphere_positionX, sphere_positionY, 0.0f);
		spheres.push_back(std::move(sphere));

		sphere_positionX += distance;
		if (sphere_positionX >= (distance * sphere_width))
		{
			sphere_positionX = 0.0f;
			sphere_positionY += distance;
		}
		sphere.release();
		//metallic += 0.25;
		roughness += 0.25;
	}
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

		// https://happycoding.io/tutorials/processing/collision-detection
		// https://learnopengl.com/In-Practice/2D-Game/Collisions/Collision-resolution

		XMVECTOR normal;
		float depthColl;
		int fColl;
		if (CollisionHandler::AABBIntersect(enemy->collision->min, enemy->collision->max, pPlayer->pCollision->min, pPlayer->pCollision->max, normal, depthColl, fColl))
		{
			pPlayer->pRigidBody->SetVelocity({ 0.0f, 0.0f, 0.0f });
		}


		//if (CollisionHandler::DetectAABB(enemy->collision.get(), pPlayer->pCollision.get()))
		//{
		//	//float new_velX = pPlayer->pRigidBody->GetVelocity().x * -1.0f;
		//	//float new_velY = pPlayer->pRigidBody->GetVelocity().y * -1.0f;
		//	//float new_velZ = pPlayer->pRigidBody->GetVelocity().z * -1.0f;

		//	//DirectX::XMFLOAT3 something = enemy->pTransform->GetPosition();
		//	//DirectX::XMFLOAT3 anotherThing = pPlayer->pTransform->GetPosition();

		//	//DirectX::XMVECTOR enemyPos = DirectX::XMLoadFloat3(&something);
		//	//DirectX::XMVECTOR playerPos = DirectX::XMLoadFloat3(&anotherThing);
		//	//DirectX::XMVECTOR subtract = DirectX::XMVectorSubtract(enemyPos, playerPos);
		//	//DirectX::XMVECTOR length = DirectX::XMVector3Length(subtract);
		//	//DirectX::XMVector3Normalize(length);

		//	//float distance = 0.0f;
		//	//DirectX::XMStoreFloat(&distance, length);
		//	//OutputDebugStringA(std::to_string(distance).c_str());
		//	
		//	//pPlayer->pRigidBody->SetVelocity({new_velX, new_velY, new_velZ});

		//	// https://stackoverflow.com/questions/10291862/what-is-the-best-way-to-get-distance-between-2-points-with-directxmath
		//	// https://gamedev.stackexchange.com/questions/5906/collision-resolution
		//	// https://www.plasmaphysics.org.uk/print/collision2d.htm
		//	// https://relativity.net.au/gaming/java/SimpleCollisionDetection.html
		//	// https://stackoverflow.com/questions/67237843/3d-rigid-body-sphere-collision-response-c
		//	// https://stackoverflow.com/questions/3232318/sphere-sphere-collision-detection-reaction
		//	// https://happycoding.io/tutorials/processing/collision-detection


		//	float vel_X = pPlayer->pRigidBody->GetVelocity().x;
		//	float vel_Y = pPlayer->pRigidBody->GetVelocity().y;
		//	float vel_Z = pPlayer->pRigidBody->GetVelocity().z;

		//	float xPos = pPlayer->pRigidBody->GetPosition().x;
		//	float yPos = pPlayer->pRigidBody->GetPosition().y;
		//	float zPos = pPlayer->pRigidBody->GetPosition().z;

		//	vel_X = -vel_X * 0.5f;
		//	vel_Y = -vel_Y * 0.5f;
		//	vel_Z = -vel_Z * 0.5f;

		//	pPlayer->pRigidBody->SetVelocity({ vel_X, vel_Y, vel_Z });

		//}
	}

	//---------------------------------------------
	// Map
	lvl1Map.Update(sceneData.dt);

	//---------------------------------------------
	// Game Objects
	object->Update(sceneData.dt);
	skybox->Update(sceneData.dt);
	sphere->Update(sceneData.dt);
	
	for (auto& sphere : spheres)
	{
		sphere->Update(sceneData.dt);
	}

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
	sphere->Draw(sceneData.gfx);

	for (auto& sphere : spheres)
	{
		sphere->Draw(sceneData.gfx);
	}

	//---------------------------------------------
	// Camera manager
	cameraManager.Draw(sceneData.gfx);
}
