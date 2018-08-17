#pragma once
#include "VoiceCallback.h"
#include <iostream>
#include <vector>
#include <string>

class WAVE
{
public:
	// �R���X�g���N�^
	WAVE();
	// �f�X�g���N�^
	~WAVE();

	// �ǂݍ���
	int Load(const std::string& fileName);

	
	// �t�H�[�}�b�g
	WAVEFORMATEX format;

	// �R�[���o�b�N
	VoiceCallback callback;

	// ���݃f�[�^
	std::vector<BYTE>data;

private:
	// �����m�F
	int CheckChar(unsigned char * data, int dataSize, const std::string & find);


	// �t�@�C��
	FILE * file;
};
