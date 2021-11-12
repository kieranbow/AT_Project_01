#include "CameraManager.h"
#include "Graphics.h"

void CameraManager::AddCamera(const std::shared_ptr<Camera>& camera, camera_ID ID)
{
	auto insert = cameras.insert(std::make_pair(ID, camera));
}

void CameraManager::ChangeCamera(camera_ID cameraID)
{
	auto iter = cameras.find(cameraID);

	if (iter != cameras.end())
	{
		iter->second->EnableCamera(true);

		currentCamera = iter->second;
	}

}

DirectX::XMMATRIX CameraManager::GetCurrentCameraViewMatrix() const
{
	if (currentCamera->IsActive())
	{
		return currentCamera->GetViewMatrix();
	}
	OutputDebugStringA("No camera active");
	return DirectX::XMMATRIX();
}

DirectX::XMMATRIX CameraManager::GetCurrentCameraProjectionMatrix() const
{
	if (currentCamera->IsActive())
	{
		return currentCamera->GetProjectionMatrix();
	}
	OutputDebugStringA("No camera active");
	return DirectX::XMMATRIX();
}

std::shared_ptr<Camera> CameraManager::GetCurrentCamera() const
{
	if (currentCamera->IsActive())
	{
		return currentCamera;
	}
	return std::shared_ptr<Camera>();
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
