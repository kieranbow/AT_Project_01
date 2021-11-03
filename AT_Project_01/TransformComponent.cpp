#include "TransformComponent.h"

TransformComponent::TransformComponent()
{
	rotaxis = DirectX::XMVectorSet(f_rotation.x, f_rotation.y, f_rotation.z, 0.0f);
	m_rotation = DirectX::XMMatrixRotationAxis(rotaxis, 0.0f);

	m_scale = DirectX::XMMatrixScaling(f_scale.x, f_scale.y, f_scale.z);
	m_translation = DirectX::XMMatrixTranslation(f_position.x, f_position.y, f_position.z);

	m_world = m_scale * m_rotation * m_translation;
}

TransformComponent::TransformComponent(DirectX::XMFLOAT3 rotate, DirectX::XMFLOAT3 scale, DirectX::XMFLOAT3 position)
{
	f_rotation = rotate;
	f_scale = scale;
	f_position = position;

	rotaxis = DirectX::XMVectorSet(f_rotation.x, f_rotation.y, f_rotation.z, 0.0f);
	m_rotation = DirectX::XMMatrixRotationAxis(rotaxis, 0.0f);

	m_scale = DirectX::XMMatrixScaling(f_scale.x, f_scale.y, f_scale.z);
	m_translation = DirectX::XMMatrixTranslation(f_position.x, f_position.y, f_position.z);

	m_world = m_scale * m_rotation * m_translation;
}

void TransformComponent::Update()
{
	// Reset m_world
	//m_world = DirectX::XMMatrixIdentity(); 

	m_world = m_scale * m_rotation * m_translation;
}

void TransformComponent::SetRotation(float pitch, float yaw, float roll)
{
	f_rotation = { pitch, yaw, roll };
	m_rotation = DirectX::XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
}

void TransformComponent::SetRotationAxis(float angle)
{
	rotaxis = DirectX::XMVectorSet(f_rotation.x, f_rotation.y, f_rotation.z, 0.0f);
	m_rotation = DirectX::XMMatrixRotationAxis(rotaxis, angle);
}

void TransformComponent::SetScale(float x, float y, float z)
{
	f_scale = { x, y, z }; // Remove this later
	m_scale = DirectX::XMMatrixScaling(f_scale.x, f_scale.y, f_scale.z);
}

void TransformComponent::SetPosition(float x, float y, float z)
{
	f_position = { x, y, z };
	m_translation = DirectX::XMMatrixTranslation(f_position.x, f_position.y, f_position.z);
}

DirectX::XMFLOAT3 TransformComponent::GetRotation() const
{
	return f_rotation;
}

DirectX::XMFLOAT3 TransformComponent::GetScale() const
{
	return f_scale;
}

DirectX::XMFLOAT3 TransformComponent::GetPosition() const
{
	return f_position;
}

DirectX::XMMATRIX TransformComponent::GetWorldMatrix() const
{
	return m_world;
}
