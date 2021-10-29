#pragma once
// DirectX
#include <d3d11.h>

// Microsoft
#include <wrl/client.h>

// C++
#include <memory>
#include <vector>

// Description
// Constant Buffer class that handles behaviour of a constant buffer
// like Creation, Update, Bind.
class ConstantBuffer
{
	public:
		ConstantBuffer() = default;
		~ConstantBuffer() = default;

		ConstantBuffer(const ConstantBuffer&) = delete;
		ConstantBuffer& operator=(const ConstantBuffer&) = delete;

		// Description
		// Creates a constant buffer using BUFFER_DESC and SUBRESOURCE_DATA
		template <typename type>
		HRESULT CreateConstantBuffer(ID3D11Device* device, type& constBufferType)
		{
			// Buffer Description
			ZeroMemory(&constantBufferDesc, sizeof(constantBufferDesc));
			constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constantBufferDesc.ByteWidth = sizeof(constBufferType);
			constantBufferDesc.MiscFlags = 0u;
			constantBufferDesc.StructureByteStride = 0u;

			// Buffer Sub-Resource Data
			constantBufferSRD.pSysMem = &constBufferType;

			// Create Constant Buffer
			HRESULT hResult = device->CreateBuffer(&constantBufferDesc, &constantBufferSRD, &pConstantBuffer);
			return hResult;
		}

		// Description
		// Updates buffer using map
		void UpdateBuffer()
		{
			
		}

		// Description
		// Bind Constant Buffer to Vertex Shader
		void SetVSConstBuffer(ID3D11DeviceContext* deviceContext, UINT startSlot, UINT numBuffer)
		{
			deviceContext->VSSetConstantBuffers(startSlot, numBuffer, pConstantBuffer.GetAddressOf());
		}

		// Description
		// Bind Constant Buffer to Pixel Shader
		void SetPSConstBuffer(ID3D11DeviceContext* deviceContext, UINT startSlot, UINT numBuffer)
		{
			deviceContext->PSSetConstantBuffers(startSlot, numBuffer, pConstantBuffer.GetAddressOf());
		}

		ID3D11Buffer* GetConstantBuffer()
		{
			return pConstantBuffer.Get();
		}

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;	// Buffer
		D3D11_BUFFER_DESC constantBufferDesc = {};					// Buffer Description
		D3D11_SUBRESOURCE_DATA constantBufferSRD = {};			// Buffer Data
};
