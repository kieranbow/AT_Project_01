#include "scene_lvl1.h"
#include <fstream>
#include <map>

Scenelvl1::Scenelvl1(SceneManager& sceneManager) : currentSceneManager(sceneManager)
{
}



enum class objID
{
	Air,
	Floor,
	Wall_1_high,
	Wall_2_high
};

objID findID(std::string id)
{
	std::map<std::string, objID> test =
	{
		{"000", objID::Air},
		{"001", objID::Floor},
		{"002", objID::Wall_1_high},
		{"003", objID::Wall_2_high},
	};

	auto iter = test.find(id);

	if (iter != test.end())
	{
		return iter->second;
	}
	return objID::Air;

};

void Scenelvl1::onCreate(SceneData& sceneData)
{
	//---------------------------------------------
	// Lighting
	directionalLight.SetLightDirection({ 0.0f, 0.5f, 1.0f });
	directionalLight.SetLightIntensity(1.0f);
	directionalLight.SetLightColor({ 1.0f, 0.75f, 0.25f });

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
		enemy->pTransform->SetPosition(positionX - (distance * 10.0f) / 2.0f, 0.0f, 10.0f);
		pEnemy.push_back(std::move(enemy));

		positionX += distance;
		if (positionX >= (distance * width))
		{
			positionX = 0.0f;
		}
		enemy.release();
	}

	//---------------------------------------------
	// Cameras
	staticCamera = std::make_shared<Camera>(sceneData.gfx->GetWindowSize().first, sceneData.gfx->GetWindowSize().second, 45.0f, 0.01f, 10000.0f, false);
	staticCamera->SetPosition({ -10.0f, 20.0f, 10.0f });
	//staticCamera->SetRotation({ 0.0f, 110.0f, 0.0f });
	staticCamera->SetRotation({ 1.5f, 0.0f, 0.0f });

	//---------------------------------------------
	// Camera manager
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
	skyIR.LoadAndCreateCubeMap(skyIR_filepath);
	skyIR.SetShaderResource(Bind::Texture::t3, 1u);

	std::string sky_filepath[6];
	sky_filepath[0].append("Assets\\Texture\\cubemap\\px.png"); // East		+X
	sky_filepath[1].append("Assets\\Texture\\cubemap\\nx.png"); // West		-X
	sky_filepath[2].append("Assets\\Texture\\cubemap\\py.png"); // Up		+Y
	sky_filepath[3].append("Assets\\Texture\\cubemap\\ny.png"); // Down		-Y
	sky_filepath[4].append("Assets\\Texture\\cubemap\\pz.png"); // North	+Z
	sky_filepath[5].append("Assets\\Texture\\cubemap\\nz.png"); // South	-Z

	Texture sky(sceneData.gfx);
	sky.LoadAndCreateCubeMap(sky_filepath);
	sky.SetShaderResource(Bind::Texture::t4, 1u);

	Texture bdrfLut(sceneData.gfx);
	bdrfLut.LoadAndCreateTexture("Assets\\Texture\\integrateBrdf.png");
	bdrfLut.SetShaderResource(Bind::Texture::t5, 1u);

	//---------------------------------------------
	// Objects
	object = std::make_unique<DefaultObject>();
	object->model->LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\Helmet_paintable_v2.obj");
	object->model->LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_PBR.cso", sceneData.gfx->inputElemDesc, sceneData.gfx->GetSizeOfInputElemDesc());
	object->model->LoadTextures(sceneData.gfx, "Assets\\Texture\\Helmet_V3_Albedo.png");
	object->model->LoadTextures(sceneData.gfx, "Assets\\Texture\\Helmet_V3_RMAO.png");
	object->model->SetPosition({ 0.0f, 0.0f, 0.0f });
	object->model->SetRotation({ 0.0f, 3.0f, 0.0f });


	//skyBox.LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\inner_sphere.obj");
	//skyBox.LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_unlit.cso", sceneData.gfx->inputElemDesc, sceneData.gfx->GetSizeOfInputElemDesc());
	//skyBox.LoadTextures(sceneData.gfx, "Assets\\Texture\\syferfontein_0d_clear_1k.png");
	//skyBox.transform.SetScale(5000.0f, 5000.f, 5000.0f);

	//texelCube.LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\cube_proj.obj");
	//texelCube.LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_BlinnPhong.cso", sceneData.gfx->inputElemDesc, sceneData.gfx->GetSizeOfInputElemDesc());
	//texelCube.LoadTextures(sceneData.gfx, "Assets\\Texture\\default.png");
	//texelCube.transform.SetPosition(0.0f, 0.0f, -4.0f);

	//std::fstream map;

	//std::map<std::string, std::string> obj_id;
	//obj_id.emplace("004", "004");

	//map.open("Assets\\Levels\\lvl1_layout.txt");

	//if (map.is_open())
	//{
	//	std::string word;

	//	float x = 0.0f;
	//	float z = 0.0f;
	//	float map_width = 32.0f; // width * 2

	//	while (map >> word)
	//	{
	//		if (x == map_width)
	//		{
	//			x = 0.0f;
	//			z += 2.0f;
	//		}

	//		switch (findID(word))
	//		{
	//			case objID::Air:
	//			{
	//				break;
	//			}
	//			case objID::Floor:
	//			{
	//				// Create object
	//				std::unique_ptr<Model>floor = std::make_unique<Model>();
	//				floor->LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\Plane.obj");
	//				floor->LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_Floor.cso", sceneData.gfx->inputElemDesc, sceneData.gfx->GetSizeOfInputElemDesc());
	//				floor->transform.SetPosition(x, -1.0f, z);

	//				// Push object into object pool
	//				objects.push_back(std::move(floor));

	//				// Delete the object memory
	//				floor.release();
	//				break;
	//			}
	//			case objID::Wall_1_high:
	//			{
	//				// Create object
	//				std::unique_ptr<Model> wall = std::make_unique<Model>();
	//				wall->LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\cube_proj.obj");
	//				wall->LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_BlinnPhong.cso", sceneData.gfx->inputElemDesc, sceneData.gfx->GetSizeOfInputElemDesc());
	//				wall->transform.SetPosition(x, 0.0f, z);

	//				// Push object into object pool
	//				objects.push_back(std::move(wall));

	//				// Delete the object memory
	//				wall.release();
	//				break;
	//			}
	//			case objID::Wall_2_high:
	//			{
	//				// Create object
	//				std::unique_ptr<Model> wall = std::make_unique<Model>();
	//				wall->LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\cube_proj.obj");
	//				wall->LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_BlinnPhong.cso", sceneData.gfx->inputElemDesc, sceneData.gfx->GetSizeOfInputElemDesc());
	//				wall->transform.SetPosition(x, 0.0f, z);

	//				std::unique_ptr<Model> wall_2 = std::make_unique<Model>();
	//				wall_2->LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\cube_proj.obj");
	//				wall_2->LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_BlinnPhong.cso", sceneData.gfx->inputElemDesc, sceneData.gfx->GetSizeOfInputElemDesc());
	//				wall_2->transform.SetPosition(x, 2.0f, z);

	//				// Push object into object pool
	//				objects.push_back(std::move(wall));
	//				objects.push_back(std::move(wall_2));

	//				// Delete the object memory
	//				wall.release();
	//				wall_2.release();
	//				break;
	//			}

	//			default:
	//				break;
	//		}

	//		x += 2.0f;
	//	}
	//	map.close();
	//}
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

	if (sceneData.mouse->IsLeftBtnDown())
	{
		cameraManager.ChangeCamera(CamID::player_cam);
	}
	if (sceneData.mouse->IsRightBtnDown())
	{
		cameraManager.ChangeCamera(CamID::static_cam);
	}

}

