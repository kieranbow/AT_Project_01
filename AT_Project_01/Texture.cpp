#include "Texture.h"
#include "ErrorChecker.h"
#include "Graphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(Graphics* pGfx)
{
	l_gfx = pGfx;
}

TextureData Texture::LoadTextureData(std::string str_file_path)
{
	// Create textureData.
	TextureData textureData;

	// Load and store pixel values into textureData.
	textureData.pixels = stbi_load(str_file_path.c_str(), &textureData.width, &textureData.height, &textureData.num_channels, STBI_rgb_alpha);

	// If the texture failed to read, return a default TextureData.
	if (stbi_failure_reason())
	{
		Logging::LogError("stbi_load failed to find texture");
		return TextureData();
	}

	return textureData;
}

bool Texture::CreateTextureFromTextureData(TextureData& texturedata, DXGI_FORMAT format)
{
	D3D11_TEXTURE2D_DESC tex_desc = {};
	ZeroMemory(&tex_desc, sizeof(tex_desc));
	tex_desc.Width = texturedata.width;
	tex_desc.Height = texturedata.height;
	tex_desc.MipLevels = 1u;
	tex_desc.ArraySize = 1u;
	tex_desc.Format = format; // DXGI_FORMAT_B8G8R8A8_UNORM_SRGB, DXGI_FORMAT_R8G8B8A8_UNORM,
	tex_desc.SampleDesc.Count = 1u;
	tex_desc.SampleDesc.Quality = 0u;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tex_desc.CPUAccessFlags = 0u;
	tex_desc.MiscFlags = 0u;

	// Pass textureData to tex_data
	D3D11_SUBRESOURCE_DATA tex_data = {};
	tex_data.pSysMem = texturedata.pixels;
	tex_data.SysMemPitch = texturedata.width * texturedata.num_channels; // img width * number of channels

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
	srvd.Format = tex_desc.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MostDetailedMip = 0;
	srvd.Texture2D.MipLevels = -1;

	// Creates texture 2D
	hr = l_gfx->GetDevice()->CreateTexture2D(&tex_desc, &tex_data, &pTexture);
	if (FAILED(hr))
	{
		Logging::ThrowIf(hr, "Failed to Create Texture2D");
		return false;
	}

	// Create Shader Resource View
	hr = l_gfx->GetDevice()->CreateShaderResourceView(pTexture.Get(), &srvd, &pShaderResourceView);
	if (FAILED(hr))
	{
		Logging::ThrowIf(hr, "Failed to Create Shader Resource View");
		return false;
	}
	return true;
}

bool Texture::LoadAndCreateTexture(std::string str_file_path, DXGI_FORMAT format)
{
	const int number_channels = 4;

	// Load raw pixel data using stbi_load
	stbi_uc* texture = stbi_load(str_file_path.c_str(), &img_width, &img_height, &num_channel, STBI_rgb_alpha);

	// Checks if the image failed to load/Read
	if (stbi_failure_reason())
	{
		Logging::LogError("stbi_load failed to find texture");
		return false;
	}

	D3D11_TEXTURE2D_DESC tex_desc = {};
	ZeroMemory(&tex_desc, sizeof(tex_desc));
	tex_desc.Width = img_width;
	tex_desc.Height = img_height;
	tex_desc.MipLevels = 1u;
	tex_desc.ArraySize = 1u;
	tex_desc.Format = format; // DXGI_FORMAT_B8G8R8A8_UNORM_SRGB, DXGI_FORMAT_R8G8B8A8_UNORM,
	tex_desc.SampleDesc.Count = 1u;
	tex_desc.SampleDesc.Quality = 0u;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tex_desc.CPUAccessFlags = 0u;
	tex_desc.MiscFlags = 0u;

	D3D11_SUBRESOURCE_DATA tex_data = {};
	tex_data.pSysMem = texture;
	tex_data.SysMemPitch = img_width * number_channels;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
	srvd.Format = tex_desc.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MostDetailedMip = 0;
	srvd.Texture2D.MipLevels = -1;

	// Creates texture 2D
	hr = l_gfx->GetDevice()->CreateTexture2D(&tex_desc, &tex_data, &pTexture);
	if (FAILED(hr))
	{
		Logging::ThrowIf(hr, "Failed to Create Texture2D");
		return false;
	}

	// Create Shader Resource View
	hr = l_gfx->GetDevice()->CreateShaderResourceView(pTexture.Get(), &srvd, &pShaderResourceView);
	if (FAILED(hr))
	{
		Logging::ThrowIf(hr, "Failed to Create Shader Resource View");
		return false;
	}

	// Once image is loaded and create, free texture data from stbi
	stbi_image_free(texture);
	return true;
}

