#pragma once
#include <DirectXMath.h>
#include <memory>

#include "Direction.h"

using namespace DirectX;

// Camera class that allows a camera to be positioned, rotated in game.
class Camera
{
	public:
		Camera();
		Camera(float _width, float _height, float _fov, float _nearZ, float _farZ, bool _enableOrthoView);
		~Camera() = default;

		// Sets the size of the camera for the projection matrix
		void SetSize(std::pair<float, float>size);

		void SetPosition(XMVECTOR position);
		void SetRotation(XMVECTOR rotation);

		void UpdatePosition(XMVECTOR position);
		void UpdateRotation(XMVECTOR rotation);

		void SetTarget(XMVECTOR target);
		void SetLookAt(XMFLOAT3 position);

		void EnableCamera(bool _enable);
		bool IsActive();

		void Update(double dt);

		DirectX::XMMATRIX GetViewMatrix() const;
		DirectX::XMMATRIX GetProjectionMatrix() const;

		Direction GetDirection() const;

		XMVECTOR GetPosition() const
		{
			return v_eye;
		}

		XMFLOAT3 GetPositionFloat() const 
		{
			return XMFLOAT3({v_eye.m128_f32[0], v_eye.m128_f32[1] , v_eye.m128_f32[2] });
		}

		XMVECTOR GetRotation() const;

	private:
		// Camera Propertices
		float width				= 800.0f;
		float height			= 600.0f;
		float fovDeg			= 90.0f;
		float fovRad			= 0.0f;
		float aspectRatio		= 0.0f;
		float nearZ				= 0.01f;
		float farZ				= 10000.0f;
		bool enableOrthographic = false;
		bool enable				= false;

		Direction direction;

		// Projection/View Matrix
		XMMATRIX m_view;
		XMMATRIX m_projection;

		// Camera Vectors
		XMVECTOR v_eye		= XMVectorSet(0.0f, 0.0f, -10.0f, 0.0f);
		XMVECTOR v_target	= XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR v_up		= XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMVECTOR v_rotation	= XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		// Default movement Vectors
		const XMVECTOR v_defForward		= XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		const XMVECTOR v_defUp			= XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		const XMVECTOR v_defBackward	= XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
		const XMVECTOR v_defLeft		= XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
		const XMVECTOR v_defRight		= XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
};
