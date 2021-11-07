#pragma once
// DirectX
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

// Microsoft
#include <wrl/client.h>

// Description
// Vertex Shader class which handles reading vertex shader files, creating vertex shader 
// and bind them to the pipeline.
class VSShader
{
	public:
		VSShader() = default;
		~VSShader() = default;

		VSShader(const VSShader&) = delete;
		VSShader& operator=(const VSShader&) = delete;

		// Description
		// Reads shader file and copies data to shader blob.
		HRESULT ReadVSShaderToBlob(LPCWSTR filePath)
		{
			return D3DReadFileToBlob(filePath, &pVertexShaderBlob);
		}

		// Description
		// Creates the shader using DirectX CreateVertexShader 
		HRESULT CreateVSShader(ID3D11Device* pDevice, D3D11_INPUT_ELEMENT_DESC* pLayoutDesc, UINT numElements)
		{
			pDevice->CreateVertexShader(
				pVertexShaderBlob->GetBufferPointer(), 
				pVertexShaderBlob->GetBufferSize(), 
				NULL, 
				&pVertexShader);

			return pDevice->CreateInputLayout(
				pLayoutDesc,
				numElements,
				GetVSBlob()->GetBufferPointer(),
				GetVSBlob()->GetBufferSize(),
				&pInputLayout);
		}

		// Description
		// Binds Vertex Shader to pipeline
		void SetVSShader(ID3D11DeviceContext* pDeviceContext, UINT NumInstance)
		{
			pDeviceContext->VSSetShader(pVertexShader.Get(), NULL, NumInstance);
		}

		ID3D11VertexShader* GetVSShader()const
		{
			return pVertexShader.Get();
		}

		ID3D10Blob* GetVSBlob() const
		{
			return pVertexShaderBlob.Get();
		}

		ID3D11InputLayout* GetInputLayout() const
		{
			return pInputLayout.Get();
		}

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
		Microsoft::WRL::ComPtr<ID3D10Blob> pVertexShaderBlob;
		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
};
