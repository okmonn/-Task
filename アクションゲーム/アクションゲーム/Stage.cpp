#include "Stage.h"

// �R���X�g���N�^
Stage::Stage()
{
	//�X�e�[�W�T�C�Y
	rect = Rect(0, 0, 4000,0);
}

// �f�X�g���N�^
Stage::~Stage()
{
}

// ����
void Stage::UpData(void)
{
}

// �X�e�[�W�T�C�Y�̎擾
Rect & Stage::GetStageRange(void)
{
	return rect;
}
