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

		void SetPosition(float x, float y, float z);
		void SetRotation(float x, float y, float z);

		void UpdatePosition(float x, float y, float z);
		void UpdatePosition(XMVECTOR pos);

		void UpdateRotation(float x, float y, float z);
		void UpdateRotation(XMVECTOR rot);

		void SetLookAt(float x, float y, float z);

		void Update(double dt);

		DirectX::XMVECTOR& GetFowardVector();
		DirectX::XMVECTOR& GetRightVector();
		DirectX::XMVECTOR& GetBackwardVector();
		DirectX::XMVECTOR& GetLeftVector();

		DirectX::XMMATRIX GetViewMatrix() const;
		DirectX::XMMATRIX GetProjectionMatrix() const;


		// new stuff

		XMMATRIX camView;
		XMVECTOR camTarget = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		XMVECTOR camUp = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		XMVECTOR camPosition = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

		XMVECTOR defaultForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		XMVECTOR defaultRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

		XMVECTOR camForward = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		XMVECTOR camRight = XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

		XMMATRIX camRotationMatrix;

		float yaw = 0.0f;
		float pitch = 0.0f;

		float strife = 0.0f;
		float backforward = 0.0f;

		// new stuff


	private:
		void UpdateViewMatrix();

		XMVECTOR positionVector	= { 0.0f, 0.0f, 0.0f };
		XMVECTOR rotationVector	= { 0.0f, 0.0f, 0.0f };
		XMFLOAT3 position			= { 0.0f, 0.0f, 0.0f };
		XMFLOAT3 rotation			= { 0.0f, 0.0f, 0.0f };
		XMMATRIX viewMatrix;
		XMMATRIX projectionMatrix;


		const XMVECTOR def_forwardVector	= XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
		const XMVECTOR def_upVector			= XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
		const XMVECTOR def_backwardVector	= XMVectorSet(0.0f, 0.0f, -1.0f, 0.0f);
		const XMVECTOR def_leftVector			= XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f);
		const XMVECTOR def_rightVector		= XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

		XMVECTOR forwardVector;
		XMVECTOR leftVector;
		XMVECTOR rightVector;
		XMVECTOR backwardVector;

		XMVECTOR eyePos = DirectX::XMVectorSet(0.0f, 0.0f, -10.0f, 0.0f);
		XMVECTOR lookAtPos = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);
		XMVECTOR upVector = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);



		float width = 0.0f;
		float height = 0.0f;
		float fovDeg = 0.0f;
		float fovRad = 0.0f;
		float aspectRatio = 0.0f;
		float nearZ = 0.0f;
		float farZ = 0.0f;
		bool enableOrthographic = false;
};

