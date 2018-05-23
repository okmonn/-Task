#pragma once
#include "Camera.h"

class BackGround
{
public:
	// �R���X�g���N�^
	BackGround(std::weak_ptr<Camera>cam);
	// �f�X�g���N�^
	virtual ~BackGround();

	// �`��
	void Draw(void);

	// ����
	void UpData(void);

private:
	// �J�����N���X
	std::weak_ptr<Camera>cam;

	// �w�i�摜
	int back;

	// ���W
	Positionf pos;

	float z;
};

