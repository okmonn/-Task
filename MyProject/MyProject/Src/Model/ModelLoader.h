#pragma once
#include "PmdData.h"
#include <memory>

class Device;

class ModelLoader
{
public:
	// �R���X�g���N�^
	ModelLoader(std::weak_ptr<Device>dev);
	// �f�X�g���N�^
	~ModelLoader();

	// �ǂݍ���
	long Load(const std::string& fileName);

	// ���_�̎擾
	std::shared_ptr<std::vector<pmd::Vertex>> GetVertex(const std::string& fileName) {
		return vertex[fileName];
	}
	// �C���f�b�N�X�̎擾
	std::shared_ptr<std::vector<unsigned short>> GetIndex(const std::string& fileName) {
		return index[fileName];
	}
	// �}�e���A���̎擾
	std::shared_ptr<std::vector<pmd::Material>> GetMaterial(const std::string& fileName) {
		return material[fileName];
	}
	// �{�[���̎擾
	std::shared_ptr<std::vector<pmd::Born>> GetBorn(const std::string& fileName) {
		return born[fileName];
	}
	// �q�[�v�̎擾
	ID3D12DescriptorHeap* GetHeap(const std::string& fileName) {
		return heap[fileName];
	}
	// �}�e���A���p���\�[�X�̎擾
	ID3D12Resource* GetMaterialRsc(const std::string& fileName) {
		return c_rsc[fileName];
	}
	// �{�[���p���\�[�X�̎擾
	ID3D12Resource* GetBornRsc(const std::string& fileName) {
		return b_rsc[fileName];
	}
	// �C���f�b�N�X�p���\�[�X�̎擾
	ID3D12Resource* GetIndexRsc(const std::string& fileName) {
		return i_rsc[fileName];
	}

private:
	// �q�[�v�̐���
	long CreateHeap(const std::string& fileName);

	// �萔���\�[�X�̐���
	long CreateConRsc(ID3D12Resource** rsc, const int& w);

	// �C���f�b�N�X�̐���
	long CreateIndex(const std::string& fileName);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// ���_
	std::map<std::string, std::shared_ptr<std::vector<pmd::Vertex>>>vertex;

	// �C���f�b�N�X
	std::map<std::string, std::shared_ptr<std::vector<unsigned short>>>index;

	// �}�e���A��
	std::map<std::string, std::shared_ptr<std::vector<pmd::Material>>>material;

	// �{�[��
	std::map<std::string, std::shared_ptr<std::vector<pmd::Born>>>born;

	// �q�[�v
	std::map<std::string, ID3D12DescriptorHeap*>heap;

	// �}�e���A���p���\�[�X
	std::map<std::string, ID3D12Resource*>c_rsc;

	// �{�[���p���\�[�X
	std::map<std::string, ID3D12Resource*>b_rsc;

	// �C���f�b�N�X�p���\�[�X
	std::map<std::string, ID3D12Resource*>i_rsc;

	// �C���f�b�N�X���M�f�[�^
	std::map<std::string, unsigned int*>data;
};
