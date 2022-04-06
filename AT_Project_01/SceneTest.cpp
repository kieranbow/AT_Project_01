#include "SceneTest.h"
#include "Model_loader.h"

SceneTest::SceneTest(SceneManager& sceneManager) : currentSceneManager(sceneManager)
{
}

void SceneTest::onCreate(SceneData& sceneData)
{
	//---------------------------------------------
	// Cameras
	staticCamera = std::make_shared<Camera>(sceneData.gfx->GetWindowSize().first, sceneData.gfx->GetWindowSize().second, 90.0f, 0.01f, 10000.0f, false);
	pPlayer = std::make_unique<Player>(sceneData.gfx);

	staticCamera->SetPosition({ 0.0f, 0.0f, 30.0f });
	staticCamera->SetRotation({ 0.0f, 110.0f, 0.0f });

	cameraManager.AddCamera(pPlayer->camera, CamID::player_cam);
	cameraManager.AddCamera(staticCamera, CamID::static_cam);
	cameraManager.ChangeCamera(CamID::player_cam);

	sceneData.gfx->SetViewMatrix(cameraManager.GetCurrentCameraViewMatrix());
	sceneData.gfx->SetProjectionMatrix(cameraManager.GetCurrentCameraProjectionMatrix());

	std::string filePath[6];
	filePath[0].append("Assets\\Texture\\cubemap\\px.png"); // East		+X
	filePath[1].append("Assets\\Texture\\cubemap\\nx.png"); // West		-X
	filePath[2].append("Assets\\Texture\\cubemap\\py.png"); // Up		+Y
	filePath[3].append("Assets\\Texture\\cubemap\\ny.png"); // Down		-Y
	filePath[4].append("Assets\\Texture\\cubemap\\pz.png"); // North	+Z
	filePath[5].append("Assets\\Texture\\cubemap\\nz.png"); // South	-Z

	Texture cubeMap(sceneData.gfx);
	cubeMap.LoadAndCreateCubeMap(filePath, DXGI_FORMAT_R8G8B8A8_UNORM);
	cubeMap.SetShaderResource(Bind::Texture::t2, 1u);






	Texture brdfMap(sceneData.gfx);
	brdfMap.LoadAndCreateTexture("Assets\\Texture\\integrateBrdf.png", DXGI_FORMAT_R8G8B8A8_UNORM);
	brdfMap.SetShaderResource(Bind::Texture::t4, 1u);

	//Material Red_plastic;
	//Red_plastic.ambient = { 0.0f, 0.0f, 0.0f, 1.0f };
	//Red_plastic.Diffuse = { 0.5f, 0.0f, 0.0f, 1.0f };
	//Red_plastic.Emissive = { 0.0f, 0.0f, 0.0f, 1.0f };
	//Red_plastic.Specular = { 0.7f, 0.6f, 0.6f, 1.0f };
	//Red_plastic.SpecularPower = 32.0f;

	//Material Emerald;
	//Emerald.ambient = { 0.0215f, 0.1745f, 0.0215f, 1.0f };
	//Emerald.Diffuse = { 0.07568f, 0.61424f, 0.07568f, 1.0f };
	//Emerald.Emissive = { 0.0f, 0.0f, 0.0f, 1.0f };
	//Emerald.Specular = { 0.633f, 0.727811f, 0.633f, 1.0f };
	//Emerald.SpecularPower = 76.8f;

	//Material Gold;
	//Gold.ambient = { 0.24725f, 0.1995f, 0.0745f, 1.0f };
	//Gold.Diffuse = { 0.75164f, 0.60648f, 0.22648f, 1.0f };
	//Gold.Emissive = { 0.0f, 0.0f, 0.0f, 1.0f };
	//Gold.Specular = { 0.628281f, 0.555802f, 0.366065f, 1.0f };
	//Gold.SpecularPower = 51.2f;

	//spaceMarineHelmet.LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\Helmet_paintable_v2.obj");
	//spaceMarineHelmet.LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_PBR.cso", sceneData.gfx->inputElemDesc, sceneData.gfx->GetSizeOfInputElemDesc());
	//spaceMarineHelmet.LoadTextures(sceneData.gfx, "Assets\\Texture\\Helmet_V3_Albedo.png");
	//spaceMarineHelmet.LoadTextures(sceneData.gfx, "Assets\\Texture\\Helmet_V3_Normal.png");
	//spaceMarineHelmet.transform.SetPosition(0.0f, 0.0f, 0.0f);
	//spaceMarineHelmet.transform.SetRotation(0.0f, 3.0f, 0.0f);

	//sphere.LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\sphere.obj");
	//sphere.LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_BlinnPhong.cso", sceneData.gfx->inputElemDesc, sceneData.gfx->GetSizeOfInputElemDesc());
	//sphere.LoadTextures(sceneData.gfx, "Assets\\Texture\\icon.png");
	//sphere.transform.SetPosition(0.0f, 25.0f, 0.0f);

	//sky.LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\inner_sphere.obj");
	//sky.LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_unlit.cso", sceneData.gfx->inputElemDesc, sceneData.gfx->GetSizeOfInputElemDesc());
	//sky.LoadTextures(sceneData.gfx, "Assets\\Texture\\syferfontein_0d_clear_1k.png");
	//sky.transform.SetScale(5000.0f, 5000.f, 5000.0f);

	//pyramid.LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\pyramid.obj");
	//pyramid.LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_BlinnPhong.cso", ied, ied_size);
	//pyramid.LoadTextures(sceneData.gfx, "Assets\\Texture\\default.png");

	//single_cube.LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\cube.obj");
	//single_cube.LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_BlinnPhong.cso", ied, ied_size);
	//single_cube.LoadTextures(sceneData.gfx, "Assets\\Texture\\grass.png");

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
	skyIR.SetShaderResource(Bind::Texture::t0, 1u);

	std::string sky_filepath[6];
	sky_filepath[0].append("Assets\\Texture\\cubemap\\px.png"); // East		+X
	sky_filepath[1].append("Assets\\Texture\\cubemap\\nx.png"); // West		-X
	sky_filepath[2].append("Assets\\Texture\\cubemap\\py.png"); // Up		+Y
	sky_filepath[3].append("Assets\\Texture\\cubemap\\ny.png"); // Down		-Y
	sky_filepath[4].append("Assets\\Texture\\cubemap\\pz.png"); // North	+Z
	sky_filepath[5].append("Assets\\Texture\\cubemap\\nz.png"); // South	-Z

	Texture skyHDRI(sceneData.gfx);
	skyHDRI.LoadAndCreateCubeMap(sky_filepath, DXGI_FORMAT_R8G8B8A8_UNORM);
	skyHDRI.SetShaderResource(Bind::Texture::t1, 1u);

	Texture bdrfLut(sceneData.gfx);
	bdrfLut.LoadAndCreateTexture("Assets\\Texture\\integrateBrdf.png", DXGI_FORMAT_R8G8B8A8_UNORM);
	bdrfLut.SetShaderResource(Bind::Texture::t2, 1u);


	// https://docs.microsoft.com/en-us/windows/uwp/gaming/complete-code-for-ddstextureloader
	// https://www.3dgep.com/texturing-lighting-directx-11/#Light_Properties-2

	float width = 10.0f;
	float height = 10.0f;

	float distance = 100.0f;

	int amount = 1000;

	float positionX = 0;
	float positionY = 0;
	float positionZ = 0;

	//ModelLoader loader("Assets\\Model\\Helmet_paintable_v2.obj");

	Material_PBR material;
	material.ambientOcculsion = 1.0f;
	material.baseColor = { 1.0f, 0.0f, 0.0f, 1.0f };
	material.metallic = 0.0f;
	material.roughness = 1.0f;

	for (int i = 0; i < amount; i++)
	{
		std::unique_ptr temp = std::make_unique<DefaultObject>();
		temp->pModel->SetPBRMaterial(material);
		//temp->model->LoadMesh(sceneData.gfx, loader.GetVertices(), loader.GetIndices());
		temp->pModel->LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\Helmet_paintable_v2.obj");
		temp->pModel->LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_PBRMaterial.cso", sceneData.gfx->inputElemDesc, sceneData.gfx->GetSizeOfInputElemDesc());
		temp->pTransform->SetPosition(positionX - (distance * 10) / 2, positionY - (distance * 10) / 2, positionZ - (distance * 10) / 2);
		temp->pTransform->SetScale(2.0f, 2.0f, 2.0f);
		cube.push_back(std::move(temp));

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
		temp.release();
	}
}

