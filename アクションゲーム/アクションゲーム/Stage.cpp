#include "Stage.h"
#include "Load.h"

// �`�b�v�T�C�Y
#define CHIP_SIZE 64

const char* path = "�X�e�[�W/stage1.fmf";

// �R���X�g���N�^
Stage::Stage()
{
	Load(path);
	readX = 0;
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
	rect = Rect(0, 0, (data.mapWidth * CHIP_SIZE) * 2, (data.mapHeight * CHIP_SIZE));

	//�X�e�[�W�̓G�f�[�^
	eData = Load::GetInstance()->GetEnemyData(fileName);
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

// �X�e�[�W�̓G�f�[�^�̎擾
std::vector<UCHAR> Stage::GetEnemyData(int minx, int max)
{
	int left = max(minx / CHIP_SIZE, readX);
	int right = max / CHIP_SIZE;
	if (right <= readX)
	{
		return std::vector<UCHAR>();
	}

	auto index = left * data.mapHeight;
	auto indey = right * data.mapHeight;

	auto begin = eData.begin() + index;
	auto itr = eData.end();
	auto end = itr;
	
	if (indey < eData.size())
	{
		end = eData.begin() + indey;
		readX = right;
	}
	else
	{
		return std::vector<UCHAR>();
	}

	return std::vector<UCHAR>(begin, end);
}

std::vector<UCHAR> Stage::GetEnemyData(void)
{
	return eData;
}
