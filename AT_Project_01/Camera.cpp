#include "Camera.h"

Camera::Camera()
{
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	positionVector = XMLoadFloat3(&position);

	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotationVector = XMLoadFloat3(&rotation);

	// View matrix
	UpdateViewMatrix();

	width = 800.0f;
	height = 600.0f;

	// Setting projection matrix
	fovDeg = 90.0f;
	fovRad = (fovDeg / 360.0f) * XM_2PI;
	aspectRatio = width / height;
	nearZ = 0.1f;
	farZ = 1000.0f;
	projectionMatrix = XMMatrixPerspectiveFovLH(fovRad, aspectRatio, nearZ, farZ);
}

void Camera::SetPosition(float x, float y, float z)
{
	position = XMFLOAT3(x, y, z);
	positionVector = XMLoadFloat3(&position);
	UpdateViewMatrix();
}

void Camera::SetRotation(float x, float y, float z)
{
	rotation = XMFLOAT3(x, y, z);
	rotationVector = XMLoadFloat3(&rotation);
	UpdateViewMatrix();
}

void Camera::UpdatePosition(XMVECTOR pos)
{
	positionVector += pos;
	XMStoreFloat3(&position, positionVector);

	UpdateViewMatrix();
}

void Camera::UpdateRotation(float x, float y, float z)
{
	rotation.x += x;
	rotation.y += y;
	rotation.z += z;
	rotationVector = XMLoadFloat3(&rotation);
	UpdateViewMatrix();
}

void Camera::UpdateRotation(XMVECTOR rot)
{
	rotationVector += rot;
	XMStoreFloat3(&rotation, rotationVector);
	UpdateViewMatrix();
}

void Camera::SetLookAt(float x, float y, float z)
{
	if (x == position.x && y == position.y && z == position.z)
	{
		return;
	}
	x = position.x - x;
	y = position.y - y;
	z = position.z - z;

	float pitch = 0.0f;
	if (y != 0.0f)
	{
		const float distance = sqrt((x * x) + (z * z));
		pitch = atan(y / distance);
	}

	float yaw = 0.0f;
	if (x != 0.0f)
	{
		yaw = atan(x / z);
	}

	if (z > 0)
	{
		yaw += XM_PI;
	}

	SetRotation(pitch, yaw, 0.0f);
}

void Camera::Update(double dt)
{
	camRotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, 0.0f);
	camTarget = XMVector3TransformCoord(defaultForward, camRotationMatrix);
	camTarget = XMVector3Normalize(camTarget);

	XMMATRIX rotateYTempMatrix;
	rotateYTempMatrix = XMMatrixRotationY(yaw);

	camRight = XMVector3Transform(defaultRight, rotateYTempMatrix);
	camForward = XMVector3Transform(defaultForward, rotateYTempMatrix);
	camUp = XMVector3Transform(camForward, rotateYTempMatrix);

	camPosition += strife * camRight;
	camPosition += backforward * camForward;

	strife = 0.0f;
	backforward = 0.0f;

	camTarget = camPosition + camTarget;

	camView = XMMatrixLookAtLH(camPosition, camTarget, camUp);
}

DirectX::XMVECTOR& Camera::GetFowardVector()
{
	return forwardVector;
}

DirectX::XMVECTOR& Camera::GetRightVector()
{
	return rightVector;
}

DirectX::XMVECTOR& Camera::GetBackwardVector()
{
	return backwardVector;
}

DirectX::XMVECTOR& Camera::GetLeftVector()
{
	return leftVector;
}

DirectX::XMMATRIX Camera::GetViewMatrix() const
{
	return viewMatrix;
}

DirectX::XMMATRIX Camera::GetProjectionMatrix() const
{
	return projectionMatrix;
}

void Camera::UpdateViewMatrix()
{
	float& x = rotation.x;
	float& y = rotation.y;
	float& z = rotation.z;


	//XMMATRIX camRotMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	//XMVECTOR camTarget = XMVector3TransformCoord(def_forwardVector, camRotMatrix);
	//camTarget += positionVector;
	//XMVECTOR upDirection = XMVector3TransformCoord(def_upVector, camRotMatrix);
	//viewMatrix = XMMatrixLookAtLH(positionVector, camTarget, upDirection);

	//XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(0.0f, rotation.y, 0.0f);
	//forwardVector		= XMVector3TransformCoord(def_forwardVector, vecRotationMatrix);
	//backwardVector	= XMVector3TransformCoord(def_backwardVector, vecRotationMatrix);
	//leftVector			= XMVector3TransformCoord(def_leftVector, vecRotationMatrix);
	//rightVector			= XMVector3TransformCoord(def_rightVector, vecRotationMatrix);
}