void SceneTest::OnDestroy()
{
}

void SceneTest::OnActivate()
{
	isSceneActive = true;
}

void SceneTest::OnDeactivate()
{
	
}

void SceneTest::Input(SceneData& sceneData)
{
	if (sceneData.keyboard->IsKeyPressed('C'))
	{
		currentSceneManager.SwitchScene(SceneID::lvl_1);
	}

	pPlayer->Input(sceneData.keyboard, sceneData.mouse);

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

		}
	}

	if (sceneData.keyboard->IsKeyPressed('R'))
	{
		cameraManager.ChangeCamera(CamID::static_cam);
	}

	if (sceneData.keyboard->IsKeyPressed('T'))
	{
		cameraManager.ChangeCamera(CamID::player_cam);
	}

}

void SceneTest::Update(SceneData& sceneData)
{
	rot += 0.05f;

	pPlayer->Update(sceneData.dt);

	//float x = pPlayer->camera->GetPosition().m128_f32[0];
	//float y = pPlayer->camera->GetPosition().m128_f32[1];
	//float z = pPlayer->camera->GetPosition().m128_f32[2];


	sceneData.gfx->currentCamera.SetPosition(cameraManager.GetCurrentCamera()->GetPosition());
	cameraManager.Update(sceneData.dt);



	//spaceMarineHelmet.transform.SetRotation(0.0f, 0.05f * rot, 0.0f);
	//spaceMarineHelmet.Update(sceneData.dt);
	
	//sphere.transform.SetPosition(x, y, z);
	//sphere.Update(sceneData.dt);

	////sky.transform.SetRotation(0.0f, 0.01f * rot, 0.0f);
	//sky.Update(sceneData.dt);

	//pyramid.transform.SetPosition(15.0f, 15.0f, 0.0f);
	//pyramid.transform.SetRotationAxis(0.5f * rot);
	//pyramid.Update(sceneData.dt);

	//single_cube.transform.SetRotation(0.0f, 0.0f, 0.5f * rot);
	//single_cube.transform.SetPosition(-15.0f, -15.0f, 0.0f);
	//single_cube.Update(sceneData.dt);

	for (auto& cubes : cube)
	{
		cubes->pTransform->SetRotation(0.14f * rot , 0.2f * rot , 1.0f * rot);
		cubes->Update(sceneData.dt);
	}
}

void SceneTest::Draw(SceneData& sceneData)
{
	sceneData.gfx->ClearBuffer(0.1f, 0.1f, 0.1f);

	cameraManager.Draw(sceneData.gfx);

	pPlayer->Draw(sceneData.gfx);

	//spaceMarineHelmet.Draw(sceneData.gfx);
	//sphere.Draw(sceneData.gfx);
	//sky.Draw(sceneData.gfx);
	//pyramid.Draw(sceneData.gfx);
	//single_cube.Draw(sceneData.gfx);

	for (auto& cubes : cube)
	{
		cubes->Draw(sceneData.gfx);
	}
}
