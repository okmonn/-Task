#pragma once
#include <memory>

struct IDXGISwapChain4;

class Union;
class Window;
class Queue;

class Swap
{
public:
	// �R���X�g���N�^
	Swap(std::weak_ptr<Window>win, std::weak_ptr<Queue>queue);
	// �f�X�g���N�^
	~Swap();

	// �X���b�v�`�F�C��
	IDXGISwapChain4* Get(void) const {
		return swap;
	}
	// �o�b�N�o�b�t�@���̎擾
	unsigned int GetCnt(void) const {
		return cnt;
	}

private:
	// �X���b�v�`�F�C���̐���
	long Create(void);


	// ���j�I��
	Union& un;

	// �E�B���h�E
	std::weak_ptr<Window>win;

	// �R�}���h�L���[
	std::weak_ptr<Queue>queue;

	// �X���b�v�`�F�C��
	IDXGISwapChain4* swap;

	// �o�b�N�o�b�t�@�̐�
	unsigned int cnt;
};
