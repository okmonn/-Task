#pragma once
#include "Obj.h"
#include <memory>

class List;
class Swap;
class Render;

class Barrier : 
	public Obj
{
public:
	// �R���X�g���N�^
	Barrier(std::weak_ptr<List>list, std::weak_ptr<Swap>swap, std::weak_ptr<Render>render);
	// �f�X�g���N�^
	~Barrier();

	// �o���A�̃Z�b�g
	void SetBarrier(D3D12_RESOURCE_STATES befor, D3D12_RESOURCE_STATES affter);

private:
	// �R�}���h���X�g
	std::weak_ptr<List>list;

	// �X���b�v�`�F�C��
	std::weak_ptr<Swap>swap;

	// �����_�[�^�[�Q�b�g
	std::weak_ptr<Render>render;

	// �o���A
	D3D12_RESOURCE_BARRIER barrier;
};

