#include "DirectionalLight.h"

DirectionalLight::DirectionalLight()
{
	directionalLight.direction = { 0.0f, 0.0f, 1.0f };
	directionalLight.color = { 1.0f, 1.0f, 1.0f, 0.0f };
	directionalLight.ambientColor = { 0.2f, 0.2f, 0.2f, 0.0f };
}

void DirectionalLight::SetLightDirection(DirectX::XMFLOAT3 direction)
{
	directionalLight.direction = direction;
}

void DirectionalLight::SetLightIntensity(float intensity)
{
	directionalLight.intensity = intensity;
}

void DirectionalLight::SetLightColor(DirectX::XMFLOAT3 color)
{
	directionalLight.color = { color.x, color.y, color.z, 0.0f };
}

void DirectionalLight::SetAmbientColor(DirectX::XMFLOAT3 color)
{
	directionalLight.ambientColor = { color.x, color.y, color.z, 0.0f };
}

void DirectionalLight::Update(float dt)
{
	
}

void DirectionalLight::Draw(Graphics* pGfx)
{

}

Light DirectionalLight::GetLightProperty() const
{
	return directionalLight;
}
