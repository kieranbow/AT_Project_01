#pragma once
// DirectX
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

// Microsoft
#include <wrl/client.h>

#include <string>

// Description
// Pixel Shader class which handles reading pixel shader files, creating pixel shader 
// and bind them to the pipeline.
class PSShader
{
public:
	PSShader() = default;
	~PSShader() = default;

	PSShader(const PSShader&) = delete;
	PSShader& operator=(const PSShader&) = delete;

	// Description
	// Reads shader file and copies data to shader blob.
	HRESULT ReadPSShaderToBlob(std::wstring filePath)
	{
		return D3DReadFileToBlob(filePath.c_str(), &pPixelShaderBlob);
	}

	// Description
	// Creates the shader using DirectX CreatePixelShader 
	HRESULT CreatePSShader(ID3D11Device* device)
	{
		return device->CreatePixelShader(
			pPixelShaderBlob->GetBufferPointer(),
			pPixelShaderBlob->GetBufferSize(),
			NULL,
			&pPixelShader);
	}

	// Description
	// Binds Pixel Shader to pipeline
	void SetPSShader(ID3D11DeviceContext* deviceContext, UINT NumInstance)
	{
		deviceContext->PSSetShader(pPixelShader.Get(), NULL, NumInstance);
	}

	ID3D11PixelShader* GetPSShader() const
	{
		return pPixelShader.Get();
	}

	ID3D10Blob* GetPSBlob() const
	{
		return pPixelShaderBlob.Get();
	}

private:
	Microsoft::WRL::ComPtr<ID3D11PixelShader> pPixelShader;
	Microsoft::WRL::ComPtr<ID3D10Blob> pPixelShaderBlob;
};
