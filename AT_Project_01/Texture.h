#pragma once
#include <d3d11.h>
#include <wrl\client.h>
#include <string>
#include "DDSTextureLoader11.h"

class Graphics;

class Texture
{
	public:
		Texture(Graphics* pGfx);
		~Texture() = default;

		bool LoadAndCreateTexture(std::string str_file_path);
		bool LoadAndCreateCubeMap(std::string str_file_path[6]);
		bool CreateSampleState(UINT startSlot, UINT numSamples);
		void SetShaderResource(UINT startSlot, UINT numViews);

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
