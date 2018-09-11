#pragma once
#include "VmdDate.h"
#include <map>
#include <vector>

class VMD
{
public:
	// �R���X�g���N�^
	VMD();
	// �f�X�g���N�^
	~VMD();

	// �ǂݍ���
	int Load(unsigned int& index, const std::string& fileName);

	// ���[�V�����f�[�^�̎擾
	std::vector<Motion> GetMotion(unsigned int& index) {
		return motion[&index];
	}

private:
	// ���[�V����
	std::map<unsigned int*, std::vector<Motion>>motion;
};

