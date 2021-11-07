#include "VertexBuffer.h"

HRESULT VertexBuffer::CreateVertexBuffer(ID3D11Device* device, const std::vector<Vertex>& vertices, UINT _offset)
{
	stride = sizeof(Vertex);
	offset = _offset;

	// Set buffer Description
	ZeroMemory(&vertexBufferDesc, sizeof(vertexBufferDesc));
	vertexBufferDesc.ByteWidth = static_cast<UINT>(sizeof(Vertex) * vertices.size());
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0u;
	vertexBufferDesc.MiscFlags = 0u;
	vertexBufferDesc.StructureByteStride = sizeof(Vertex);

	// Set buffer sub-resource description
	ZeroMemory(&vertexBufferSRD, sizeof(vertexBufferSRD));
	vertexBufferSRD.pSysMem = vertices.data();

	HRESULT hResult = device->CreateBuffer(&vertexBufferDesc, &vertexBufferSRD, &pVertexBuffer);
	return hResult;
}

void VertexBuffer::BindBuffer(UINT startSlot, UINT numBuffer, ID3D11DeviceContext* deviceContext)
{
	deviceContext->IASetVertexBuffers(startSlot, numBuffer, pVertexBuffer.GetAddressOf(), &stride, &offset);
}

ID3D11Buffer* VertexBuffer::Get() const
{
	return pVertexBuffer.Get();
}

ID3D11Buffer* const* VertexBuffer::GetAddressOf() const
{
	return pVertexBuffer.GetAddressOf();
}
