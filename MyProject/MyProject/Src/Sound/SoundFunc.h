#pragma once
#include <string>
#include <vector>

struct _iobuf;
typedef _iobuf FILE;
struct RIFF;
struct FMT;
struct DATA;

namespace sound
{
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

	// ���O�֐�
	int Log(const int& bottom, const int& value);

	// �����t�[���G�ϊ�
	void FFT(const std::vector<float>& waveData, std::vector<float>& real, std::vector<float>& imag, const int& size);

	// �f�B���C
	void Delay(std::vector<float>& waveData, const float& attenuation, const float& delayTime, const int& loop, const int& sample = 44100);

	// �f�B�X�g�[�V����
	void Distortion(std::vector<float>& waveData, const float& gain, const float& level);

	// ���~�b�^�[
	void Limiter(std::vector<float>& waveData);
}