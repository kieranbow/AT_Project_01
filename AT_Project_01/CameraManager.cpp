#include "CameraManager.h"
#include "Graphics.h"

void CameraManager::AddCamera(const std::shared_ptr<Camera>& camera)
{
	auto insert = cameras.insert(std::make_pair(ID, camera));
	ID++;
}

void CameraManager::Update(float dt)
{
	if (currentCamera->IsActive())
	{
		currentCamera->Update(dt);
	}
}

void CameraManager::Draw(Graphics* pGfx)
{
	if (currentCamera->IsActive())
	{
		pGfx->SetViewMatrix(currentCamera->GetViewMatrix());
		pGfx->SetProjectionMatrix(currentCamera->GetProjectionMatrix());
	}
}
