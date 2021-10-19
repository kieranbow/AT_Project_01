#include "Graphics.h"
#include <array>

Graphics::Graphics(HWND hwnd)
{
	// Create Swap Chain Description
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferDesc.Width = 0;
	swapChainDesc.BufferDesc.Height = 0;
	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = 0;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	
	// Anti-aliasing
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;

	// Specify usage of buffer for outputing
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 1;

	swapChainDesc.OutputWindow = hwnd;
	swapChainDesc.Windowed = TRUE;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	// Create Device and swap chain 
	hResult = D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL,
		0,
		D3D11_SDK_VERSION,
		&swapChainDesc,
		&pSwapChain,
		&pDevice,
		NULL,
		&pDeviceContext
	);

	// Back Buffer
	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer); // Add error checker
	pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, &pRenderTargetView);

}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[] = { red, green, blue, 1.0f };
	pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), color);
}

void Graphics::EndFrame()
{
	pSwapChain->Present(1u, 0u);
}
