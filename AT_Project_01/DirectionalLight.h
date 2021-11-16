#pragma once
#include "GameObject.h"
#include "Light.h"

class DirectionalLight : public GameObject
{
public:
	DirectionalLight();
	~DirectionalLight() override = default;

	// Set which direction the light will render
	void SetLightDirection(DirectX::XMFLOAT3 direction);
	
	// Set the intensity of the directional light
	void SetLightIntensity(float intensity);

	// Sets the color of the directional light
	void SetLightColor(DirectX::XMFLOAT3 color);

	// Used for Blinn Phong shaders. PBR shaders are not affected by this.
	void SetAmbientColor(DirectX::XMFLOAT3 color);

	void Update(float dt) final;
	void Draw(Graphics* pGfx) final;

	Light GetLightProperty() const;

private:
	Light directionalLight;
};
