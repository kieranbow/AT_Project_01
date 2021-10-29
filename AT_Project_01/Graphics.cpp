#include "Graphics.h"
#include "ErrorChecker.h"

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
	Logging::ThrowIf(hResult, "SwapChain Failed");

	// Back Buffer
	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;

	hResult = pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	Logging::ThrowIf(hResult, "Swap Chain failed to get buffer");

	hResult = pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, &pRenderTargetView);
	Logging::ThrowIf(hResult, "Device failed to create render target view");

	// Depth buffer
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Create Depth State
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;

	hResult = pDevice->CreateDepthStencilState(&depthStencilDesc, &pDSState);
	Logging::ThrowIf(hResult, "Device Failed to create Depth Stencil State");

	// Bind Depth state
	pDeviceContext->OMSetDepthStencilState(pDSState.Get(), 1u);

	Microsoft::WRL::ComPtr<ID3D11Texture2D> pDepthStencil;
	D3D11_TEXTURE2D_DESC depthDesc = {};
	depthDesc.Width = 800u;
	depthDesc.Height = 600u;
	depthDesc.MipLevels = 1u; // Only use one mip level
	depthDesc.ArraySize = 1u;
	depthDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthDesc.SampleDesc.Count = 1u; // anti-alising
	depthDesc.SampleDesc.Quality = 0u;
	depthDesc.Usage = D3D11_USAGE_DEFAULT;
	depthDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hResult = pDevice->CreateTexture2D(&depthDesc, NULL, &pDepthStencil);
	Logging::ThrowIf(hResult, "Device Failed To Create Depth Texture");

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;

	pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDepthView);
	Logging::ThrowIf(hResult, "Device Failed To Create Depth Stencil View");

	pDeviceContext->OMSetRenderTargets(1u, pRenderTargetView.GetAddressOf(), pDepthView.Get());
}

void Graphics::SetViewport()
{
	// configure viewport
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(vp));
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pDeviceContext->RSSetViewports(1u, &vp);
}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[] = { red, green, blue, 1.0f };
	pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), color);
	pDeviceContext->ClearDepthStencilView(pDepthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 1u);
}

void Graphics::DrawFrame()
{
	theCube = std::make_unique<DaCube>(pDevice.Get(), pDeviceContext.Get());
	theCube->Draw(pDeviceContext.Get());

	cube20 = std::make_unique<DaCube>(pDevice.Get(), pDeviceContext.Get());
	cube20->Draw(pDeviceContext.Get());
}

void Graphics::drawTriangle(float x, float y)
{

	const std::vector<Vertex> cubeVertices =
	{
		{-1.0f, -1.0f, -1.0f,	1.0f, 0.0f, 0.0f},
		{1.0f, -1.0f, -1.0f,	0.0f, 1.0f, 0.0f},
		{-1.0f, 1.0f, -1.0f,	0.0f, 0.0f, 1.0f},
		{1.0f, 1.0f, -1.0f,		1.0f, 0.0f, 0.0f},
		{-1.0f, -1.0f, 1.0f,	0.0f, 1.0f, 0.0f},
		{1.0f, -1.0f, 1.0f,		0.0f, 0.0f, 1.0f},
		{-1.0f, 1.0f, 1.0f,		1.0f, 0.0f, 0.0f},
		{1.0f, 1.0f, 1.0f,		0.0f, 1.0f, 0.0f},
	};

	// Create Vertex Buffer
	hResult = vertexBuffer.CreateVertexBuffer(pDevice.Get(), cubeVertices, 0u);
	Logging::ThrowIf(hResult, "Device Failed to create vertex buffer");

	// Bind Vertex Buffer
	vertexBuffer.BindBuffer(0u, 1u ,pDeviceContext.Get());

	const std::vector<unsigned short> cubeIndices =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4,
	};

	// Create Index Buffer
	hResult = indexBuffer.CreateIndexBuffer(pDevice.Get(), cubeIndices);
	Logging::ThrowIf(hResult, "Index Buffer Failed To Create Buffer");

	// Bind Index Buffer to pipeline
	indexBuffer.BindBuffer(pDeviceContext.Get(), DXGI_FORMAT_R16_UINT, 0u);


	// create constant buffer for transformation matrix
	struct ConstantBuffer
	{
		DirectX::XMMATRIX transform;
	};

	const ConstantBuffer cb =
	{
		{
			DirectX::XMMatrixTranspose(
				DirectX::XMMatrixRotationZ(35) *
				DirectX::XMMatrixRotationX(20) *
				DirectX::XMMatrixTranslation(x, y, 4.0f) *
				DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f ,0.5f, 10.0f)
			)
		}
	};

	// Create Constant buffer
	hResult = vertexConstBuffer.CreateConstantBuffer(pDevice.Get(), cb);
	Logging::ThrowIf(hResult, "Device Failed to VS create Constant Buffer");

	// Bind Constant Buffer to pipeline
	vertexConstBuffer.SetVSConstBuffer(pDeviceContext.Get(), 0u, 1u);



	// Read vertex shader
	vsShader.ReadVSShaderToBlob(L"..\\x64\\Debug\\VertexShader.cso");
	Logging::ThrowIf(hResult, "Failed to read Vertex Shader");

	// Create Vertex Shader
	vsShader.CreateVSShader(pDevice.Get());
	Logging::ThrowIf(hResult, "Device Failed to Create Vertex Shader");

	// Bind Vertex Shader
	vsShader.SetVSShader(pDeviceContext.Get(), 0u);

	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	hResult = pDevice->CreateInputLayout(
		ied,
		(UINT)std::size(ied),
		vsShader.GetVSBlob()->GetBufferPointer(),
		vsShader.GetVSBlob()->GetBufferSize(),
		&pInputLayout);

	Logging::ThrowIf(hResult, "Device Failed to create Input Layout");



	// Read Pixel Shader
	hResult = psShader.ReadPSShaderToBlob(L"..\\x64\\Debug\\PixelShader.cso");
	Logging::ThrowIf(hResult, "Failed to read Pixel Shader");

	// Create Pixel Shader
	hResult = psShader.CreatePSShader(pDevice.Get());
	Logging::ThrowIf(hResult, "Failed to create pixel Shader");

	// Bind Pixel Shader
	psShader.SetPSShader(pDeviceContext.Get(), 0u);



	// bind vertex layout
	pDeviceContext->IASetInputLayout(pInputLayout.Get());

	// Set primitive topology to triangle list (groups of 3 vertices)
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	// configure viewport
	D3D11_VIEWPORT vp;
	ZeroMemory(&vp, sizeof(vp));
	vp.Width = 800;
	vp.Height = 600;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pDeviceContext->RSSetViewports(1u, &vp);


	// pDeviceContext->DrawIndexed((UINT)std::size(indices), 0u, 0u);
	pDeviceContext->DrawIndexed(static_cast<UINT>(cubeIndices.size()), 0u, 0u);
}

ID3D11Device* Graphics::GetDevice() const
{
	return pDevice.Get();
}

ID3D11DeviceContext* Graphics::GetDeviceContext() const
{
	return pDeviceContext.Get();
}

void Graphics::EndFrame()
{
	hResult = pSwapChain->Present(1u, 0u);
	Logging::ThrowIf(hResult, "Swapchain failed to present");
}