bool Texture::LoadAndCreateCubeMap(std::string str_file_path[6], DXGI_FORMAT format)
{
	const int number_channels = 4;
	D3D11_SUBRESOURCE_DATA pData[6];
	stbi_uc* img;

	// Loop through and load all 6 cube textures and assign them to pData
	for (int i = 0; i < 6; i++)
	{
		img = stbi_load(str_file_path[i].c_str(), &img_width, &img_height, &num_channel, 0);

		if (stbi_failure_reason())
		{
			Logging::LogError("stbi_load failed to find texture");
			return false;
		}

		pData[i].pSysMem = img;
		pData[i].SysMemPitch = img_width * number_channels;
		pData[i].SysMemSlicePitch = 0;
	}

	D3D11_TEXTURE2D_DESC cubeDesc{};
	cubeDesc.Width = img_width;
	cubeDesc.Height = img_height;
	cubeDesc.MipLevels = 1u;
	cubeDesc.ArraySize = 6u;
	cubeDesc.Format = format;
	cubeDesc.CPUAccessFlags = 0;
	cubeDesc.SampleDesc.Count = 1;
	cubeDesc.SampleDesc.Quality = 0;
	cubeDesc.Usage = D3D11_USAGE_DEFAULT;
	cubeDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	cubeDesc.CPUAccessFlags = 0;
	cubeDesc.MiscFlags = D3D11_RESOURCE_MISC_TEXTURECUBE;

	D3D11_SHADER_RESOURCE_VIEW_DESC SMViewDesc{};
	SMViewDesc.Format = cubeDesc.Format;
	SMViewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURECUBE;
	SMViewDesc.TextureCube.MipLevels = -1;
	SMViewDesc.TextureCube.MostDetailedMip = 0;

	// Create Texture2D
	hr = l_gfx->GetDevice()->CreateTexture2D(&cubeDesc, &pData[0], pTexture.GetAddressOf());
	if (FAILED(hr))
	{
		Logging::ThrowIf(hr, "Failed to Create Texture2D cubemap");
		return false;
	}

	// Create Shader Resource View
	hr = l_gfx->GetDevice()->CreateShaderResourceView(pTexture.Get(), &SMViewDesc, &pShaderResourceView);
	if (FAILED(hr))
	{
		Logging::ThrowIf(hr, "Failed to Create Shader Resource View for cubemap");
		return false;
	}

	// Free loaded texture memory
	stbi_image_free(img);

	return true;
}

bool Texture::CreateSampleState(UINT startSlot, UINT numSamples)
{
	D3D11_SAMPLER_DESC sampleDesc = {};
	sampleDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampleDesc.MinLOD = 1u;
	sampleDesc.MaxLOD = D3D11_FLOAT32_MAX;
	sampleDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_POINT; // D3D11_FILTER_MIN_MAG_MIP_LINEAR
	sampleDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;

	hr = l_gfx->GetDevice()->CreateSamplerState(&sampleDesc, pSampleState.GetAddressOf());
	if (FAILED(hr))
	{
		Logging::ThrowIf(hr, "Texture failed to create Sampler state");
		return false;
	}

	l_gfx->GetDeviceContext()->PSSetSamplers(startSlot, numSamples, pSampleState.GetAddressOf());
	return true;
}

void Texture::SetShaderResource(UINT startSlot, UINT numViews)
{
	l_gfx->GetDeviceContext()->PSSetShaderResources(startSlot, numViews, pShaderResourceView.GetAddressOf());
}

ID3D11Texture2D* Texture::GetTexture() const
{
	return pTexture.Get();
}
