#include "ScenePBRShowcase.h"

ScenePBRShowcase::ScenePBRShowcase(SceneManager& sceneManager) : currentSceneManager(sceneManager)
{
}

void ScenePBRShowcase::onCreate(SceneData& sceneData)
{
	//---------------------------------------------
	// Lighting
	directionalLight.SetLightDirection({ 0.0f, 0.0f, 0.0f });
	directionalLight.SetLightIntensity(1.0f);
	directionalLight.SetLightColor({ 1.0f, 1.0f, 1.0f });
	directionalLight.SetAmbientColor({ 0.36f, 0.49f, 0.76f });

	// Pass scene directional light to graphics so that all objects can use it for rendering
	sceneData.gfx->directionalLight = directionalLight;

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


	float width = 5.0f;
	float distance = 2.0f;
	float positionX = 0;
	float positionY = 0;

	Material_PBR aluminum;
	aluminum.ambientOcculsion = 1.0f;
	aluminum.baseColor = { 0.913f, 0.921f, 0.925f, 1.0f };
	aluminum.metallic = 0.0f;
	aluminum.roughness = 0.0f;

	Material_PBR blue_rubber;
	blue_rubber.ambientOcculsion = 1.0f;
	blue_rubber.baseColor = { 0.1f, 0.1f, 1.0f, 1.0f };
	blue_rubber.metallic = 0.0f;
	blue_rubber.roughness = 0.0f;

	float metallic = 0.0f;
	float roughness = 0.0f;

	for (int i = 0; i < 5; i++)
	{
		std::unique_ptr<DefaultObject> sphere = std::make_unique<DefaultObject>();

		//blue_rubber.metallic = metallic;
		blue_rubber.roughness = roughness;

		sphere->pModel->SetPBRMaterial(blue_rubber);
		sphere->pModel->LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\sphere.obj");
		sphere->pModel->LoadShaders(sceneData.gfx, L"VS_Default.cso", L"PS_PBRMaterial.cso", sceneData.gfx->inputElemDesc, sceneData.gfx->GetSizeOfInputElemDesc());
		sphere->pTransform->SetPosition(positionX, positionY, 0.0f);
		spheres.push_back(std::move(sphere));

		positionX += distance;
		if (positionX >= (distance * width))
		{
			positionX = 0.0f;
			positionY += distance;
		}
		sphere.release();
		//metallic += 0.25;
		roughness += 0.25;
	}

	skybox = std::make_unique<SkyBox>(sceneData.gfx, "Assets\\Texture\\syferfontein_0d_clear_1k.png");

	//---------------------------------------------
	// Cameras & Camera manager
	staticCamera = std::make_shared<Camera>(sceneData.gfx->GetWindowSize().first, sceneData.gfx->GetWindowSize().second, 45.0f, 0.01f, 10000.0f, false);
	staticCamera->SetPosition({ 0.0f, 10.0f, -10.0f });
	staticCamera->SetRotation({ 0.0f, 0.0f, 0.0f });

	cameraManager.AddCamera(staticCamera, CamID::static_cam);
	cameraManager.ChangeCamera(CamID::static_cam);

	sceneData.gfx->SetViewMatrix(cameraManager.GetCurrentCameraViewMatrix());
	sceneData.gfx->SetProjectionMatrix(cameraManager.GetCurrentCameraProjectionMatrix());
}

void ScenePBRShowcase::OnDestroy()
{
}

void ScenePBRShowcase::OnActivate()
{
	isSceneActive = true;
}

void ScenePBRShowcase::OnDeactivate()
{
}

void ScenePBRShowcase::Input(SceneData& sceneData)
{
	float speed = 0.5f;
	// When key is pressed update the transform based on position of camera
	if (sceneData.keyboard->IsKeyPressed('W'))
	{
		staticCamera->UpdatePosition(staticCamera->GetDirection().v_forward * speed);
	}
	if (sceneData.keyboard->IsKeyPressed('S'))
	{
		staticCamera->UpdatePosition(staticCamera->GetDirection().v_backward * speed);
	}
	if (sceneData.keyboard->IsKeyPressed('A'))
	{
		staticCamera->UpdatePosition(staticCamera->GetDirection().v_left * speed);
	}
	if (sceneData.keyboard->IsKeyPressed('D'))
	{
		staticCamera->UpdatePosition(staticCamera->GetDirection().v_right * speed);
	}
	if (sceneData.keyboard->IsKeyPressed(VK_SPACE))
	{
		staticCamera->UpdatePosition({ 0.0f, 1.0f * speed, 0.0f, 0.0f });
	}
	if (sceneData.keyboard->IsKeyPressed('Z'))
	{
		staticCamera->UpdatePosition({ 0.0f, -1.0f * speed, 0.0f, 0.0f });
	}

	// Camera rotation
	if (sceneData.keyboard->IsKeyPressed(37)) // Left arrow
	{
		staticCamera->UpdateRotation({ 0.0f, -0.05f, 0.0f, 0.0f });
	}
	if (sceneData.keyboard->IsKeyPressed(39)) // Right arrow
	{
		staticCamera->UpdateRotation({ 0.0f, 0.05f, 0.0f, 0.0f });
	}
	if (sceneData.keyboard->IsKeyPressed(38)) // Up arrow
	{
		staticCamera->UpdateRotation({ -0.05f, 0.0f, 0.0f, 0.0f });
	}
	if (sceneData.keyboard->IsKeyPressed(40)) // Down arrow
	{
		staticCamera->UpdateRotation({ 0.05f, 0.0f, 0.0f, 0.0f });
	}
}

void ScenePBRShowcase::Update(SceneData& sceneData)
{
	for (auto& sphere : spheres)
	{
		sphere->Update(sceneData.dt);
	}
	
	skybox->Update(sceneData.dt);
	directionalLight.Update(sceneData.dt);

	cameraManager.Update(sceneData.dt);
}

void ScenePBRShowcase::Draw(SceneData& sceneData)
{
	sceneData.gfx->ClearBuffer(0.1f, 0.1f, 0.1f);

	for (auto& sphere : spheres)
	{
		sphere->Draw(sceneData.gfx);
	}

	skybox->Draw(sceneData.gfx);

	cameraManager.Draw(sceneData.gfx);
}
