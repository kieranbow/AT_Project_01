#include "Camera.h"

Camera::Camera()
{
	m_view = XMMatrixLookAtLH(v_eye, v_target, v_up);

	if (enableOrthographic)
	{
		m_projection = XMMatrixOrthographicLH(width, height, nearZ, farZ);
	}
	else
	{
		aspectRatio = width / height;
		fovRad = (fovDeg / 360.0f) * XM_2PI;
		m_projection = XMMatrixPerspectiveFovLH(fovRad, aspectRatio, nearZ, farZ);
	}
}

void Camera::SetPosition(XMVECTOR position)
{
	v_eye = position;
}

void Camera::SetRotation(XMVECTOR rotation)
{
	v_rotation = rotation;
}

void Camera::UpdatePosition(XMVECTOR position)
{
	v_eye += position;
}

void Camera::UpdateRotation(XMVECTOR rotation)
{
	v_rotation += rotation;
}

void Camera::SetTarget(XMVECTOR target)
{
	v_target += target;
}

void Camera::SetLookAt(XMFLOAT3 position)
{
	if (position.x == v_eye.m128_f32[0] 
		&& position.y == v_eye.m128_f32[1] 
		&& position.z == v_eye.m128_f32[2])
	{
		return;
	}

	position.x = v_eye.m128_f32[0] - position.x;
	position.y = v_eye.m128_f32[1] - position.y;
	position.z = v_eye.m128_f32[2] - position.z;

	float pitch = 0.0f;
	if (position.y != 0.0f)
	{
		const float distance = sqrt(position.x * position.x + position.z * position.z);
		pitch = atan(position.y / distance);
	}

	float yaw = 0.0f;
	if (position.x != 0.0f)
	{
		yaw = atan(position.x / position.z);
	}

	if (position.z > 0)
	{
		yaw += DirectX::XM_PI;
	}

	SetRotation({ pitch, yaw, 0.0f, 0.0f });
}

void Camera::Update(double dt)
{
	XMMATRIX m_camRotation = XMMatrixRotationRollPitchYaw(v_rotation.m128_f32[0], v_rotation.m128_f32[1], v_rotation.m128_f32[2]);
	XMVECTOR v_camTarget = XMVector3TransformCoord(v_defForward, m_camRotation);
	v_camTarget += v_eye;
	XMVECTOR v_upDirection = XMVector3TransformCoord(v_defUp, m_camRotation);
	m_view = XMMatrixLookAtLH(v_eye, v_camTarget, v_upDirection);

	XMMATRIX m_rotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, v_rotation.m128_f32[1], 0.0f);
	direction.v_forward = XMVector3TransformCoord(v_defForward, m_rotationMatrix);
	direction.v_backward = XMVector3TransformCoord(v_defBackward, m_rotationMatrix);
	direction.v_left = XMVector3TransformCoord(v_defLeft, m_rotationMatrix);
	direction.v_right = XMVector3TransformCoord(v_defRight, m_rotationMatrix);
}

DirectX::XMMATRIX Camera::GetViewMatrix() const
{
	return m_view;
}

DirectX::XMMATRIX Camera::GetProjectionMatrix() const
{
	return m_projection;
}

Direction Camera::GetDirection() const
{
	return direction;
}
