#pragma once
#include <d3d11.h>
#include <wrl\client.h>
#include <string>
#include "DDSTextureLoader11.h"

class Graphics;

struct TextureData
{
	unsigned char* pixels;
	int width;
	int height;
	int num_channels;
};

class Texture
{
	public:
		Texture(Graphics* pGfx);
		~Texture() = default;

		// Load the nessessary data like pixels and image size into a TextureData struct
		TextureData LoadTextureData(std::string str_file_path);

		// Create a texture and shader resource using TextureData struct
		bool CreateTextureFromTextureData(TextureData& texturedata, DXGI_FORMAT format);

		// Create and load a texture and shader resource from a filepath
		bool LoadAndCreateTexture(std::string str_file_path, DXGI_FORMAT format);

		// Create and load a cubemap and shader resource from a filepath
		bool LoadAndCreateCubeMap(std::string str_file_path[6], DXGI_FORMAT format);
		
		// Create Sample state for pixel shader
		bool CreateSampleState(UINT startSlot, UINT numSamples);

		// Bound the shader resource to the graphics pipeline for rendering 
		void SetShaderResource(UINT startSlot, UINT numViews);

		// Get a pointer to ID3D11Texture2D
		ID3D11Texture2D* GetTexture() const;

	private:
		HRESULT hr;

		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampleState;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView;

		D3D11_TEXTURE2D_DESC tex_desc = {};

		int width = 0;
		int height = 0;
		int num_channel = 0;

		Graphics* l_gfx;
};
