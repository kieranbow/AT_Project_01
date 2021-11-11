#pragma once
// DirectX 11
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")
#pragma comment(lib, "DXGI.lib")

// Microsoft
#include <wrl/client.h>

// C++
#include <memory>

// Set rasterizer inside draw RSSetState

#include "Camera.h"

// The class handles all the main rendering for the game.
class Graphics
{
	public:
		Graphics(HWND hwnd, int window_width, int window_height);
		~Graphics() = default;

		// Prevents the graphic class from being copied
		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;

		// Uses DirectX ClearRenderTargetView to clear the render with a color
		void ClearBuffer(float red, float green, float blue);

		// Calls the swapchain's Present function to render to screen
		void Present();

		// Allow a camera to set the view matrix
		void SetViewMatrix(DirectX::XMMATRIX view);

		// Allows a camera to set the projection matrix
		void SetProjectionMatrix(DirectX::XMMATRIX projection);

		// Getters
		ID3D11Device* GetDevice() const;
		ID3D11DeviceContext* GetDeviceContext() const;
		std::pair<float, float> GetWindowSize() const;

		// All objects will get this view matrix
		DirectX::XMMATRIX GetViewMatrix() const;

		// All objects will get this projection matrix
		DirectX::XMMATRIX GetProjectionMatrix() const;

		Camera currentCamera;

	private:
		HRESULT hResult;
		Microsoft::WRL::ComPtr<ID3D11Device>			pDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>		pDeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain>			pSwapChain;
		Microsoft::WRL::ComPtr<ID3D11Resource>			pBackBuffer;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	pRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>		pInputLayout;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>	pRasterizerState;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	pDepthView;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;
		Microsoft::WRL::ComPtr<ID3D11Texture2D>			pDepthStencil;

		// An active camera will set these matrices and all objects will get these matrices for mvp
		DirectX::XMMATRIX m_view = DirectX::XMMATRIX();
		DirectX::XMMATRIX m_projection = DirectX::XMMATRIX();

		std::pair<float, float> windowSize;
};
