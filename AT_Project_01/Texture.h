#pragma once
#include <d3d11.h>
#include <wrl\client.h>
#include <string>

class Graphics;

// A struct that contains the propertices for a texture like width, height,
// pixels and number of color channels.
struct TextureData
{
	unsigned char* pixels;
	int width;
	int height;
	int num_channels;
};

// The Texture class handles all loading of textures and cubemaps. It is also reponsible for creating sampler States for the pixel shader 
// and Shader Resource View.
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

		// ComPtr for ID3D11
		Microsoft::WRL::ComPtr<ID3D11SamplerState> pSampleState;
		Microsoft::WRL::ComPtr<ID3D11Texture2D> pTexture;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> pShaderResourceView;

		// Image propertices
		int img_width = 0;
		int img_height = 0;
		int num_channel = 0;

		// Local pointer to Graphics class
		Graphics* l_gfx;
};
