#pragma once
#define DIRECTSOUND_VERSION 0x0800
#include <dsound.h>
#include <memory>
#include <string>
#include <map>

class Window;
class WAVE;

class Sound
{
public:
	// �R���X�g���N�^
	Sound(std::weak_ptr<Window>win);
	// �f�X�g���N�^
	~Sound();

	// WAVE�̓ǂݍ���
	HRESULT LoadWAVE(UINT& index, const std::string& fileName);

private:
	// �T�E���h�̐���
	HRESULT CreateSound(void);

	// �������[�h�̃Z�b�g
	HRESULT SetCooperative(void);

	// �v���C�}���T�E���h�o�b�t�@�̐���
	HRESULT CreateBuffer(void);

	// �v���C�}���T�E���h�o�b�t�@�̃t�H�[�}�b�g�Z�b�g
	HRESULT SetFormat(void);

	// �Z�J���_���o�b�t�@�̐���
	HRESULT CreateScondly(UINT* index, const WAVE& wave);

	// �Z�J���_���o�b�t�@�̃��b�N
	HRESULT Lock(UINT* index, const WAVE& wave);


	// �E�B���h�E
	std::weak_ptr<Window>win;

	// �Q�ƌ���
	HRESULT result;

	// �_�C���N�g�T�E���h
	LPDIRECTSOUND8 sound;

	// �v���C�}���[�T�E���h�o�b�t�@
	LPDIRECTSOUNDBUFFER buffer;

	// �Z�J���_���[�o�b�t�@
	std::map<UINT*, LPDIRECTSOUNDBUFFER>snd;
};

