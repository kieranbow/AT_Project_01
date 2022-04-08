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


	//---------------------------------------------
	// Map
	XMVECTOR playerPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
	lvl1Map.LoadMap(sceneData.gfx, "Assets\\Levels\\lvl1_layout.txt", 56, playerPosition); // width * 2

	pPlayer->camera->SetPosition(playerPosition);

	for (int i = 0; i < lvl1Map.getEnemyPosition().size(); i++)
	{
		std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(sceneData.gfx);
		enemy->pTransform->SetPosition(lvl1Map.getEnemyPosition()[i].x, 1.0f, lvl1Map.getEnemyPosition()[i].z);
		pEnemy.push_back(std::move(enemy));
		enemy.release();
	}


	//float width = 10.0f;
	//float height = 10.0f;
	//float distance = 10.0f;
	//float positionX = 0;

	//for (int e = 0; e < 10; e++)
	//{
	//	std::unique_ptr<Enemy> enemy = std::make_unique<Enemy>(sceneData.gfx);
	//	enemy->pTransform->SetPosition(positionX - (distance * 10.0f) / 2.0f, 1.0f, 10.0f);
	//	pEnemy.push_back(std::move(enemy));

	//	positionX += distance;
	//	if (positionX >= (distance * width))
	//	{
	//		positionX = 0.0f;
	//	}
	//	enemy.release();
	//}

	//---------------------------------------------
	// Cameras & Camera manager
	staticCamera = std::make_shared<Camera>(sceneData.gfx->GetWindowSize().first, sceneData.gfx->GetWindowSize().second, 45.0f, 0.01f, 10000.0f, false);
	staticCamera->SetPosition({ -10.0f, 20.0f, 10.0f });
	//staticCamera->SetRotation({ 1.5f, 0.0f, 0.0f });

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
	// Game Objects
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

	for(auto& object : lvl1Map.getMapObjects())
	{
		XMVECTOR objNormal;
		float objDepthColl;
		int objFColl;

		if (CollisionHandler::AABBIntersect(object->pCollision->min, object->pCollision->max, pPlayer->pCollision->min, pPlayer->pCollision->max, objNormal, objDepthColl, objFColl))
		{
			XMVECTOR something = pPlayer->camera->GetPosition() + (objNormal * objDepthColl);
			pPlayer->camera->SetPosition(something);
		}

		XMVECTOR bulletNormal;
		float bulletDepthColl;
		int bulletFColl;
		for (auto& bullet : pPlayer->gun->getBulletPool())
		{
			if (CollisionHandler::AABBIntersect(object->pCollision->min, object->pCollision->max, bullet->pCollision->min, bullet->pCollision->max, bulletNormal, bulletDepthColl, bulletFColl))
			{
				switch (bulletFColl)
				{
					case CollisionHandler::Faces::negX:
						bullet->pRigidBody->SetVelocity({ -0.02f, 0.0f, 0.0f });
						break;
					case CollisionHandler::Faces::posX:
						bullet->pRigidBody->SetVelocity({ 0.02f, 0.0f, 0.0f });
						break;
					case CollisionHandler::Faces::negY:
						bullet->pRigidBody->SetVelocity({ 0.0f, -0.02f, 0.0f });
						break;
					case CollisionHandler::Faces::posY:
						bullet->pRigidBody->SetVelocity({ 0.0f, 0.02f, 0.0f });
						break;
					case CollisionHandler::Faces::negZ:
						bullet->pRigidBody->SetVelocity({ 0.0f, 0.0f, -0.02f });
						break;
					case CollisionHandler::Faces::posZ:
						bullet->pRigidBody->SetVelocity({ 0.0f, 0.0f, 0.02f });
						break;

					default:
						break;
				}
			}
		}
	}

	for (auto& enemy : pEnemy)
	{
		XMFLOAT3 position;
		XMStoreFloat3(&position, pPlayer->camera->GetPosition());

		enemy->LookAt(position);
		enemy->Update(sceneData.dt);

		XMVECTOR enemyNormal; 
		float enemyDepthColl; 
		int enemyFColl;

		if (CollisionHandler::AABBIntersect(enemy->collision->min, enemy->collision->max, pPlayer->pCollision->min, pPlayer->pCollision->max, enemyNormal, enemyDepthColl, enemyFColl))
		{
			XMVECTOR direction = { pPlayer->pRigidBody->direction.x, 0.0f, pPlayer->pRigidBody->direction.z, 0.0f };
			XMVECTOR reflect = (direction - 2.0f * XMVector3Dot(direction, enemyNormal) * enemyNormal) * enemyDepthColl;

			XMFLOAT3 velocity;
			XMStoreFloat3(&velocity, reflect + pPlayer->camera->GetPosition());

			pPlayer->pRigidBody->SetPosition(velocity);

			XMVECTOR something = pPlayer->camera->GetPosition() + (enemyNormal * enemyDepthColl);
			pPlayer->camera->SetPosition(something);

			pPlayer->pHealth->subtractHealth(10.0f);
		}

		XMVECTOR bulletNormal;
		float bulletDepthColl;
		int bulletFColl;
		for (auto& bullet : pPlayer->gun->getBulletPool())
		{
			if (CollisionHandler::AABBIntersect(enemy->collision->min, enemy->collision->max, bullet->pCollision->min, bullet->pCollision->max, bulletNormal, bulletDepthColl, bulletFColl))
			{
				enemy->pHealth->subtractHealth(25.0f);
				
				bullet->pRigidBody->SetVelocity({0.0f, 0.0f, 0.0f});
				bullet->pTransform->SetPosition(0.0f, -5.0f, 0.0f);
				bullet->pModel->disableRendering();
			}
		}

		if (enemy->pHealth->getStatus() == 1)
		{
			enemy->pTransform->SetPosition(0.0f, -10.0f, 0.0f);
			enemy->pModel->disableRendering();
		}
	}
	
	//---------------------------------------------
	// Map
	lvl1Map.Update(sceneData.dt);

	//---------------------------------------------
	// Game Objects
	skybox->Update(sceneData.dt);

	//---------------------------------------------
	// Camera manager

	// https://www.jscodetips.com/examples/calculate-the-position-of-an-orbiting-object
	float distance = 30.0f;
	float degree = 10.0f * time;

	float radians = degree * (XM_PI / 180.0f);

	float x = pPlayer->camera->GetPositionFloat().x + distance * std::cosf(radians);
	float z = pPlayer->camera->GetPositionFloat().z + distance * std::sinf(radians);

	staticCamera->SetPosition({ x, 30.0f, z });

	staticCamera->SetLookAt(pPlayer->camera->GetPositionFloat());

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
	skybox->Draw(sceneData.gfx);

	//---------------------------------------------
	// Camera manager
	cameraManager.Draw(sceneData.gfx);
}
