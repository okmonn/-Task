#pragma once
#include <DirectXMath.h>

// �~����
#define PI 3.141592f

// ���W�A���ϊ�
#define RAD(X) (X) * PI / 180.0f

// ���_
struct Vertex {
	//���W
	DirectX::XMFLOAT3 pos;
	//�t�u
	DirectX::XMFLOAT2 uv;
};

// ��ԍs��
struct WVP
{
	//���[���h
	DirectX::XMMATRIX world;
	//�r���[�v���W�F�N�V����
	DirectX::XMMATRIX viewProjection;
};

// �摜�f�[�^
struct Image {
	//�E�B���h�E�T�C�Y
	DirectX::XMFLOAT2 window;
};
