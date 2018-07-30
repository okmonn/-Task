#pragma once
#include "Obj.h"
#include <dxgi1_4.h>
#include <memory>

class Window;
class Queue;

class Swap :
	public Obj
{
public:
	// �R���X�g���N�^
	Swap(std::weak_ptr<Window>win, std::weak_ptr<Queue>queue);
	// �f�X�g���N�^
	~Swap();

	// �C���^�[�t�F�[�X�t�@�N�g���[�̎擾
	IDXGISwapChain3* Get(void) const {
		return swap;
	}

	// �o�b�N�o�b�t�@���̎擾
	UINT GetBack(void) const {
		return bufferCnt;
	}

private:
	// �t�@�N�g���[�̐���
	HRESULT CreateFactory(void);

	// �X���b�v�`�F�C���̐���
	HRESULT Create(void);


	// �E�B���h�E
	std::weak_ptr<Window>win;

	// �R�}���h���X�g
	std::weak_ptr<Queue>queue;

	//�C���^�[�t�F�[�X�t�@�N�g���[
	IDXGIFactory4* factory;

	//�X���b�v�`�F�C��
	IDXGISwapChain3* swap;

	//�o�b�N�o�b�t�@��
	UINT bufferCnt;

};

