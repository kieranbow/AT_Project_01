#pragma once
// DirectX
#include <d3d11.h>
#include <wrl/client.h>

// C++
#include <memory>
#include <vector>

#include "Vertex.h"

class IndexBuffer
{
	public:
		IndexBuffer() = default;

		HRESULT CreateIndexBuffer(ID3D11Device* device, const std::vector<unsigned short>& indices)
		{
			// Set buffer Description
			ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
			indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
			indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			indexBufferDesc.CPUAccessFlags = 0u;
			indexBufferDesc.MiscFlags = 0u;
			indexBufferDesc.ByteWidth = sizeof(indices);
			indexBufferDesc.StructureByteStride = sizeof(unsigned short);
			
			// Set buffer sub-resource description
			ZeroMemory(&indexBufferSRD, sizeof(indexBufferSRD));
			indexBufferSRD.pSysMem = indices.data();

			HRESULT hResult = device->CreateBuffer(&indexBufferDesc, &indexBufferSRD, pIndexBuffer.GetAddressOf());
			return hResult;
		}

		void BindBuffer(ID3D11DeviceContext* deviceContext)
		{
			deviceContext->IASetIndexBuffer(pIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0u);
		}

		// Getter
		ID3D11Buffer* Get() const 
		{
			return pIndexBuffer.Get();
		}

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;		// Buffer
		D3D11_BUFFER_DESC indexBufferDesc = {};					// Buffer Description
		D3D11_SUBRESOURCE_DATA indexBufferSRD = {};			// Buffer Data

};