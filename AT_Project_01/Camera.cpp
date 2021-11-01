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

void Camera::SetLookAt(XMVECTOR position)
{
	if (position.m128_f32[0] == v_eye.m128_f32[0] 
		&& position.m128_f32[1] == v_eye.m128_f32[1] 
		&& position.m128_f32[2] == v_eye.m128_f32[2])
	{
		return;
	}
	position.m128_f32[0] = v_eye.m128_f32[0] - position.m128_f32[0];
	position.m128_f32[1] = v_eye.m128_f32[1] - position.m128_f32[1];
	position.m128_f32[2] = v_eye.m128_f32[2] - position.m128_f32[2];

	float pitch = 0.0f;
	if (position.m128_f32[1] != 0.0f)
	{
		const float distance = sqrt(position.m128_f32[0] * position.m128_f32[0] + position.m128_f32[2] * position.m128_f32[2]);
		pitch = atan(position.m128_f32[1] / distance);
	}

	float yaw = 0.0f;
	if (position.m128_f32[0] != 0.0f)
	{
		yaw = atan(position.m128_f32[0] / position.m128_f32[2]);
	}

	if (position.m128_f32[2] > 0)
	{
		yaw += DirectX::XM_PI;
	}

	SetRotation({ pitch, yaw, 0.0f, 0.0f });
}

void Camera::Update(double dt)
{
	XMMATRIX m_camRotation = XMMatrixRotationRollPitchYaw(v_rotation.m128_f32[0], v_rotation.m128_f32[1], 0.0f);
	XMVECTOR v_camTarget = XMVector3TransformCoord(v_defForward, m_camRotation);
	v_camTarget += v_eye;
	XMVECTOR v_upDirection = XMVector3TransformCoord(v_defUp, m_camRotation);
	m_view = XMMatrixLookAtLH(v_eye, v_camTarget, v_upDirection);
}

DirectX::XMMATRIX Camera::GetViewMatrix() const
{
	return m_view;
}

DirectX::XMMATRIX Camera::GetProjectionMatrix() const
{
	return m_projection;
}
