#pragma once
#include "../DirectX/Obj.h"
#include "WAVE/WAVE.h"
#include <dsound.h>
#include <memory>
#include <map>

class Window;

class Sound :
	public Obj
{
public:
	// �R���X�g���N�^
	Sound(std::weak_ptr<Window>win);
	// �f�X�g���N�^
	~Sound();

private:
	// �T�E���h�̐���
	HRESULT CreateSound(void);

	// �������[�h�̃Z�b�g
	HRESULT SetCooperative(void);

	// �v���C�}���T�E���h�o�b�t�@�̐���
	HRESULT CreateBuffer(UINT* index, const WAVE& wave);


	// �E�B���h�E
	std::weak_ptr<Window>win;

	// �_�C���N�g�T�E���h
	LPDIRECTSOUND8 sound;

	// �Z�J���_���[�o�b�t�@
	std::map<UINT*, LPDIRECTSOUNDBUFFER>buffer;
};

