#include "Graphics.h"

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

void Graphics::CreateVertexBuffer()
{
	Vertex vertices[] =
	{
		Vertex(0.0f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f), // Center Point
		Vertex(0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f), // Left Point
		Vertex(-0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f), // Right Point
	};

	// Buffer Description
	D3D11_BUFFER_DESC vertexBufferDesc;
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));

	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags - 0; // Stops CPU having access to this buffer
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.ByteWidth = sizeof(vertices);
	vertexBufferDesc.StructureByteStride = sizeof(Vertex);

	// Buffer Data
	D3D11_SUBRESOURCE_DATA vertexBufferData;
	ZeroMemory(&vertexBufferData, sizeof(vertexBufferData));
	vertexBufferData.pSysMem = vertices;

	// Create Buffer
	pDevice->CreateBuffer(&vertexBufferDesc, &vertexBufferData, &pVertexBuffer);

	// Bind Buffer
	UINT stride = sizeof(Vertex);
	UINT offset = 0u;
	pDeviceContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);
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

	D3DReadFileToBlob(L"..\\x64\\Debug\\VertexShader.cso", &pVertexShaderBuffer); //Error log this
	pDevice->CreateVertexShader(pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(), NULL, &pVertexShader); // Error log this;
	pDeviceContext->VSSetShader(pVertexShader.Get(), NULL, 0);
}

void Graphics::InitializeInputLayout()
{
	// Layout Description
	// Vertex Shader will have access to propertices like POSITION and COLOR
	D3D11_INPUT_ELEMENT_DESC layoutDesc[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	UINT numElements = std::size(layoutDesc);

	// Error Check this
	pDevice->CreateInputLayout(layoutDesc, numElements, pVertexShaderBuffer->GetBufferPointer(), pVertexShaderBuffer->GetBufferSize(), &pInputLayout);

	// Set Input Layout
	pDeviceContext->IASetInputLayout(pInputLayout.Get());

}

void Graphics::PixelShader()
{
	D3DReadFileToBlob(L"..\\x64\\Debug\\PixelShader.cso", &pPixelShaderBuffer); //Error log this
	pDevice->CreatePixelShader(pPixelShaderBuffer->GetBufferPointer(), pPixelShaderBuffer->GetBufferSize(), NULL, &pPixelShader);
	pDeviceContext->PSSetShader(pPixelShader.Get(), NULL, 0);
}

void Graphics::drawTriangle()
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

	struct Vertex
	{
		struct
		{
			float x;
			float y;
			float z;
		} pos;

		struct
		{
			float r;
			float g;
			float b;
		} color;
	};

	// create vertex buffer (1 2d triangle at center of screen)
	Vertex vertices[] =
	{
		{ -1.0f,-1.0f,-1.0f,	 1.0f, 0, 0},
		{ 1.0f,-1.0f,-1.0f,	 0, 1.0f, 0},
		{ -1.0f,1.0f,-1.0f,	 0, 0, 1.0f},
		{ 1.0f,1.0f,-1.0f,		 1.0f, 0, 0},
		{ -1.0f,-1.0f,1.0f,	 0, 1.0f, 0},
		{ 1.0f,-1.0f,1.0f,		 0, 0, 1.0f},
		{ -1.0f,1.0f,1.0f,		 1.0f, 0, 0},
		{ 1.0f,1.0f,1.0f,	     0, 1.0f, 0},
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	pDevice->CreateBuffer(&bd, &sd, &pVertexBuffer);

	// Bind vertex buffer to pipeline
	const UINT stride = sizeof(Vertex);
	const UINT offset = 0u;
	pDeviceContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &offset);


	// create index buffer
	const unsigned short indices[] =
	{
		0,2,1, 2,3,1,
		1,3,5, 3,7,5,
		2,6,3, 3,6,7,
		4,5,7, 4,7,6,
		0,4,2, 2,4,6,
		0,1,4, 1,5,4
	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;
	D3D11_BUFFER_DESC ibd = {};
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.Usage = D3D11_USAGE_DEFAULT;
	ibd.CPUAccessFlags = 0u;
	ibd.MiscFlags = 0u;
	ibd.ByteWidth = sizeof(indices);
	ibd.StructureByteStride = sizeof(unsigned short);
	D3D11_SUBRESOURCE_DATA isd = {};
	isd.pSysMem = indices;
	pDevice->CreateBuffer(&ibd, &isd, &pIndexBuffer);

	// bind index buffer
	pDeviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);


	// create constant buffer for transformation matrix
	struct ConstantBuffer
	{
		DirectX::XMMATRIX transform;
	};
	const ConstantBuffer cb =
	{
		{
			DirectX::XMMatrixTranspose(
				DirectX::XMMatrixRotationZ(5) *
				DirectX::XMMatrixRotationX(20) *
				DirectX::XMMatrixTranslation(xPos, yPos, 4.0f) *
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


	// input (vertex) layout (2d position only)
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


	// bind render target
	pDeviceContext->OMSetRenderTargets(1u, pRenderTargetView.GetAddressOf(), nullptr);


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


	pDeviceContext->DrawIndexed((UINT)std::size(indices), 0u, 0u);
}

void Graphics::drawTestTriangle()
{
	CreateVertexBuffer();
	VertexShader();
	InitializeInputLayout();
	PixelShader();

	// Output Merger
	pDeviceContext->OMSetRenderTargets(1u, pRenderTargetView.GetAddressOf(), NULL);

	// Set order of topology rendering
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	CreateViewport();

	// Draw Vertices
	pDeviceContext->Draw(3, 0u);


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
