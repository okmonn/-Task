#pragma once
#include "../Tstring.h"
#include <d3d12.h>

// �����[�X�}�N��
#define Release(X) { if((X) != nullptr) (X)->Release(); (X) = nullptr; }

class Obj
{
public:
	// �R���X�g���N�^
	Obj();
	// �f�X�g���N�^
	virtual ~Obj();

protected:
	// �f�o�b�O�p�o��
	void OutDebug(const std::tstring tex, HRESULT result);


	// �Q�ƌ���
	HRESULT result;
};

