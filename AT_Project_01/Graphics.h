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

#include "VertexBuffer.h"
#include "IndexBuffer.h"

class Graphics
{
	public:
		// Pipeline
		// Input Assembler
		// Vertex Shader
		// Tessellation
		// Geometry Shader
		// Render Target
		// Output merger
		// Pixel Shader
		// Rasterizer

		Graphics(HWND hwnd);
		~Graphics() = default;

		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;

		void CreateViewport();

		void InitalizeIAstage();

		void InitalizeRenderData();

		void ClearBuffer(float red, float green, float blue);
		void DrawFrame();
		void EndFrame();



		void drawTriangle(float x, float y);

		Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView;

	private:
		HRESULT hResult;

		//----------
		// IA Stage
		//----------

		
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout; // Input Layout

		
		//Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
		VertexBuffer vertexBuffer; // Vertex Buffer
		IndexBuffer indexBuffer; // IndexBuffer


		
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
