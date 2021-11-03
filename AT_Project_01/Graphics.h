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

class Graphics
{
	public:
		Graphics(HWND hwnd, int window_width, int window_height);
		~Graphics() = default;

		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;

		void ClearBuffer(float red, float green, float blue);
		void EndFrame();

		void SetViewMatrix(DirectX::XMMATRIX view);
		void SetProjectionMatrix(DirectX::XMMATRIX projection);

		// Getters
		ID3D11Device* GetDevice() const;
		ID3D11DeviceContext* GetDeviceContext() const;
		std::pair<float, float> GetWindowSize() const;

		DirectX::XMMATRIX GetViewMatrix() const;
		DirectX::XMMATRIX GetProjectionMatrix() const;

	private:
		HRESULT hResult;
		Microsoft::WRL::ComPtr<ID3D11Device>					pDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext>		pDeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView>	pRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout;
		Microsoft::WRL::ComPtr<ID3D11RasterizerState>		pRasterizerState;
		Microsoft::WRL::ComPtr<ID3D11DepthStencilView>	pDepthView;

		DirectX::XMMATRIX m_view;
		DirectX::XMMATRIX m_projection;

		std::pair<float, float> windowSize;
};
