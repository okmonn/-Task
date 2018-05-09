#pragma once
#include "Typedef.h"
#include <vector>
#include <map>
#include <string>

class Load
{
public:
	// �f�X�g���N�^
	~Load();

	// �C���X�^���X��
	static void Create(void);
	// �j��
	static void Destroy(void);

	// �C���X�^���X�ϐ��̎擾
	static Load* GetInstance(void)
	{
		return s_Instance;
	}

	// �ǂݍ���
	bool LoadAct(std::string fileName);

	// �w�b�_�[�̎擾
	ImageHeader GetHeader(void);
	// �摜�f�[�^�̎擾
	ImageData GetImageData(USHORT index);
	// �����f�[�^�̎擾
	std::vector<CutData> GetCutData(std::string name);

	// �摜�f�[�^�̃T�C�Y�̎擾
	UINT GetImageDataSize(void);
	// �����f�[�^�̃T�C�Y�̎擾
	UINT GetCutDataSize(void);

private:
	// �R���X�g���N�^
	Load();

	// �C���X�^���X�ϐ�
	static Load* s_Instance;

	// �w�b�_�[
	ImageHeader header;
	// �摜�f�[�^
	std::map<USHORT, ImageData>data;
	// �����f�[�^
	std::map<std::string, std::vector<CutData>>cut;
	// �U���Z�`�f�[�^
	std::map<std::string, std::map<int, std::vector<Attack>>>attack;
};