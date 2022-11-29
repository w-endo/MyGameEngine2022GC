#pragma once
#include <d3d11.h>
#include "string"

class Texture
{
	ID3D11SamplerState* pSampler_;
	ID3D11ShaderResourceView* pSRV_;
public:
	Texture();
	~Texture();
	HRESULT Load(std::string fileName);
	void Release();

	ID3D11SamplerState* GetSampler();
	ID3D11ShaderResourceView* GetSRV();
};