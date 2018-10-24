#pragma once
#include "TextureData.h"
#include <string>
#include <map>
#include <memory>

struct ID3D12DescriptorHeap;
struct ID3D12Resource;
struct D3D12_SUBRESOURCE_DATA;

class Device;

class TextureLoader
{
	// �摜�̃I���W���f�[�^
	struct Origin {
		//�f�R�[�h�f�[�^
		std::unique_ptr<unsigned char[]>decode;
		//�T�u�f�[�^
		std::shared_ptr<D3D12_SUBRESOURCE_DATA>sub;
		//�q�[�v
		ID3D12DescriptorHeap* heap;
		//�萔���\�[�X
		ID3D12Resource* rsc;
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
		return origin[fileName].rsc;
	}
	// �f�R�[�h�f�[�^�̎擾
	unsigned char* GetDecode(const std::string& fileName) {
		return origin[fileName].decode.get();
	}
	// �T�u�f�[�^�̎擾
	std::shared_ptr<D3D12_SUBRESOURCE_DATA> GetSub(const std::string& fileName) {
		return origin[fileName].sub;
	}

private:
	// �q�[�v�̐���
	long CreateHeap(const std::string& fileName);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �摜�f�[�^
	std::map<std::string, Origin> origin;
};
