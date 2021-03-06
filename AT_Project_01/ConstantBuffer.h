#pragma once
// DirectX
#include <d3d11.h>

// Microsoft
#include <wrl/client.h>

// C++
#include <memory>
#include <vector>

struct Bind
{
	struct Buffer
	{
		const static UINT b0 = 0u;
		const static UINT b1 = 1u;
		const static UINT b2 = 2u;
		const static UINT b3 = 3u;
		const static UINT b4 = 4u;
		const static UINT b5 = 5u;
	};
	struct Texture
	{
		const static UINT t0 = 0u;
		const static UINT t1 = 1u;
		const static UINT t2 = 2u;
		const static UINT t3 = 3u;
		const static UINT t4 = 4u;
		const static UINT t5 = 5u;
	};
	struct Sampler
	{
		const static UINT s0 = 0u;
		const static UINT s1 = 1u;
		const static UINT s2 = 2u;
		const static UINT s3 = 3u;
		const static UINT s4 = 4u;
		const static UINT s5 = 5u;
	};
};

// Constant Buffer class that handles behaviour of a constant buffer
// like Creation, Update, Bind.
template <typename type>
class ConstantBuffer
{
	public:
		ConstantBuffer() = default;
		~ConstantBuffer() = default;

		ConstantBuffer(const ConstantBuffer&) = delete;
		ConstantBuffer& operator=(const ConstantBuffer&) = delete;

		// Creates a constant buffer using BUFFER_DESC and SUBRESOURCE_DATA
		// Usage flags: D3D11_USAGE_DYNAMIC
		// CPUAccessFlags: D3D11_CPU_ACCESS_WRITE
		HRESULT CreateDynamicConstantBuffer(ID3D11Device* device)
		{
			// Buffer Description
			ZeroMemory(&constantBufferDesc, sizeof(constantBufferDesc));
			constantBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
			constantBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constantBufferDesc.ByteWidth = sizeof(type);
			constantBufferDesc.MiscFlags = 0u;
			constantBufferDesc.StructureByteStride = 0u;

			// Create Constant Buffer
			HRESULT hResult = device->CreateBuffer(&constantBufferDesc, NULL, &pConstantBuffer);
			return hResult;
		}

		// Creates a constant buffer using BUFFER_DESC and SUBRESOURCE_DATA
		// Usage flags: D3D11_USAGE_DEFAULT
		// CPUAccessFlags: 0u
		HRESULT CreateStaticConstantBuffer(ID3D11Device* device)
		{
			// Buffer Description
			ZeroMemory(&constantBufferDesc, sizeof(constantBufferDesc));
			constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
			constantBufferDesc.CPUAccessFlags = 0u;
			constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
			constantBufferDesc.ByteWidth = sizeof(type);
			constantBufferDesc.MiscFlags = 0u;
			constantBufferDesc.StructureByteStride = 0u;

			// Create Constant Buffer
			HRESULT hResult = device->CreateBuffer(&constantBufferDesc, NULL, &pConstantBuffer);
			return hResult;
		}

		// Updates buffer using map
		HRESULT UpdateBuffer(ID3D11DeviceContext* deviceContext)
		{
			HRESULT hr;
			D3D11_MAPPED_SUBRESOURCE mappedResource;

			hr = deviceContext->Map(pConstantBuffer.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
			CopyMemory(mappedResource.pData, &data, sizeof(type));
			deviceContext->Unmap(pConstantBuffer.Get(), 0);
			return hr;
		}

		void UpdateSubResource(ID3D11DeviceContext* deviceContext)
		{
			deviceContext->UpdateSubresource(pConstantBuffer.Get(), 0, NULL, &data, 0, 0);
		}

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

		type data;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> pConstantBuffer;	// Buffer
		D3D11_BUFFER_DESC constantBufferDesc = {};				// Buffer Description
};
