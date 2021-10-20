#pragma once
#include <d3d11.h>
#include <wrl\client.h>


struct BufferStruct
{
	UINT ByteWidth;
	D3D11_USAGE Usage;
	UINT BindFlags;
	UINT CPUAccessFlags;
	UINT MiscFlags;
	UINT StructureByteStride;
};

class Buffer
{
	public:
		Buffer() = default;
		Buffer(BufferStruct bufferDescStruct, const void* sysMemory)
		{
			// Initialize Buffer description
			ZeroMemory(&bufferDesc, sizeof(bufferDesc));
			bufferDesc.ByteWidth				= bufferDescStruct.ByteWidth;
			bufferDesc.Usage						= bufferDescStruct.Usage;
			bufferDesc.BindFlags					= bufferDescStruct.BindFlags;
			bufferDesc.CPUAccessFlags		= bufferDescStruct.CPUAccessFlags;
			bufferDesc.MiscFlags					= bufferDescStruct.MiscFlags;
			bufferDesc.StructureByteStride	= bufferDescStruct.StructureByteStride;

			// Initialize buffer data
			ZeroMemory(&bufferSubResData, sizeof(bufferSubResData));
			bufferSubResData.pSysMem = sysMemory;
		}
		~Buffer() = default;

		// Getters
		D3D11_BUFFER_DESC GetBufferDesc() const 
		{
			return bufferDesc;
		};
		D3D11_SUBRESOURCE_DATA GetBufferData() const 
		{
			return bufferSubResData;
		};

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> buffer; // Buffer
		D3D11_BUFFER_DESC bufferDesc; // Buffer Description
		D3D11_SUBRESOURCE_DATA bufferSubResData; // Buffer Data
};
