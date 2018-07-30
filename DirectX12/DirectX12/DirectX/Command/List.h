#pragma once
#include "../Obj.h"
#include <memory>

class Device;
class Allocator;

class List :
	public Obj
{
public:
	// �R���X�g���N�^
	List(std::weak_ptr<Device>dev, std::weak_ptr<Allocator>allo);
	// �f�X�g���N�^
	~List();

	// �R�}���h���X�g�̎擾
	ID3D12GraphicsCommandList* Get(void) const {
		return list;
	}

private:
	// �R�}���h���X�g�̐���
	HRESULT Create(void);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �R�}���h�A���P�[�^
	std::weak_ptr<Allocator>allo;

	// �R�}���h���X�g
	ID3D12GraphicsCommandList* list;
};

