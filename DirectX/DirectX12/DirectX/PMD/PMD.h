#pragma once
#include "../Descriptor/Descriptor.h"
#include <DirectXMath.h>
#include <string>
#include <vector>
#include <map>

class Pipe;
class Texture;
struct Model;

class PMD :
	public Descriptor
{
	// �}�e���A��
	struct Mat {
		//��{�F
		DirectX::XMFLOAT3 diffuse;
		//�e�N�X�`���t���O
		BOOL flag;
	};

public:
	// �R���X�g���N�^
	PMD(std::weak_ptr<Device>dev, std::weak_ptr<List>list, std::weak_ptr<Pipe>pipe, std::weak_ptr<Texture>tex);
	// �f�X�g���N�^
	~PMD();

	// PMD�ǂݍ���
	HRESULT LoadPMD(UINT& index, const std::string& fileName);

	// �`��
	void Draw(UINT& index);

private:
	// �e�N�X�`���̓ǂݍ���
	HRESULT LoadTex(UINT* index, const std::string& fileName);

	// �萔�o�b�t�@�q�[�v�̐���
	HRESULT CreateHeap(UINT* index);

	// �萔�o�b�t�@���\�[�X�̐���
	HRESULT CreateResource(UINT* index);

	// �萔�o�b�t�@�r���[�̐���
	HRESULT CreateConView(UINT* index);

	// ���_�o�b�t�@�̐���
	HRESULT CreateVeretxBuffer(UINT* index);

	// ���_�C���f�b�N�X�̐���
	HRESULT CreateIndexBuffer(UINT* index);


	// �p�C�v���C��
	std::weak_ptr<Pipe>pipe;

	// �e�N�X�`��
	std::weak_ptr<Texture>tex;

	// ���f���f�[�^
	std::map<UINT*, Model>model;

	// �}�e���A��
	Mat mat;
};

namespace func {
	// ������̌����E�擪���甲���o��
	std::string FindFirstString(const std::string& path, const CHAR& find, UINT offset = 1, bool end = false);

	// ������̌����E�������ꏊ���甲���o��
	std::string FindEndString(const std::string& path, const CHAR& find, UINT offset = 1, bool end = true);

	// �t�H���_�[���ƃt�@�C�����̘A��
	std::string FileLink(const std::string& path1, const std::string& path2);
}