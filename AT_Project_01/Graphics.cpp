#include "Graphics.h"
#include "ErrorChecker.h"

Graphics::Graphics(HWND hwnd, int window_width, int window_height)
{
	// ----------------------------------------------------------------
	// Window Size
	windowSize.first	= static_cast<float>(window_width);
	windowSize.second	= static_cast<float>(window_height);

	// ----------------------------------------------------------------
	// Create Swap Chain Description
	DXGI_SWAP_CHAIN_DESC swapChainDesc = {};
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));
	swapChainDesc.BufferDesc.Width	= 0;
	swapChainDesc.BufferDesc.Height	= 0;
	swapChainDesc.BufferDesc.Format	= DXGI_FORMAT_B8G8R8A8_UNORM;
	swapChainDesc.BufferDesc.RefreshRate.Numerator		= 0;
	swapChainDesc.BufferDesc.RefreshRate.Denominator	= 0;
	swapChainDesc.BufferDesc.Scaling	= DXGI_MODE_SCALING_UNSPECIFIED;
	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.SampleDesc.Count		= 1;	// Anti-aliasing
	swapChainDesc.SampleDesc.Quality	= 0;
	swapChainDesc.BufferUsage			= DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount			= 1;
	swapChainDesc.OutputWindow			= hwnd;
	swapChainDesc.Windowed				= TRUE;
	swapChainDesc.SwapEffect			= DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags					= 0;

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
	Logging::ThrowIf(hResult, "SwapChain Failed");

	// ----------------------------------------------------------------
	// Back Buffer
	hResult = pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	Logging::ThrowIf(hResult, "Swap Chain failed to get buffer");

	hResult = pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, &pRenderTargetView);
	Logging::ThrowIf(hResult, "Device failed to create render target view");

	// ----------------------------------------------------------------
	// Depth buffer
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable	= TRUE;
	depthStencilDesc.DepthWriteMask	= D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc		= D3D11_COMPARISON_LESS;

	hResult = pDevice->CreateDepthStencilState(&depthStencilDesc, &pDSState); // Create Depth State
	Logging::ThrowIf(hResult, "Device Failed to create Depth Stencil State");
	
	pDeviceContext->OMSetDepthStencilState(pDSState.Get(), 1u); // Bind Depth state

	// ----------------------------------------------------------------
	// Depth Stencil
	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width		= static_cast<UINT>(windowSize.first);
	depthDesc.Height	= static_cast<UINT>(windowSize.second);
	depthDesc.MipLevels	= 1u; // Only use one mip level
	depthDesc.ArraySize	= 1u;
	depthDesc.Format	= DXGI_FORMAT_D32_FLOAT;
	depthDesc.SampleDesc.Count		= 1u; // anti-alising
	depthDesc.SampleDesc.Quality	= 0u;
	depthDesc.Usage		= D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags	= D3D11_BIND_DEPTH_STENCIL;

	hResult = pDevice->CreateTexture2D(&depthDesc, NULL, &pDepthStencil);
	Logging::ThrowIf(hResult, "Device Failed To Create Depth Texture");

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format				= DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension		= D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice	= 0u;

	pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDepthView);
	Logging::ThrowIf(hResult, "Device Failed To Create Depth Stencil View");

	// ----------------------------------------------------------------
	// Rasterizer State
	D3D11_RASTERIZER_DESC rasterizerDesc;
	ZeroMemory(&rasterizerDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasterizerDesc.FillMode	= D3D11_FILL_SOLID;
	rasterizerDesc.CullMode	= D3D11_CULL_BACK;

	hResult = pDevice->CreateRasterizerState(&rasterizerDesc, &pRasterizerState);
	Logging::ThrowIf(hResult, "Device failed to create rasterizer state");

	// ----------------------------------------------------------------
	// Viewport
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(vp));
	vp.Width	= windowSize.first;
	vp.Height	= windowSize.second;
	vp.MinDepth	= 0;
	vp.MaxDepth	= 1;
	vp.TopLeftX	= 0;
	vp.TopLeftY	= 0;
	pDeviceContext->RSSetViewports(1u, &vp);

	// ----------------------------------------------------------------
	// Input Element Description
	inputElemDesc[0] = { "POSITION",0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	inputElemDesc[1] = { "NORMAL",	0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
	inputElemDesc[2] = { "TEXCOORD",0, DXGI_FORMAT_R32G32_FLOAT,	0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 };
		
	// ----------------------------------------------------------------
	// Output merger
	pDeviceContext->OMSetRenderTargets(1u, pRenderTargetView.GetAddressOf(), pDepthView.Get());
}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[] = { red, green, blue, 1.0f };
	pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), color);
	pDeviceContext->ClearDepthStencilView(pDepthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 1u);
}

ID3D11Device* Graphics::GetDevice() const
{
	return pDevice.Get();
}

ID3D11DeviceContext* Graphics::GetDeviceContext() const
{
	return pDeviceContext.Get();
}

std::pair<float, float> Graphics::GetWindowSize() const
{
	return windowSize;
}

DirectX::XMMATRIX Graphics::GetViewMatrix() const
{
	return m_view;
}

DirectX::XMMATRIX Graphics::GetProjectionMatrix() const
{
	return m_projection;
}

UINT Graphics::GetSizeOfInputElemDesc() const
{
	return static_cast<UINT>(std::size(inputElemDesc));
}

void Graphics::Present()
{
	hResult = pSwapChain->Present(1u, 0u);
	Logging::ThrowIf(hResult, "Swapchain failed to present");
}

void Graphics::SetViewMatrix(DirectX::XMMATRIX view)
{
	m_view = view;
}

void Graphics::SetProjectionMatrix(DirectX::XMMATRIX projection)
{
	m_projection = projection;
}
