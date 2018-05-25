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

private:
	// �X�e�[�W�̑S�̃T�C�Y
	Rect rect;

	// �X�e�[�W�f�[�^
	StageHeader data;

	// �X�e�[�W�̓G�f�[�^
	std::vector<UCHAR>eData;
};

