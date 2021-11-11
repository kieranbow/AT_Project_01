#pragma once
#include "IndexBuffer.h"
#include "VertexBuffer.h"

class Graphics;

// Stores a shared pointer to both vertex and index buffer
class Mesh
{
	public:
		// Constructor that creates both buffers using data from param
		Mesh(Graphics* pGfx, std::vector<Vertex>& rVertices, std::vector<unsigned short>& rIndices);
		~Mesh() = default;

		// Copy Constructor to copy buffers to a new mesh
		Mesh(const Mesh& mesh);

		// Binds the buffers to the pipeline
		void Bind(Graphics* pGfx);

	private:
		std::shared_ptr<VertexBuffer> spVertexBuffer = std::make_shared<VertexBuffer>();
		std::shared_ptr<IndexBuffer> spIndexBuffer = std::make_shared<IndexBuffer>();
};
