#pragma once
#include "PmdData.h"
#include <memory>

class Device;
class List;
class Constant;
class Root;
class Pipe;
class Texture;
class ModelLoader;

class Model
{
	// PMD���
	struct Pmd {
		//���_
		std::weak_ptr<std::vector<pmd::Vertex>>vertex;
		//�C���f�b�N�X
		std::weak_ptr<std::vector<unsigned short>>index;
		//�}�e���A��
		std::weak_ptr<std::vector<pmd::Material>>material;
		//�{�[��
		std::weak_ptr<std::vector<pmd::Born>>born;

		//�q�[�v
		ID3D12DescriptorHeap* heap;
		//�}�e���A���p���\�[�X
		ID3D12Resource* c_rsc;
		//�{�[���p���\�[�X
		ID3D12Resource* b_rsc;
		//�C���f�b�N�X�p���\�[�X
		ID3D12Resource* i_rsc;

		//���_�p���\�[�X
		ID3D12Resource* v_rsc;
		//�}�e���A���f�[�^���M
		unsigned char* c_data;
		//�{�[���s��f�[�^���M
		unsigned char* b_data;
		//���_�f�[�^���M
		unsigned int* v_data;

		//�e�N�X�`��
		std::map<int, int>tex;
		//��Z�e�N�X�`��
		std::map<int, int>sph;
		//���Z�e�N�X�`��
		std::map<int, int>spa;
		//�g�D�[���e�N�X�`��
		std::map<int, int>toon;

		//�{�[���m�[�h
		std::map<std::string, pmd::BornNode>node;

		//���M�}�e���A���f�[�^
		pmd::Mat mat;
		//���M�{�[���s��f�[�^
		std::vector<DirectX::XMMATRIX>bornMtx;
	};

public:
	// �R���X�g���N�^
	Model(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Constant>con,
		std::weak_ptr<Root>root, std::weak_ptr<Pipe>pipe, std::weak_ptr<Texture>tex);
	// �f�X�g���N�^
	~Model();

	// �ǂݍ���
	long Load(const std::string& fileName, int& i);

	// �`��
	void Draw(int& i);

private:
	// �e�N�X�`���̓ǂݍ���
	long LoadTexture(const std::string& fileName, int* i);

	// �}�e���A���p�V�F�[�_�r���[�̐���
	long CreateMaterialView(int* i);

	// �{�[���m�[�h�̃Z�b�g
	void SetNode(int* i);

	// �{�[���p�V�F�[�_�r���[�̐���
	long CreateBornView(int* i);

	// ���_�o�b�t�@�̐���
	long CreateVertex(int* i);


	// �f�o�C�X
	std::weak_ptr<Device>dev;

	// �R�}���h���X�g
	std::weak_ptr<List>list;

	// �萔�o�b�t�@
	std::weak_ptr<Constant>con;

	// ���[�g�V�O�l�`��
	std::weak_ptr<Root>root;

	// �p�C�v���C��
	std::weak_ptr<Pipe>pipe;

	// �e�N�X�`��
	std::weak_ptr<Texture>tex;

	// ���f�����[�_�[
	std::unique_ptr<ModelLoader>loader;

	// PMD���
	std::map<int*, Pmd>pmd;
};
