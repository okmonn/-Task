#pragma once
#include "WAVE.h"
#include <map>

class Xaudio2
{
public:
	// �R���X�g���N�^
	Xaudio2();
	// �f�X�g���N�^
	~Xaudio2();

	// �ǂݍ���
	HRESULT LoadWAVE(const std::string& fileName, WAVE& wave);

	// �\�[�X�{�C�X�̏���
	void Delete(WAVE& wave);

private:
	// ������
	HRESULT Init(void);

	// XAudio2�̐���
	HRESULT CreateAudio(void);

	// �f�o�b�O�̃Z�b�g
	void SetDebug(void);

	// �}�X�^�[�{�C�X�̐���
	HRESULT CreateMaster(void);


	// �Q�ƌ���
	HRESULT result;

	// XAudio2
	IXAudio2 * audio;

	// �}�X�^�[�{�C�X
	IXAudio2MasteringVoice* master;

	// �\�[�X�{�C�X
	std::map<WAVE*, IXAudio2SourceVoice*>voice;
};

