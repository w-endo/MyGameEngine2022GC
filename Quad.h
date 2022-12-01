#pragma once
#include "Direct3D.h"
#include <DirectXMath.h>
#include "Texture.h"
#include "Transform.h"

using namespace DirectX;

//�R���X�^���g�o�b�t�@�[
struct CONSTANT_BUFFER
{
	XMMATRIX	matWVP;
	XMMATRIX	matW;
};

//���_���
struct VERTEX
{
	XMVECTOR position;
	XMVECTOR uv;
	XMVECTOR normal;
};


class Quad
{
	ID3D11Buffer* pVertexBuffer_;	//���_�o�b�t�@
	ID3D11Buffer* pIndexBuffer_;
	ID3D11Buffer* pConstantBuffer_;	//�R���X�^���g�o�b�t�@
	Texture* pTexture_;

public:
	Quad();
	~Quad();
	void Initialize();
	void Draw(Transform& transform);
	void Release();
};