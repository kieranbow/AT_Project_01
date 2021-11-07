#pragma once
// DirectX
#include <d3d11.h>
#include <wrl/client.h>

// C++
#include <memory>
#include <vector>

#include "Vertex.h"

// A Vertex buffer class that handles the construction and binding of the Vertex Buffer
class VertexBuffer
{
	public:
		VertexBuffer() = default;

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		// Creates a constant buffer using BUFFER_DESC and SUBRESOURCE_DATA
		HRESULT CreateVertexBuffer(ID3D11Device* device, const std::vector<Vertex>& vertices, UINT _offset);

		// Binds the buffer to the pipeline using IASetVertexBuffer
		void BindBuffer(UINT startSlot, UINT numBuffer, ID3D11DeviceContext* deviceContext);

		ID3D11Buffer* Get() const;
		ID3D11Buffer* const* GetAddressOf() const;

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;	// Buffer
		D3D11_BUFFER_DESC vertexBufferDesc = {};			// Buffer Description
		D3D11_SUBRESOURCE_DATA vertexBufferSRD = {};		// Buffer Data

		UINT stride = 0u;
		UINT offset = 0u;
};
