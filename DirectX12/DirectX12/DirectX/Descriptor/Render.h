#pragma once
#include "Descriptor.h"

class Render : 
	public Descriptor
{
public:
	// �R���X�g���N�^
	Render(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Swap>swap);
	// �f�X�g���N�^
	~Render();

	// �����_�[�^�[�Q�b�g�̃Z�b�g
	void SetRender(ID3D12DescriptorHeap* depth, UINT index = 0);

private:
	// ���\�[�X�̐���
	HRESULT Create(void);
};

