#pragma once
#include "Typedef.h"
#include <vector>

class Stage
{
public:
	// �R���X�g���N�^
	Stage();
	// �f�X�g���N�^
	virtual ~Stage();
	
	// �ǂݍ���
	void Load(std::string fileName);

	// ����
	void UpData(void);

	// �X�e�[�W�T�C�Y�̎擾
	Rect& GetStageRange(void);

	// �X�e�[�W�̓G�̎擾
	std::vector<UCHAR>GetEnemyData(int minx, int max);
	std::vector<UCHAR>GetEventData(int minx, int max);

private:
	// �X�e�[�W�̑S�̃T�C�Y
	Rect rect;

	// �X�e�[�W�f�[�^
	StageHeader data;

	// �X�e�[�W�̓G�f�[�^
	std::vector<UCHAR>eData;

	// �X�e�[�W�̃C�x���g�f�[�^
	std::vector<UCHAR>evData;

	// �ǂݍ���
	int readX;

	int readY;
};

