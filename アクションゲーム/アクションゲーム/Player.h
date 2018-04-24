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
	// ������̌���
	std::string FindString(const std::string path, const char find , int offset = 0, bool start = true);
	// �ǂݍ���
	void Load(void);


	// �摜�f�[�^
	int image;
	
	// �t���[����
	int flam;

	// ���W
	Positionf pos;

	// ����
	Rect rect;
};

