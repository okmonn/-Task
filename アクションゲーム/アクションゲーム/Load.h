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
	bool LoadMap(std::string fileName);

	// �w�b�_�[�̎擾
	ImageHeader GetHeader(std::string m);
	// �摜�f�[�^�̎擾
	ImageData GetImageData(std::string m, USHORT index);
	// �����f�[�^�̎擾
	std::vector<CutData> GetCutData(std::string m, std::string name);
	// ������Z�a�̎擾
	std::map<std::string, std::map<int, std::vector<Attack>>> GetAttac(std::string m);

	// �摜�f�[�^�̃T�C�Y�̎擾
	UINT GetImageDataSize(std::string m);
	// �����f�[�^�̃T�C�Y�̎擾
	UINT GetCutDataSize(std::string m);

	// �X�e�[�W�w�b�_�[�̎擾
	StageHeader GetStageHeader(std::string fileName);

	std::vector<UCHAR>GetEnemyData(std::string p,int min, int max);

private:
	// �R���X�g���N�^
	Load();

	// �C���X�^���X�ϐ�
	static Load* s_Instance;

	// �w�b�_�[
	std::map<std::string, ImageHeader> header;
	// �摜�f�[�^
	std::map<std::string, std::map<USHORT, ImageData>>data;
	// �����f�[�^
	std::map<std::string, std::map<std::string, std::vector<CutData>>>cut;
	// �U���Z�`�f�[�^
	std::map<std::string, std::map<std::string, std::map<int, std::vector<Attack>>>>attack;

	// �X�e�[�W�w�b�_�[
	std::map<std::string, StageHeader>st;

	// �X�e�[�W�̓G�f�[�^
	std::map<std::string, std::vector<UCHAR>>eData;
};