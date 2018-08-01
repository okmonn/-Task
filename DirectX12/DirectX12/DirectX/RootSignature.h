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
	RootSignature(std::weak_ptr<Device>dev, LPCWSTR fileName, LPCSTR func1, LPCSTR func2);
	// �f�X�g���N�^
	~RootSignature();

	// ���_�V�F�[�_�̃R���p�C��
	HRESULT ComVer(LPCWSTR fileName, LPCSTR func, LPCSTR target = "vs_5_0");
	// �s�N�Z���V�F�[�_�̃R���p�C��
	HRESULT ComPix(LPCWSTR fileName, LPCSTR func, LPCSTR target = "ps_5_0");

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

	// ���_�V�F�[�_�̎擾
	ID3DBlob* GetVertex(void) const {
		return vertex;
	}

	// �s�N�Z���V�F�[�_�̎擾
	ID3DBlob* GetPixel(void) const {
		return pixel;
	}

private:
	// �V���A���C�Y
	HRESULT Serialize(void);

	// ���[�g�V�O�l�`���̐���
	HRESULT Create(void);
	// ���[�g�V�O�l�`���̐���
	HRESULT CreateSignature(void);


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

