#pragma once
#include "PmdData.h"
#include <memory>

class MotionLoader
{
public:
	// �R���X�g���N�^
	MotionLoader();
	// �f�X�g���N�^
	~MotionLoader();

	// �ǂݍ���
	int Load(const std::string& fileName);

	// ���[�V�����f�[�^�̎擾
	std::shared_ptr<std::map<std::string, std::vector<vmd::Motion>>> Get(const std::string& fileName) {
		return motion[fileName];
	}

	// �A�j���[�V�����̎��Ԃ̎擾
	float GetAnimTime(const std::string& fileName) {
		return animTime[fileName];
	}

private:
	// ���[�V�����f�[�^
	std::map<std::string, std::shared_ptr<std::map<std::string, std::vector<vmd::Motion>>>>motion;

	// �A�j���[�V�����̎���
	std::map<std::string, float>animTime;
};
