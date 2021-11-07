#include "Mesh.h"

#include "Graphics.h"
#include "Model_loader.h"
#include "ErrorChecker.h"


Mesh::Mesh(Graphics* pGfx, std::vector<Vertex>& rVertices, std::vector<unsigned short>& rIndices)
{
	spVertexBuffer->CreateVertexBuffer(pGfx->GetDevice(), rVertices, 0u);
	spIndexBuffer->CreateIndexBuffer(pGfx->GetDevice(), rIndices);
}

Mesh::Mesh(const Mesh& mesh)
{
	spVertexBuffer = mesh.spVertexBuffer;
	spIndexBuffer = mesh.spIndexBuffer;
}

void Mesh::Bind(Graphics* pGfx)
{
	spVertexBuffer->BindBuffer(0u, 1u, pGfx->GetDeviceContext());
	spIndexBuffer->BindBuffer(pGfx->GetDeviceContext(), DXGI_FORMAT_R16_UINT, 0u);
}
