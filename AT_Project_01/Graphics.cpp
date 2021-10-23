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
	ErrorChecker::ThrowIf(hResult, "SwapChain Failed");

	// Back Buffer
	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;

	hResult = pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer);
	ErrorChecker::ThrowIf(hResult, "Swap Chain failed to get buffer");

	hResult = pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, &pRenderTargetView);
	ErrorChecker::ThrowIf(hResult, "Device failed to create render target view");

	// Depth buffer
	D3D11_DEPTH_STENCIL_DESC depthStencilDesc = {};
	depthStencilDesc.DepthEnable = TRUE;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	// Create Depth State
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> pDSState;

	hResult = pDevice->CreateDepthStencilState(&depthStencilDesc, &pDSState);
	ErrorChecker::ThrowIf(hResult, "Device Failed to create Depth Stencil State");

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
	ErrorChecker::ThrowIf(hResult, "Device Failed To Create Depth Texture");

	D3D11_DEPTH_STENCIL_VIEW_DESC descDSV = {};
	descDSV.Format = DXGI_FORMAT_D32_FLOAT;
	descDSV.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	descDSV.Texture2D.MipSlice = 0u;

	pDevice->CreateDepthStencilView(pDepthStencil.Get(), &descDSV, &pDepthView);
	ErrorChecker::ThrowIf(hResult, "Device Failed To Create Depth Stencil View");

	pDeviceContext->OMSetRenderTargets(1u, pRenderTargetView.GetAddressOf(), pDepthView.Get());
}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[] = { red, green, blue, 1.0f };
	pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), color);
	pDeviceContext->ClearDepthStencilView(pDepthView.Get(), D3D11_CLEAR_DEPTH, 1.0f, 0u);
}

