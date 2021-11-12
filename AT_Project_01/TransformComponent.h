#pragma once
#include <DirectXMath.h>

// The Transformation class contrains world matrix and vector allowing any object
// that has this component to Scale, Rotate and Translate.
class TransformComponent
{
	public:
		TransformComponent();
		TransformComponent(DirectX::XMFLOAT3 rotate, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 position);
		~TransformComponent() = default;

		TransformComponent(const TransformComponent&) = delete;
		TransformComponent& operator=(const TransformComponent&) = delete;

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
		DirectX::XMFLOAT3 f_scale		= { 1.0f, 1.0f, 1.0f };
		DirectX::XMFLOAT3 f_rotation	= { 1.0f, 1.0f, 1.0f };
		DirectX::XMFLOAT3 f_position	= { 0.0f, 0.0f, 0.0f };

		DirectX::XMVECTOR rotaxis = { 0.0f, 0.0f, 0.0f, 0.0f };
		
		DirectX::XMMATRIX m_world = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX m_rotation;
		DirectX::XMMATRIX m_scale;
		DirectX::XMMATRIX m_translation;

		// Default movement Vectors
		const DirectX::XMVECTOR v_defForward = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		const DirectX::XMVECTOR v_defUp = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		const DirectX::XMVECTOR v_defBackward = DirectX::XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
		const DirectX::XMVECTOR v_defLeft = DirectX::XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
		const DirectX::XMVECTOR v_defRight = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
};
