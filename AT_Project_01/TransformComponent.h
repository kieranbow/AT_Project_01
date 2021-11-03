#pragma once
#include <DirectXMath.h>

class TransformComponent
{
	public:
		TransformComponent();
		TransformComponent(DirectX::XMFLOAT3 rotate, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 position);
		~TransformComponent() = default;


		void Update();
		void SetRotation(float pitch, float yaw, float roll);
		void SetRotationAxis(float angle);
		void SetScale(float x, float y, float z);
		void SetPosition(float x, float y, float z);
		
		DirectX::XMFLOAT3 GetRotation() const;
		DirectX::XMFLOAT3 GetScale() const;
		DirectX::XMFLOAT3 GetPosition() const;

		DirectX::XMMATRIX GetWorldMatrix() const;

	private:
		DirectX::XMFLOAT3 f_scale = { 1.0f, 1.0f, 1.0f };
		DirectX::XMFLOAT3 f_rotation = { 1.0f, 1.0f, 1.0f };
		DirectX::XMFLOAT3 f_position = { 1.0f, 1.0f, 1.0f };

		DirectX::XMVECTOR rotaxis = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		
		DirectX::XMMATRIX m_world = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX m_rotation;
		DirectX::XMMATRIX m_scale;
		DirectX::XMMATRIX m_translation;
};

