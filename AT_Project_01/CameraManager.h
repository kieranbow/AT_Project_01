#pragma once
#include <DirectXMath.h>
#include <unordered_map>
#include "Camera.h"

// Forward declaration
class Graphics;

using camera_ID = unsigned int;

// A struct to contain all IDs for cameras.
struct CamID
{
	const static camera_ID static_cam = 0u;
	const static camera_ID player_cam = 1u;
	const static camera_ID cine_cam_1 = 3u;
	const static camera_ID cine_cam_2 = 4u;
	const static camera_ID cine_cam_3 = 5u;
	const static camera_ID cine_cam_4 = 6u;
	const static camera_ID cine_cam_5 = 7u;
};

// A simple camera manager that uses a finite state machine to update cameras in a given scene
class CameraManager
{
	public:
		CameraManager() = default;
		~CameraManager() = default;

		CameraManager(const CameraManager&) = delete;
		CameraManager& operator=(const CameraManager&) = delete;

		void AddCamera(const std::shared_ptr<Camera>& camera, camera_ID ID);
		void ChangeCamera(camera_ID cameraID);

		void Update(float dt);
		void Draw(Graphics* pGfx);

		DirectX::XMMATRIX GetCurrentCameraViewMatrix() const;
		DirectX::XMMATRIX GetCurrentCameraProjectionMatrix() const;
		std::shared_ptr<Camera> GetCurrentCamera() const;

	private:
		std::unordered_map<camera_ID, std::shared_ptr<Camera>> cameras;
		std::shared_ptr<Camera> currentCamera;

		camera_ID ID = 0;
};
