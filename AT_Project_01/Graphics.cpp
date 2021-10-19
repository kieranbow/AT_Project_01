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

	InitLayout();

	// Back Buffer
	Microsoft::WRL::ComPtr<ID3D11Resource> pBackBuffer;
	pSwapChain->GetBuffer(0, __uuidof(ID3D11Resource), &pBackBuffer); // Add error checker
	pDevice->CreateRenderTargetView(pBackBuffer.Get(), NULL, &pRenderTargetView);

	// Output Merger
	pDeviceContext->OMSetRenderTargets(1, pRenderTargetView.GetAddressOf(), NULL);

	// Viewport
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(viewport));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = 800; // <-Change this
	viewport.Height = 600; // <-Change this

	pDeviceContext->RSSetViewports(1, &viewport);

}

void Graphics::ClearBuffer(float red, float green, float blue)
{
	const float color[] = { red, green, blue, 1.0f };

	// Clear Render Target
	pDeviceContext->ClearRenderTargetView(pRenderTargetView.Get(), color);

	// Set Input Layout
	pDeviceContext->IASetInputLayout(pInputLayout.Get());

	// Set order of topology rendering
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// Set Shaders
	pDeviceContext->VSSetShader(pVertexShader.Get(), NULL, 0);
	pDeviceContext->PSSetShader(pPixelShader.Get(), NULL, 0);

	// Set Vertex Buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, pVertexBuffer.GetAddressOf(), &stride, &offset);

	// Draw Vertices
	pDeviceContext->Draw(4, 0);


	EndFrame();

}

void Graphics::CreateVertexBuffer()
{
	Vertex vertex[] =
	{
		Vertex(0.0f, 0.5f), // Center Point
		Vertex(-0.5f, 0.0f), // Left Point
		Vertex(0.5f, 0.0f), // Right Point
		Vertex(0.0f, 0.5f)	// Top Point
	};

	// Buffer Description
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(Vertex) * ARRAYSIZE(vertex);
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags - 0; // Stops CPU having access to this buffer
	vertexBufferDesc.MiscFlags = 0;

	// Buffer Data
	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = vertex;

	// Create Buffer
	pDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, pVertexBuffer.GetAddressOf());

	// Bind Buffer


}

void Graphics::VertexShader()
{
	LPCWSTR shaderFolder = L"";

#pragma region DetermineShaderPath
	if (IsDebuggerPresent() == TRUE)
	{
		#ifdef _DEBUG
			#ifdef _WIN64
				shaderFolder = L"..\\x64\\Debug\\";
			#else
				shaderFolder = L"..\\Debug\\";
			#endif // _WIN64
		#else
			#ifdef _WIN64
				shaderFolder = L"..\\x64\\Release\\";
			#else
				shaderFolder = L"..\\Release\\";
			#endif // _WIN64
		#endif // _DEBUG

	}

	D3DReadFileToBlob(L"..\\x64\\Debug\\VertexShader.cso", pVertexShaderBuffer.GetAddressOf()); //Error log this
	pDevice->CreateVertexShader(pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(), NULL, pVertexShader.GetAddressOf()); // Error log this;

}

void Graphics::InitLayout()
{
	// Input assembler
	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	UINT numElements = ARRAYSIZE(layoutDesc);

	CreateVertexBuffer();
	VertexShader();

	PixelShader();

	// Error Check this
	pDevice->CreateInputLayout(layoutDesc, numElements, pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(), pInputLayout.GetAddressOf());
}

void Graphics::PixelShader()
{
	D3DReadFileToBlob(L"..\\x64\\Debug\\PixelShader.cso", pPixelShaderBuffer.GetAddressOf()); //Error log this
	pDevice->CreatePixelShader(pPixelShaderBuffer->GetBufferPointer(), pPixelShaderBuffer->GetBufferSize(), NULL, pPixelShader.GetAddressOf());
}

void Graphics::EndFrame()
{
	pSwapChain->Present(1u, 0u);
}