void Graphics::drawTriangle(float x, float y)
{
	// Pipeline
	// Input Assembler
	// Vertex Shader
	// Tessellation
	// Geometry Shader
	// Render Target
	// Output merger
	// Pixel Shader
	// Rasterizer


	// Vertex Buffer
	std::vector<Vertex> cubeVertices;
	cubeVertices.emplace_back(-1.0f,-1.0f,-1.0f, 1.0f, 0, 0);
	cubeVertices.emplace_back(1.0f, -1.0f, -1.0f, 0, 1.0f, 0);
	cubeVertices.emplace_back(-1.0f, 1.0f, -1.0f, 0, 0, 1.0f);
	cubeVertices.emplace_back(1.0f, 1.0f, -1.0f, 1.0f, 0, 0);
	cubeVertices.emplace_back(-1.0f, -1.0f, 1.0f, 0, 1.0f, 0);
	cubeVertices.emplace_back(1.0f, -1.0f, 1.0f, 0, 0, 1.0f);
	cubeVertices.emplace_back(-1.0f, 1.0f, 1.0f, 1.0f, 0, 0);
	cubeVertices.emplace_back(1.0f, 1.0f, 1.0f, 0, 1.0f, 0);

	hResult = vertexBuffer.CreateVertexBuffer(pDevice.Get(), cubeVertices);
	ErrorChecker::ThrowIf(hResult, "Vertex Buffer Failed to Create Buffer");

	vertexBuffer.BindBuffer(pDeviceContext.Get());

	// Index Buffer
	std::vector<unsigned short> cubeIndices;
	cubeIndices.emplace_back(0);
	cubeIndices.emplace_back(2);
	cubeIndices.emplace_back(1);

	cubeIndices.emplace_back(2);
	cubeIndices.emplace_back(3);
	cubeIndices.emplace_back(1);

	cubeIndices.emplace_back(1);
	cubeIndices.emplace_back(3);
	cubeIndices.emplace_back(5);

	cubeIndices.emplace_back(3);
	cubeIndices.emplace_back(7);
	cubeIndices.emplace_back(5);

	cubeIndices.emplace_back(2);
	cubeIndices.emplace_back(6);
	cubeIndices.emplace_back(3);

	cubeIndices.emplace_back(3);
	cubeIndices.emplace_back(6);
	cubeIndices.emplace_back(7);

	cubeIndices.emplace_back(4);
	cubeIndices.emplace_back(5);
	cubeIndices.emplace_back(7);

	cubeIndices.emplace_back(4);
	cubeIndices.emplace_back(7);
	cubeIndices.emplace_back(6);

	cubeIndices.emplace_back(0);
	cubeIndices.emplace_back(4);
	cubeIndices.emplace_back(2);

	cubeIndices.emplace_back(2);
	cubeIndices.emplace_back(4);
	cubeIndices.emplace_back(6);

	cubeIndices.emplace_back(0);
	cubeIndices.emplace_back(1);
	cubeIndices.emplace_back(4);

	cubeIndices.emplace_back(1);
	cubeIndices.emplace_back(5);
	cubeIndices.emplace_back(4);


	hResult = indexBuffer.CreateIndexBuffer(pDevice.Get(), cubeIndices);
	ErrorChecker::ThrowIf(hResult, "Index Buffer Failed To Create Buffer");

	indexBuffer.BindBuffer(pDeviceContext.Get());



	// create constant buffer for transformation matrix
	struct ConstantBuffer
	{
		DirectX::XMMATRIX transform;
	};
	const ConstantBuffer cb =
	{
		{
			DirectX::XMMatrixTranspose(
				DirectX::XMMatrixRotationZ(0) *
				DirectX::XMMatrixRotationX(0) *
				DirectX::XMMatrixTranslation(x, y, 4.0f) *
				DirectX::XMMatrixPerspectiveLH(1.0f, 3.0f / 4.0f ,0.5f, 10.0f)
			)
		}
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;
	D3D11_BUFFER_DESC cbd;
	cbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd.Usage = D3D11_USAGE_DYNAMIC;
	cbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	cbd.MiscFlags = 0u;
	cbd.ByteWidth = sizeof(cb);
	cbd.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd = {};
	csd.pSysMem = &cb;
	pDevice->CreateBuffer(&cbd, &csd, &pConstantBuffer);

	// bind constant buffer to vertex shader
	pDeviceContext->VSSetConstantBuffers(0u, 1u, pConstantBuffer.GetAddressOf());


	// lookup table for cube face colors
	struct ConstantBuffer2
	{
		struct
		{
			float r;
			float g;
			float b;
			float a;
		} face_colors[6];
	};
	const ConstantBuffer2 cb2 =
	{
		{
			{1.0f,0.0f,1.0f},
			{1.0f,0.0f,0.0f},
			{0.0f,1.0f,0.0f},
			{0.0f,0.0f,1.0f},
			{1.0f,1.0f,0.0f},
			{0.0f,1.0f,1.0f},
		}
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer2;
	D3D11_BUFFER_DESC cbd2;
	cbd2.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	cbd2.Usage = D3D11_USAGE_DEFAULT;
	cbd2.CPUAccessFlags = 0u;
	cbd2.MiscFlags = 0u;
	cbd2.ByteWidth = sizeof(cb2);
	cbd2.StructureByteStride = 0u;
	D3D11_SUBRESOURCE_DATA csd2 = {};
	csd2.pSysMem = &cb2;
	pDevice->CreateBuffer(&cbd2, &csd2, &pConstantBuffer2);

	// bind constant buffer to pixel shader
	pDeviceContext->PSSetConstantBuffers(0u, 1u, pConstantBuffer2.GetAddressOf());



	// create pixel shader
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	D3DReadFileToBlob(L"..\\x64\\Debug\\PixelShader.cso", &pBlob);
	pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader);

	// bind pixel shader
	pDeviceContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);


	// create vertex shader
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	D3DReadFileToBlob(L"..\\x64\\Debug\\VertexShader.cso", &pBlob);
	pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);

	// bind vertex shader
	pDeviceContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);

	Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;

	const D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};
	pDevice->CreateInputLayout(
		ied, (UINT)std::size(ied),
		pBlob->GetBufferPointer(),
		pBlob->GetBufferSize(),
		&pInputLayout
	);

	// bind vertex layout
	pDeviceContext->IASetInputLayout(pInputLayout.Get());

	// Set primitive topology to triangle list (groups of 3 vertices)
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);


	// configure viewport
	D3D11_VIEWPORT vp;
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

void Graphics::CreateViewport()
{
	// Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(viewport));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800; // <-Change this
	viewport.Height = 600; // <-Change this

	pDeviceContext->RSSetViewports(1, &viewport);
}

void Graphics::EndFrame()
{
	pSwapChain->Present(1u, 0u);
}
