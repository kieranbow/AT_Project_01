#pragma once
#include <unordered_map>
#include "Camera.h"

using camera_ID = unsigned int;

class Graphics;

class CameraManager
{
	public:
		CameraManager() = default;
		~CameraManager() = default;

		CameraManager(const CameraManager&) = delete;
		CameraManager& operator=(const CameraManager&) = delete;

		void AddCamera(const std::shared_ptr<Camera>& camera);

		void Update(float dt);
		void Draw(Graphics* pGfx);

	private:
		std::unordered_map<camera_ID, std::shared_ptr<Camera>> cameras;
		std::shared_ptr<Camera> currentCamera;

		camera_ID ID = 0;
};