void Scenelvl1::Update(SceneData& sceneData)
{
	time += 0.05f;

	//---------------------------------------------
	// Entity
	pPlayer->Update(sceneData.dt);

	directionalLight.SetLightDirection({ 0.0f, 1.0f * time, 0.0f });

	for (auto& enemy : pEnemy)
	{
		//enemy->LookAt(pPlayer->model.transform.GetPosition());
		// enemy->MoveTo(pPlayer->model.transform.GetPosition(), sceneData.dt);
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

		if (enemy->collision->AABBIntersect(pPlayer->pCollision.get()))
		{
			float new_velX = pPlayer->pRigidBody->GetVelocity().x * -1.0f;
			float new_velY = pPlayer->pRigidBody->GetVelocity().y * -1.0f;
			float new_velZ = pPlayer->pRigidBody->GetVelocity().z * -1.0f;


			//pPlayer->pRigidBody->SetVelocity({ new_velX, new_velY, new_velZ });

			//pPlayer->pRigidBody->SetVelocity({new_velX, new_velY, new_velZ});
		}


	}

	//---------------------------------------------
	// Objects
	object->Update(sceneData.dt);
	//skyBox.Update(sceneData.dt);
	//texelCube.Update(sceneData.dt);

	//for (auto& object : objects)
	//{
	//	object->Update(sceneData.dt);
	//}

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
	object->Draw(sceneData.gfx);
	//skyBox.Draw(sceneData.gfx);
	//texelCube.Draw(sceneData.gfx);

	//for (auto& object : objects)
	//{
	//	object->Draw(sceneData.gfx);
	//}


	//---------------------------------------------
	// Camera manager
	cameraManager.Draw(sceneData.gfx);
}
