#pragma once
#include "Descriptor.h"

class Window;

class Depth :
	public Descriptor
{
public:
	// �R���X�g���N�^
	Depth(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Swap>swap);
	// �f�X�g���N�^
	~Depth();

	// �[�x�X�e���V���̃Z�b�g
	void SetDepth(UINT index = 0);

private:
	// ���\�[�X����
	HRESULT CreateResource(void);

	// ���\�[�X�r���[�̐���
	HRESULT CreateView(void);


	// �E�B���h�E
	std::weak_ptr<Window>win;
};
