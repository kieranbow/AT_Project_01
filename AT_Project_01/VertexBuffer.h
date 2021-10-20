#pragma once
#include "Buffer.h"
#include "Vertex.h"
#include <vector>

class VertexBuffer : public Buffer
{
	public:
		explicit VertexBuffer(BufferStruct bufferStruct, Vertex vertex);

		VertexBuffer(const VertexBuffer&) = delete;
		VertexBuffer& operator=(const VertexBuffer&) = delete;

		void  FillVertexBuffer(Vertex vertices);

	private:
		std::vector<Vertex>vertices;
};
