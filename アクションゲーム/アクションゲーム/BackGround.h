#pragma once
#include "Typedef.h"

class BackGround
{
public:
	// �R���X�g���N�^
	BackGround();
	// �f�X�g���N�^
	virtual ~BackGround();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

private:
	// �w�i�摜
	int back;
};

