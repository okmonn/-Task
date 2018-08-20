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

	// �����f�[�^�̓ǂݍ���
	int Load(void);

	// �C���f�b�N�X�̎擾
	int GetIndex(void) const {
		return index == 0 ? 1 : 0;
	}
	// �ǂݍ��ݏI���t���O�̎擾
	bool GetEnd(void) const {
		return end;
	}


	// �t�H�[�}�b�g
	WAVEFORMATEX format;

	// �R�[���o�b�N
	VoiceCallback callback;

	// �f�[�^
	std::vector<std::vector<BYTE>>data;

private:
	// �����m�F
	int CheckChar(unsigned char * data, int dataSize, const std::string & find);


	// �t�@�C��
	FILE * file;

	// �C���f�b�N�X
	int index;

	// ���݂̓ǂݍ��݃o�C�g��
	int read;

	// �ő�ǂݍ��݃o�C�g��
	int readMax;

	// �ǂݍ��ݏI���t���O
	bool end;
};
