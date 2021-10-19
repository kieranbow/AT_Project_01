#pragma once
// DirectX 11
#include <d3d11.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"DXGI.lib")

// Microsoft
#include <wrl/client.h>

// C++

class Graphics
{
	public:
		Graphics(HWND hwnd);
		~Graphics() = default;

		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;

		void ClearBuffer(float red, float green, float blue);
		void EndFrame();

	private:
		HRESULT hResult;

		Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView;
};
