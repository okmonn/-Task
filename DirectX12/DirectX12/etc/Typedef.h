#pragma once
#include <DirectXMath.h>

// ���W�A���ϊ�
#define RAD(X) (X) * DirectX::XM_PI / 180.0f
// �ʓx�@�ϊ�
#define ANGLE(X) (X) * 180.0f / DirectX::XM_PI

// ���[���h�r���[�v���W�F�N�V����
struct WVP {
	//���[���h
	DirectX::XMMATRIX world;
	//�r���[�v���W�F�N�V����
	DirectX::XMMATRIX viewProjection;
};
