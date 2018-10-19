#pragma once
#include <string>
#include <map>
#include <memory>

namespace DirectX {
	struct TexMetadata;
	class ScratchImage;
}

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
		//���\�[�X
		ID3D12Resource* rsc;
	};

public:
	// �R���X�g���N�^
	TextureLoader(std::weak_ptr<Device>dev);
	// �f�X�g���N�^
	~TextureLoader();

	// �ǂݍ���
	long Load(const std::string& fileName, ID3D12Resource* rsc, DirectX::TexMetadata* meta, DirectX::ScratchImage* img);

private:
	// �萔���\�[�X�̐���
	long CreateRsc(const std::string& fileName);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �摜�f�[�^
	std::map<std::string, Origin> origin;
};
