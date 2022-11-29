#include <wincodec.h>
#include "Texture.h"
#include "Direct3D.h"

#pragma comment( lib, "WindowsCodecs.lib" )

Texture::Texture():pSampler_(nullptr), pSRV_(nullptr)
{
}

Texture::~Texture()
{
}

HRESULT Texture::Load(std::string fileName)
{
	//�t�@�C���������C�h������
	wchar_t wtext[FILENAME_MAX];
	size_t ret;
	mbstowcs_s(&ret, wtext, fileName.c_str(), fileName.length());

	//�摜�t�@�C�����J��
	CoInitialize(nullptr);

	IWICImagingFactory* pFactory = nullptr;
	IWICBitmapDecoder* pDecoder = nullptr;
	IWICBitmapFrameDecode* pFrame = nullptr;
	IWICFormatConverter* pFormatConverter = nullptr;
	CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, reinterpret_cast<void**>(&pFactory));
	HRESULT hr = pFactory->CreateDecoderFromFilename(wtext, NULL, GENERIC_READ, WICDecodeMetadataCacheOnDemand, &pDecoder);
	pDecoder->GetFrame(0, &pFrame);
	pFactory->CreateFormatConverter(&pFormatConverter);
	pFormatConverter->Initialize(pFrame, GUID_WICPixelFormat32bppRGBA, WICBitmapDitherTypeNone, NULL, 1.0f, WICBitmapPaletteTypeMedianCut);

	CoUninitialize();


	//�摜�̃T�C�Y���擾
	UINT imgWidth;
	UINT imgHeight;
	pFormatConverter->GetSize(&imgWidth, &imgHeight);


	//�e�N�X�`�����
	ID3D11Texture2D* pTexture;
	D3D11_TEXTURE2D_DESC texdec;
	texdec.Width = imgWidth;
	texdec.Height = imgHeight;
	texdec.MipLevels = 1;
	texdec.ArraySize = 1;
	texdec.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	texdec.SampleDesc.Count = 1;
	texdec.SampleDesc.Quality = 0;
	texdec.Usage = D3D11_USAGE_DYNAMIC;
	texdec.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texdec.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	texdec.MiscFlags = 0;
	Direct3D::pDevice->CreateTexture2D(&texdec, nullptr, &pTexture);

	//�f�o�C�X�R���e�L�X�g�Ƀe�N�X�`����n��
	D3D11_MAPPED_SUBRESOURCE hMappedres;
	Direct3D::pContext->Map(pTexture, 0, D3D11_MAP_WRITE_DISCARD, 0, &hMappedres);
	pFormatConverter->CopyPixels(nullptr, imgWidth * 4, imgWidth * imgHeight * 4, (BYTE*)hMappedres.pData);
	Direct3D::pContext->Unmap(pTexture, 0);

	//�T���v���[
	D3D11_SAMPLER_DESC  SamDesc;
	ZeroMemory(&SamDesc, sizeof(D3D11_SAMPLER_DESC));
	SamDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	SamDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	SamDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	Direct3D::pDevice->CreateSamplerState(&SamDesc, &pSampler_);


	//�V�F�[�_�[���\�[�X�r���[
	D3D11_SHADER_RESOURCE_VIEW_DESC srv = {};
	srv.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srv.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srv.Texture2D.MipLevels = 1;
	Direct3D::pDevice->CreateShaderResourceView(pTexture, &srv, &pSRV_);
	pTexture->Release();

    return S_OK;
}

void Texture::Release()
{
}

ID3D11SamplerState* Texture::GetSampler()
{
	return pSampler_;
}

ID3D11ShaderResourceView* Texture::GetSRV()
{
	return pSRV_;
}
