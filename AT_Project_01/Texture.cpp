#include "Texture.h"
#include "ErrorChecker.h"
#include "Graphics.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(Graphics* pGfx)
{
	l_gfx = pGfx;
}

bool Texture::LoadAndCreateTexture(std::string str_file_path)
{
	// Load raw pixel data using stbi_load
	stbi_uc* texture = stbi_load(str_file_path.c_str(), &width, &height, &num_channel, STBI_rgb_alpha);

	if (stbi_failure_reason())
	{
		Logging::LogError("stbi_load failed to find texture");
		return false;
	}

	ZeroMemory(&tex_desc, sizeof(tex_desc));
	tex_desc.Width = width;
	tex_desc.Height = height;
	tex_desc.MipLevels = 1u;
	tex_desc.ArraySize = 1u;
	tex_desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	tex_desc.SampleDesc.Count = 1u;
	tex_desc.SampleDesc.Quality = 0u;
	tex_desc.Usage = D3D11_USAGE_DEFAULT;
	tex_desc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	tex_desc.CPUAccessFlags = 0u;
	tex_desc.MiscFlags = 0u;

	D3D11_SUBRESOURCE_DATA tex_data = {};
	tex_data.pSysMem = texture;
	tex_data.SysMemPitch = width * 4; // img width * number of channels

	D3D11_SHADER_RESOURCE_VIEW_DESC srvd = {};
	srvd.Format = tex_desc.Format;
	srvd.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvd.Texture2D.MostDetailedMip = 0;
	srvd.Texture2D.MipLevels = -1;

	hr = l_gfx->GetDevice()->CreateTexture2D(&tex_desc, &tex_data, &pTexture);
	if (FAILED(hr))
	{
		Logging::ThrowIf(hr, "Failed to Create Texture2D");
		return false;
	}

	hr = l_gfx->GetDevice()->CreateShaderResourceView(pTexture.Get(), &srvd, &pShaderResourceView);
	if (FAILED(hr))
	{
		Logging::ThrowIf(hr, "Failed to Create Shader Resource View");
		return false;
	}
	stbi_image_free(texture);
	return true;
}

bool Texture::LoadAndCreateCubeMap(std::string str_file_path[6])
{
	D3D11_SUBRESOURCE_DATA pData[6];
	stbi_uc* img;

	for (int i = 0; i < 6; i++)
	{
		img = stbi_load(str_file_path[i].c_str(), &width, &height, &num_channel, 0);

		if (stbi_failure_reason())
		{
			Logging::LogError("stbi_load failed to find texture");
			return false;
		}

		pData[i].pSysMem = img;
		pData[i].SysMemPitch = width * 4;
		pData[i].SysMemSlicePitch = 0;
	}

	D3D11_TEXTURE2D_DESC cubeDesc{};
	cubeDesc.Width = width;
	cubeDesc.Height = height;
	cubeDesc.MipLevels = 1u;
	cubeDesc.ArraySize = 6u;
	cubeDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
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

	hr = l_gfx->GetDevice()->CreateTexture2D(&cubeDesc, &pData[0], pTexture.GetAddressOf());
	if (FAILED(hr))
	{
		Logging::ThrowIf(hr, "Failed to Create Texture2D cubemap");
		return false;
	}

	hr = l_gfx->GetDevice()->CreateShaderResourceView(pTexture.Get(), &SMViewDesc, &pShaderResourceView);
	if (FAILED(hr))
	{
		Logging::ThrowIf(hr, "Failed to Create Shader Resource View for cubemap");
		return false;
	}

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
