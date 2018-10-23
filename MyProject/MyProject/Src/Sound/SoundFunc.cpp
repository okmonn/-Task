#include "SoundFunc.h"
#include "../Func/Func.h"
#include <Windows.h>
#include <tchar.h>
#include <iostream>

// short�^�̃I�[�o�[�t���[�̖h�~
#define OVERFLLOW_SHORT 32768.0f

// char�^�̃I�[�o�[�t���[�̖h�~
#define OVERFLLOW_CHAR 127.0f

// �X�e���I8�r�b�g
struct Stereo8 {
	unsigned char left;
	unsigned char right;

	void operator=(const int& i) {
		left = i;
		right = i;
	}
};

// �X�e���I16�r�b�g
struct Stereo16 {
	short left;
	short right;

	void operator=(const int& i) {
		left = i;
		right = i;
	}
};

// RIFF�`�����N�̓ǂݍ���
int sound::LoadRIFF(RIFF & riff, FILE * file)
{
	if (file == nullptr)
	{
		OutputDebugString(_T("\n�t�@�C���ǂݍ��ݕs��\n"));
		return -1;
	}

	fread(&riff, sizeof(RIFF), 1, file);
	if (func::CheckChar("RIFF", riff.id) != true || func::CheckChar("WAVE", riff.type) != true)
	{
		OutputDebugString(_T("\nWAVE�t�@�C���ł͂���܂���\n"));
		fclose(file);
		return -1;
	}

	return 0;
}

// FMT�`�����N�̓ǂݍ���
int sound::LoadFMT(FMT & fmt, FILE * file)
{
	if (file == nullptr)
	{
		OutputDebugString(_T("\n�t�@�C���ǂݍ��ݕs��\n"));
		return -1;
	}

	fread(&fmt, sizeof(FMT), 1, file);
	if (func::CheckChar("fmt ", fmt.id) != true)
	{
		OutputDebugString(_T("\nFMT�`�����N�ł͂���܂���\n"));
		fclose(file);
		return -1;
	}

	//�g�������̓ǂݍ���
	std::vector<unsigned char>extended(fmt.size - (sizeof(fmt) - sizeof(fmt.id) - sizeof(fmt.size)));
	if (extended.size() > 0)
	{
		fread(extended.data(), sizeof(unsigned char) * extended.size(), 1, file);
	}


	return 0;
}

// DATA�`�����N�̓ǂݍ���
int sound::LoadDATA(DATA & data, FILE * file)
{
	if (file == nullptr)
	{
		OutputDebugString(_T("\n�t�@�C���ǂݍ��ݕs��\n"));
		return -1;
	}

	//�_�~�[�錾 
	std::string chunkID;
	chunkID.resize(4);

	while (true)
	{
		fread(&chunkID[0], sizeof(char) * chunkID.size(), 1, file);

		//DATA�`�����N����
		if (chunkID == "data")
		{
			break;
		}
		//���̑��̃`�����N
		else
		{
			unsigned long size = 0;
			fread(&size, sizeof(size), 1, file);

			std::vector<unsigned char>data(size);
			fread(data.data(), sizeof(unsigned char) * data.size(), 1, file);
		}
	}

	//DATA�̓ǂݍ��� 
	data.id = chunkID;
	fread(&data.size, sizeof(data.size), 1, file);

	return 0;
}

// �g�`�f�[�^ ���m�����E8�r�b�g
int sound::LoadMono8(std::vector<float>& data, FILE * file)
{
	if (file == nullptr)
	{
		OutputDebugString(_T("\n�t�@�C���ǂݍ��ݕs��\n"));
		return -1;
	}

	unsigned char tmp = 0;
	for (auto& i : data)
	{
		if (feof(file) == 0)
		{
			fread(&tmp, sizeof(unsigned char), 1, file);
		}
		else
		{
			tmp = 0;
		}

		//float�l�ɕϊ��E���f�[�^��0�`2�͈̔͂ɐ��K��
		i = static_cast<float>(tmp) / OVERFLLOW_CHAR;
	}

	return 0;
}

// �g�`�f�[�^�@���m�����E16�r�b�g
int sound::LoadMono16(std::vector<float>& data, FILE * file)
{
	if (file == nullptr)
	{
		OutputDebugString(_T("\n�t�@�C���ǂݍ��ݕs��\n"));
		return -1;
	}

	short tmp = 0;
	for (auto& i : data)
	{
		if (feof(file) == 0)
		{
			fread(&tmp, sizeof(short), 1, file);
		}
		else
		{
			tmp = 0;
		}

		//float�l�ɕϊ��E���f�[�^��-1�`1�͈̔͂ɐ��K��
		i = static_cast<float>(tmp) / OVERFLLOW_SHORT;
	}

	return 0;
}

// �g�`�f�[�^�@���m����
int sound::LoadMono(std::vector<float>& data, int bit, FILE * file)
{
	int flag = 0;
	switch (bit)
	{
	case 8:
		flag = LoadMono8(data, file);
		break;
	case 16:
		flag = LoadMono16(data, file);
		break;
	default:
		break;
	}

	return flag;
}

// �g�`�f�[�^�@�X�e���I�E8�r�b�g
int sound::LoadStereo8(std::vector<float>& data, FILE * file)
{
	if (file == nullptr)
	{
		OutputDebugString(_T("\n�t�@�C���ǂݍ��ݕs��\n"));
		return -1;
	}

	Stereo8 tmp = {};
	for (unsigned int i = 0; i < data.size(); i += 2)
	{
		if (feof(file) == 0)
		{
			fread(&tmp, sizeof(Stereo8), 1, file);
		}
		else
		{
			tmp = 0;
		}

		//float�l�ɕϊ��E���f�[�^��0�`2�͈̔͂ɐ��K��
		data[i] = static_cast<float>(tmp.left) / OVERFLLOW_CHAR;
		data[i + 1] = static_cast<float>(tmp.right) / OVERFLLOW_CHAR;
	}

	return 0;
}

// �g�`�f�[�^�@�X�e���I�E16�r�b�g
int sound::LoadStereo16(std::vector<float>& data, FILE * file)
{
	if (file == nullptr)
	{
		OutputDebugString(_T("\n�t�@�C���ǂݍ��ݕs��\n"));
		return -1;
	}

	Stereo16 tmp = {};
	for (unsigned int i = 0; i < data.size(); i += 2)
	{
		if (feof(file) == 0)
		{
			fread(&tmp, sizeof(Stereo16), 1, file);
		}
		else
		{
			tmp = 0;
		}

		//float�l�ɕϊ�
		data[i] = static_cast<float>(tmp.left) / OVERFLLOW_SHORT;
		data[i + 1] = static_cast<float>(tmp.right) / OVERFLLOW_SHORT;
	}

	return 0;
}

// �g�`�f�[�^�@�X�e���I
int sound::LoadStereo(std::vector<float>& data, int bit, FILE * file)
{
	int flag = 0;
	switch (bit)
	{
	case 8:
		flag = LoadStereo8(data, file);
		break;
	case 16:
		flag = LoadStereo16(data, file);
		break;
	default:
		break;
	}

	return flag;
}
