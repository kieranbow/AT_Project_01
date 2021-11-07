#pragma once
#include "IndexBuffer.h"
#include "VertexBuffer.h"

class Graphics;

///<summary>Stores a shared pointer to both vertex and index buffer</summary>
class Mesh
{
	public:
		/// <summary>Constructor that creates both buffers using data from param </summary>
		/// <param name="pGfx"></param> <param name="rVertices"></param> <param name="rIndices"></param>
		Mesh(Graphics* pGfx, std::vector<Vertex>& rVertices, std::vector<unsigned short>& rIndices);
		~Mesh() = default;

		/// <summary>Copy Constructor to copy buffers to a new mesh</summary>
		/// <param name="mesh"></param>
		Mesh(const Mesh& mesh);

		/// <summary>Binds the buffers to the pipeline</summary>
		/// <param name="pGfx"></param>
		void Bind(Graphics* pGfx);

	private:
		std::shared_ptr<VertexBuffer> spVertexBuffer = std::make_shared<VertexBuffer>();
		std::shared_ptr<IndexBuffer> spIndexBuffer = std::make_shared<IndexBuffer>();
};
