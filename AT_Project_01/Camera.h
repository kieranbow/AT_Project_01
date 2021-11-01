#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Camera
{
	public:
		Camera();
		~Camera() = default;

		Camera(const Camera&) = delete;
		Camera& operator=(const Camera&) = delete;

		void SetPosition(XMVECTOR position);
		void SetRotation(XMVECTOR rotation);

		void UpdatePosition(XMVECTOR position);
		void UpdateRotation(XMVECTOR rotation);

		void SetTarget(XMVECTOR target);

		void SetLookAt(XMVECTOR position);

		void Update(double dt);

		DirectX::XMMATRIX GetViewMatrix() const;
		DirectX::XMMATRIX GetProjectionMatrix() const;

	private:
		// Camera Propertices
		float width = 800.0f;
		float height = 600.0f;
		float fovDeg = 90.0f;
		float fovRad = 0.0f;
		float aspectRatio = 0.0f;
		float nearZ = 0.01f;
		float farZ = 1000.0f;
		bool enableOrthographic = false;

		XMMATRIX m_view;
		XMMATRIX m_projection;

		XMVECTOR v_eye = XMVectorSet(0.0f, 0.0f, -10.0f, 0.0f);
		XMVECTOR v_target = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR v_up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMVECTOR v_rotation = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		XMVECTOR v_defForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		XMVECTOR v_defUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
};
