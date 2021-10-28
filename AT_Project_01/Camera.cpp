#include "Camera.h"

Camera::Camera()
{
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	positionVector = XMLoadFloat3(&position);

	rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotationVector = XMLoadFloat3(&rotation);

	UpdateViewMatrix();
	
	fovDeg = 59.0f;
	fovRad = (fovDeg / 360.0f) * XM_2PI;
	projectionMatrix = XMMatrixPerspectiveFovLH(fovRad, 800.0f / 600.0f, 0.01f, 1000.0f);
}

Camera::Camera(XMFLOAT3 _position, XMFLOAT3 _rotation, float _fovDeg, float _fovRad, float _width, float _height, float _aspectRatio, float _nearZ, float _farZ)
{
	position = _position;
	positionVector = XMLoadFloat3(&position);

	rotation = _rotation;
	rotationVector = XMLoadFloat3(&rotation);

	UpdateViewMatrix();


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

void Camera::UpdatePosition(float x, float y, float z)
{
	position.x += x;
	position.y += y;
	position.z += z;

	positionVector = XMLoadFloat3(&position);
	UpdateViewMatrix();
}

void Camera::UpdateRotation(float x, float y, float z)
{
	rotation = XMFLOAT3(x, y, z);
	rotationVector = XMLoadFloat3(&rotation);
	UpdateViewMatrix();
}

void Camera::UpdateViewMatrix()
{
	XMMATRIX camRotMatrix = XMMatrixRotationRollPitchYaw(rotation.x, rotation.y, rotation.z);
	XMVECTOR camTarget = XMVector3TransformCoord(forwardVector, camRotMatrix);
	camTarget += positionVector;
	XMVECTOR upDirection = XMVector3TransformCoord(upVector, camRotMatrix);
	viewMatrix = XMMatrixLookAtLH(positionVector, camTarget, upDirection);
}
