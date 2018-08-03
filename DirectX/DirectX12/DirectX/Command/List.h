#pragma once
#include "Command.h"

class List :
	public Command
{
public:
	// �R���X�g���N�^
	List(std::weak_ptr<Device>dev);
	// �f�X�g���N�^
	~List();

	// �R�}���h�A���P�[�^
	ID3D12CommandAllocator* GetAllo(void) const {
		return allo;
	}

	// �R�}���h���X�g
	ID3D12GraphicsCommandList* GetList(void) const {
		return list;
	}

private:
	// �R�}���h�A���P�[�^�̐���
	HRESULT CreateAllo(void);

	// �R�}���h���X�g�̐���
	HRESULT CreateList(void);


	// �R�}���h�A���P�[�^
	ID3D12CommandAllocator * allo;

	// �R�}���h���X�g
	ID3D12GraphicsCommandList* list;
};

