#pragma once
#include "Typedef.h"
class Stage
{
public:
	// �R���X�g���N�^
	Stage();
	// �f�X�g���N�^
	virtual ~Stage();

	// ����
	void UpData(void);

	// �X�e�[�W�T�C�Y�̎擾
	Rect& GetStageRange(void);

private:
	// �X�e�[�W�̑S�̃T�C�Y
	Rect rect;
};

