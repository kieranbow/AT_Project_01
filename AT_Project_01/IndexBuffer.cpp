#include "IndexBuffer.h"

HRESULT IndexBuffer::CreateIndexBuffer(ID3D11Device * device, const std::vector<unsigned short>&indices)
{
	// Set buffer Description
	ZeroMemory(&indexBufferDesc, sizeof(indexBufferDesc));
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0u;
	indexBufferDesc.MiscFlags = 0u;
	indexBufferDesc.ByteWidth = static_cast<UINT>(indices.size() * sizeof(unsigned short));
	indexBufferDesc.StructureByteStride = sizeof(unsigned short);

	// Set buffer sub-resource description
	ZeroMemory(&indexBufferSRD, sizeof(indexBufferSRD));
	indexBufferSRD.pSysMem = indices.data();

	HRESULT hResult = device->CreateBuffer(&indexBufferDesc, &indexBufferSRD, &pIndexBuffer); // mem leak fixed here
	return hResult;
}

void IndexBuffer::BindBuffer(ID3D11DeviceContext* deviceContext, DXGI_FORMAT format, UINT offset)
{
	deviceContext->IASetIndexBuffer(pIndexBuffer.Get(), format, offset);
}

ID3D11Buffer* IndexBuffer::Get() const
{
	return pIndexBuffer.Get();
}
