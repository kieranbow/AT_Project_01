#pragma once
// DirectX
#include <d3d11.h>
#include <d3dcompiler.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

#include <wrl/client.h>

class VSShader
{
	public:
		VSShader() = default;
		~VSShader() = default;

		VSShader(const VSShader&) = delete;
		VSShader& operator=(const VSShader&) = delete;

		HRESULT ReadVSShaderToBlob(LPCWSTR filePath)
		{
			return D3DReadFileToBlob(filePath, &pVertexShaderBlob);
		}

		HRESULT CreateVSShader(ID3D11Device* device)
		{
			return device->CreateVertexShader(
				pVertexShaderBlob->GetBufferPointer(), 
				pVertexShaderBlob->GetBufferSize(), 
				NULL, 
				&pVertexShader);
		}

		void SetVSShader(ID3D11DeviceContext* deviceContext, UINT NumInstance)
		{
			deviceContext->VSSetShader(pVertexShader.Get(), NULL, NumInstance);
		}

		ID3D11VertexShader* GetVSShader()const
		{
			return pVertexShader.Get();
		}

		ID3D10Blob* GetVSBlob() const
		{
			return pVertexShaderBlob.Get();
		}

	private:
		Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
		Microsoft::WRL::ComPtr<ID3D10Blob> pVertexShaderBlob;
};