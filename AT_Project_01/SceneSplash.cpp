#include "SceneSplash.h"

SceneSplash::SceneSplash(SceneManager& sceneManager) : currentSceneManager(sceneManager)
{
}

void SceneSplash::onCreate(SceneData& sceneData)
{
	//---------------------------------------------
	// Cameras & Camera manager
	staticCamera = std::make_shared<Camera>(sceneData.gfx->GetWindowSize().first, sceneData.gfx->GetWindowSize().second, 45.0f, 0.01f, 10000.0f, false);
	staticCamera->SetPosition({ 0.0f, 0.0f, 0.0f });
	staticCamera->SetRotation({ 0.0f, 0.0f, 0.0f });

	cameraManager.AddCamera(staticCamera, CamID::static_cam);
	cameraManager.ChangeCamera(CamID::static_cam);

	sceneData.gfx->SetViewMatrix(cameraManager.GetCurrentCameraViewMatrix());
	sceneData.gfx->SetProjectionMatrix(cameraManager.GetCurrentCameraProjectionMatrix());

	splashscreen = std::make_unique<DefaultObject>();
	splashscreen->model->LoadMeshFromSource(sceneData.gfx, "Assets\\Model\\splash.obj");
	splashscreen->model->LoadShaders(sceneData.gfx, L"..\\x64\\Debug\\VS_Default.cso", L"..\\x64\\Debug\\PS_unlit.cso", sceneData.gfx->inputElemDesc, sceneData.gfx->GetSizeOfInputElemDesc());
	splashscreen->model->LoadTextures(sceneData.gfx, "Assets\\Texture\\splash.png", DXGI_FORMAT_R8G8B8A8_UNORM);
	splashscreen->model->SetPosition({ 0.0f, 0.0f, 5.0f });
	splashscreen->model->SetScale({ 4.2f, 2.2f, 1.0f });

}

void SceneSplash::OnDestroy()
{
}

void SceneSplash::OnActivate()
{
	isSceneActive = true;
}

void SceneSplash::OnDeactivate()
{
}

void SceneSplash::Input(SceneData& sceneData)
{
	if (sceneData.keyboard->IsKeyPressed(VK_SPACE))
	{
		currentSceneManager.SwitchScene(SceneID::lvl_1);
	}
}

void SceneSplash::Update(SceneData& sceneData)
{
	splashscreen->Update(sceneData.dt);

	//---------------------------------------------
	// Camera manager
	sceneData.gfx->currentCamera.SetPosition(cameraManager.GetCurrentCamera()->GetPosition());
	cameraManager.Update(sceneData.dt);
}

void SceneSplash::Draw(SceneData& sceneData)
{
	sceneData.gfx->ClearBuffer(0.1f, 0.1f, 0.1f);

	splashscreen->Draw(sceneData.gfx);

	//---------------------------------------------
	// Camera manager
	cameraManager.Draw(sceneData.gfx);
}
