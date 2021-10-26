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
#include "ConstantBuffer.h"
#include "VSShader.h"
#include "PSShader.h"

class Graphics
{
	public:
		Graphics(HWND hwnd);
		~Graphics() = default;

		Graphics(const Graphics&) = delete;
		Graphics& operator=(const Graphics&) = delete;

		void InitalizeIAstage();

		void InitalizeRenderData();

		void ClearBuffer(float red, float green, float blue);
		void DrawFrame();
		void EndFrame();



		void drawTriangle(float x, float y);

		// Getters
		ID3D11Device* GetDevice() const;
		ID3D11DeviceContext* GetDeviceContext() const;

	private:
		HRESULT hResult;

		Microsoft::WRL::ComPtr<ID3D11Device> pDevice;
		Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDeviceContext;
		Microsoft::WRL::ComPtr<IDXGISwapChain> pSwapChain;
		Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pRenderTargetView;

		//----------
		// IA Stage
		//----------

		
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout; // Input Layout

		//Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;

		VertexBuffer vertexBuffer;
		IndexBuffer indexBuffer;
		
		VSShader vsShader;
		PSShader psShader;
		
		// Constant Buffers
		ConstantBuffer vertexConstBuffer;
		ConstantBuffer pixelConstBuffer;
		
		Microsoft::WRL::ComPtr<ID3D11RasterizerState> rasterizerState;

		Microsoft::WRL::ComPtr<ID3D11DepthStencilView> pDepthView;

};
