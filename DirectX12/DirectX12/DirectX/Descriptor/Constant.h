#pragma once
#include "Descriptor.h"

class Window;

class Constant :
	public Descriptor
{
public:
	// �R���X�g���N�^
	Constant(std::weak_ptr<Window>win, std::weak_ptr<Device>dev, std::weak_ptr<Swap>swap);
	// �f�X�g���N�^
	~Constant();

private:
	// WVP�̃Z�b�g
	void SetWorldViewProjection(void);

	// ���\�[�X�̐���
	HRESULT CreateResource(void);

	// ���\�[�X�r���[�̐���
	HRESULT CreateView(void);


	// �E�B���h�E
	std::weak_ptr<Window>win;

	// WVP
	WVP wvp;
};

