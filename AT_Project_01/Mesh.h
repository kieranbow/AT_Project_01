#pragma once
#include "IndexBuffer.h"
#include "VertexBuffer.h"

class Graphics;

class Mesh
{
	public:
		Mesh(Graphics* pGfx, std::vector<Vertex>& rVertices, std::vector<unsigned short>& rIndices);
		~Mesh() = default;

		Mesh(const Mesh& mesh);

		void Bind(Graphics* pGfx);

	private:
		std::shared_ptr<VertexBuffer> spVertexBuffer = std::make_shared<VertexBuffer>();
		std::shared_ptr<IndexBuffer> spIndexBuffer = std::make_shared<IndexBuffer>();
};
