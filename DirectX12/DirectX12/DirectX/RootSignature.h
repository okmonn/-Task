#pragma once
#include "Obj.h"
#include <memory>

class Device;

class RootSignature :
	public Obj
{
public:
	// �R���X�g���N�^
	RootSignature(std::weak_ptr<Device>dev);
	// �f�X�g���N�^
	~RootSignature();

	// ���[�g�V�O�l�`���̎擾
	ID3D12RootSignature* Get(void) const {
		return rootSignature;
	}

	// ���b�Z�[�W�̎擾
	ID3DBlob* GetSig(void) const {
		return signature;
	}

	// �G���[���b�Z�[�W�̎擾
	ID3DBlob* GetErr(void) const {
		return error;
	}

private:
	// �V���A���C�Y
	HRESULT Serialize(void);

	// ���[�g�V�O�l�`���̐���
	HRESULT Create(void);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	//���[�g�V�O�l�`��
	ID3D12RootSignature* rootSignature;

	//���b�Z�[�W
	ID3DBlob* signature;

	//�G���[���b�Z�[�W
	ID3DBlob* error;

	// ���_�V�F�[�_
	ID3DBlob* vertex;

	// �s�N�Z���V�F�[�_
	ID3DBlob* pixel;
};

