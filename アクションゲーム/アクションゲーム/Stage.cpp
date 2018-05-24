#include "Stage.h"
#include "Load.h"

//1�`�b�v 64x64�T�C�Y

#define CHIP_SIZE 64

const char* path = "�X�e�[�W/stage1.fmf";

// �R���X�g���N�^
Stage::Stage()
{
	Load(path);
}

// �f�X�g���N�^
Stage::~Stage()
{
}

// �ǂݍ���
void Stage::Load(std::string fileName)
{
	Load::GetInstance()->LoadMap(fileName);

	data = Load::GetInstance()->GetStageHeader(fileName);
	data.chipHeight *= 2;
	data.chipWidth *= 2;

	//�X�e�[�W�T�C�Y
	rect = Rect(0, 0, data.mapWidth * CHIP_SIZE, data.mapHeight * CHIP_SIZE);
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
