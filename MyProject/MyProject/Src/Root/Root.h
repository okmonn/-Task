#pragma once
#include <string>
#include <memory>

struct ID3D12RootSignature;
struct ID3D10Blob;
typedef ID3D10Blob ID3DBlob;
struct D3D12_ROOT_PARAMETER;

class Device;

class Root
{
public:
	// �R���X�g���N�^
	Root(std::weak_ptr<Device>dev, const std::wstring& fileName);
	// �f�X�g���N�^
	~Root();

	// ���[�g�V�O�l�`���̎擾
	ID3D12RootSignature* Get(void) const {
		return root;
	}
	// ���b�Z�[�W�̎擾
	ID3DBlob* GetMsg(void) const {
		return signature;
	}
	// �G���[���b�Z�[�W�̎擾
	ID3DBlob* GetError(void) const {
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
	// �V�F�[�_�̃R���p�C��
	long ShaderCompile(const std::wstring& fileName);

	// ���[�g�V�O�l�`���̐���
	long Create(void);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// ���[�g�V�O�l�`��
	ID3D12RootSignature* root;

	// ���b�Z�[�W
	ID3DBlob * signature;

	// �G���[���b�Z�[�W
	ID3DBlob* error;

	// ���_�V�F�[�_
	ID3DBlob* vertex;

	// �s�N�Z���V�F�[�_
	ID3DBlob* pixel;
};
