#pragma once
// DirectX
#include <d3d11.h>
#include <wrl/client.h>

// C++
#include <memory>
#include <vector>

#include "Vertex.h"

// A IndexBuffer class that handles the construction and binding of the Index Buffer
class IndexBuffer
{
	public:
		IndexBuffer() = default;
		~IndexBuffer() = default;

		IndexBuffer(const IndexBuffer&) = delete;
		IndexBuffer& operator=(const IndexBuffer&) = delete;

		// Creates a constant buffer using BUFFER_DESC and SUBRESOURCE_DATA
		HRESULT CreateIndexBuffer(ID3D11Device* device, const std::vector<unsigned short>& indices);

		// Binds the buffer to the pipeline using IASetIndexBuffer
		void BindBuffer(ID3D11DeviceContext* deviceContext, DXGI_FORMAT format, UINT offset);

		ID3D11Buffer* Get() const;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> pIndexBuffer;	// Buffer
		D3D11_BUFFER_DESC indexBufferDesc = {};				// Buffer Description
		D3D11_SUBRESOURCE_DATA indexBufferSRD = {};			// Buffer Data
};
