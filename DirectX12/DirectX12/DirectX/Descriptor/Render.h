#pragma once
#include "Descriptor.h"

class Render : 
	public Descriptor
{
public:
	// �R���X�g���N�^
	Render(std::weak_ptr<Device>dev, std::weak_ptr<Swap>swap);
	// �f�X�g���N�^
	~Render();

private:
	// ���\�[�X�̐���
	HRESULT Create(void);
};

