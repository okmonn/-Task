#pragma once
#include <string>
#include <map>
#include <memory>

namespace DirectX {
	struct TexMetadata;
	class ScratchImage;
}

struct ID3D12DescriptorHeap;
struct ID3D12Resource;

class Device;

class TextureLoader
{
	// �摜�̃I���W���f�[�^
	struct Origin {
		//���^�f�[�^
		DirectX::TexMetadata* meta;
		//�X�N���b�`�C���[�W
		DirectX::ScratchImage* img;
		//�q�[�v
		ID3D12DescriptorHeap* heap;
		//�萔���\�[�X
		ID3D12Resource* c_rsc;
	};

public:
	// �R���X�g���N�^
	TextureLoader(std::weak_ptr<Device>dev);
	// �f�X�g���N�^
	~TextureLoader();

	// �ǂݍ���
	long Load(const std::string& fileName);

	// �萔�q�[�v�̎擾
	ID3D12DescriptorHeap* GetHeap(const std::string& fileName) {
		return origin[fileName].heap;
	}
	// �萔���\�[�X�̎擾
	ID3D12Resource* GetConRsc(const std::string& fileName) {
		return origin[fileName].c_rsc;
	}
	// ���^�f�[�^�̎擾
	DirectX::TexMetadata* GetMeta(const std::string& fileName) {
		return origin[fileName].meta;
	}
	// �X�N���b�`�C���[�W�̎擾
	DirectX::ScratchImage* GetImg(const std::string& fileName) {
		return origin[fileName].img;
	}

private:
	// �q�[�v�̐���
	long CreateHeap(const std::string& fileName);

	// �萔���\�[�X�̐���
	long CreateRsc(const std::string& fileName);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �摜�f�[�^
	std::map<std::string, Origin> origin;
};
