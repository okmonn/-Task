#pragma once
#include "Typedef.h"

class Player
{
public:
	// �R���X�g���N�^
	Player();
	// �f�X�g���N�^
	virtual ~Player();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

private:
	// �摜�f�[�^
	int image;
	
	// �t���[����
	int flam;
};

