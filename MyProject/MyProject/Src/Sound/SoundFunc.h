#pragma once
#include <string>
#include <vector>

struct _iobuf;
typedef _iobuf FILE;

namespace sound {
	// RIFF
	struct RIFF {
		unsigned char id[4];
		long size;
		unsigned char type[4];
	};

	// FMT
	struct FMT {
		unsigned char id[4];
		long size;
		short type;
		short channel;
		long sample;
		long byte;
		short block;
		short bit;
	};

	// DATA
	struct DATA {
		std::string id;
		long size;
	};

	// RIFF�`�����N�̓ǂݍ���
	// ���s����debug�o�͂ƁAfclose���s�ς�
	int LoadRIFF(RIFF& riff, FILE* file);

	// FMT�`�����N�̓ǂݍ���
	// ���s����debug�o�͂ƁAfclose���s�ς�
	int LoadFMT(FMT& fmt, FILE* file);

	// DATA�`�����N�̓ǂݍ���
	// ���s����debug�o�͂ƁAfclose���s�ς�
	int LoadDATA(DATA& data, FILE* file);

	// �g�`�f�[�^ ���m�����E8�r�b�g
	// ���K������
	int LoadMono8(std::vector<float>&data, FILE* file);

	// �g�`�f�[�^�@���m�����E16�r�b�g
	// ���K������
	int LoadMono16(std::vector<float>&data, FILE* file);

	// �g�`�f�[�^�@���m����
	int LoadMono(std::vector<float>&data, int bit, FILE* file);

	// �g�`�f�[�^�@�X�e���I�E8�r�b�g
	// ���K������
	int LoadStereo8(std::vector<float>&data, FILE* file);

	// �g�`�f�[�^�@�X�e���I�E16�r�b�g
	// ���K������
	int LoadStereo16(std::vector<float>&data, FILE* file);

	// �g�`�f�[�^�@�X�e���I
	int LoadStereo(std::vector<float>&data, int bit, FILE* file);
}