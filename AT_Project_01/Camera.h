#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
	public:
		Camera();
		Camera(XMFLOAT3 _position, XMFLOAT3 _rotation, float _fovDeg, float _fovRad, float _width, float _height, float _aspectRatio, float _nearZ, float _farZ);
		~Camera() = default;

		Camera(const Camera&) = delete;
		Camera& operator=(const Camera&) = delete;

		void SetPosition(float x, float y, float z);
		void SetRotation(float x, float y, float z);
		void UpdatePosition(float x, float y, float z);
		void UpdateRotation(float x, float y, float z);

	private:
		void UpdateViewMatrix();

		XMVECTOR positionVector	= { 0.0f, 0.0f, 0.0f };
		XMVECTOR rotationVector	= { 0.0f, 0.0f, 0.0f };
		XMFLOAT3 position			= { 0.0f, 0.0f, 0.0f };
		XMFLOAT3 rotation			= { 0.0f, 0.0f, 0.0f };
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;

		const XMVECTOR forwardVector	= XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		const XMVECTOR upVector			= XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		float width = 0.0f;
		float height = 0.0f;
		float fovDeg = 0.0f;
		float fovRad = 0.0f;
		float aspectRatio = 0.0f;
		float nearZ = 0.0f;
		float farZ = 0.0f;
		bool enableOrthographic = false;
};

