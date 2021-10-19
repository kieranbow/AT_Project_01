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

struct Vertex
{
	// Struct Constructors
	Vertex(){}
	Vertex(float x, float y) : position(x, y) {}

	DirectX::XMFLOAT2 position;
};


class Graphics
{
	public:
		Graphics(HWND hwnd);
		~Graphics() = default;

		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;

		void ClearBuffer(float red, float green, float blue);

		void VertexShader();

		void InitLayout(); // change this

		void EndFrame();

	private:
		HRESULT hResult;

		Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;

		// Vertex Shader
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
		Microsoft::WRL::ComPtr<ID3D10Blob> pVertexShaderBuffer;
};
