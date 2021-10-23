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

// Graphic Utility
#include "Vertex.h"

class Graphics
{
	public:
		Graphics(HWND hwnd);
		~Graphics() = default;

		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;

		void ClearBuffer(float red, float green, float blue);

		void CreateVertexBuffer();

		void VertexShader(); // <-Refactor this

		void InitializeInputLayout(); // <-Refactor this

		void CreatePixelBuffer();

		void PixelShader(); // <-Refactor this

		void drawTriangle(float x, float y);
		void drawTestTriangle();

		void CreateViewport();

		void EndFrame();

	private:
		HRESULT hResult;

		Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView;
		
		// Input Layout
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;

		// Vertex Buffer
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;

		// Vertex Shader
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
		Microsoft::WRL::ComPtr<ID3D10Blob> pVertexShaderBuffer;

		// Pixel Buffer
		Microsoft::WRL::ComPtr<ID3D11Buffer> pPixelBuffer;

		// Pixel Shader
		Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
		Microsoft::WRL::ComPtr<ID3D10Blob> pPixelShaderBuffer;

		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthView;

};
