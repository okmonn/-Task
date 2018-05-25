#include "Load.h"
#include <cassert>
#include <tchar.h>

Load* Load::s_Instance = nullptr;

// �R���X�g���N�^
Load::Load()
{
	header = {};
	data.clear();
	cut.clear();
	attack.clear();
	st.clear();
	eData.clear();
}

// �f�X�g���N�^
Load::~Load()
{
}

// �C���X�^���X��
void Load::Create(void)
{
	if (s_Instance == nullptr)
	{
		s_Instance = new Load;
	}
}

// �j��
void Load::Destroy(void)
{
	if (s_Instance != nullptr)
	{
		delete s_Instance;
	}
	s_Instance = nullptr;
}

// �ǂݍ���
bool Load::LoadAct(std::string fileName)
{
	FILE* file;

	for (auto itr = header.begin(); itr != header.end(); ++itr)
	{
		if (fileName == itr->first)
		{
			return true;
			break;
		}
	}

	//�t�@�C���J�炭
	if ((fopen_s(&file, fileName.c_str(), "rb")) != 0)
	{
		return false;
	}

	fread(&header[fileName].ver, sizeof(header[fileName].ver), 1, file);

	assert(header[fileName].ver == 1.01f);

	fread(&header[fileName].pathNum, sizeof(header[fileName].pathNum), 1, file);

	header[fileName].path.resize(header[fileName].pathNum);
	fread(&header[fileName].path[0], header[fileName].pathNum, 1, file);

	fread(&header[fileName].dataNum, sizeof(header[fileName].dataNum), 1, file);

	ImageData dummy = {};
	for (int i = 0; i < header[fileName].dataNum; ++i)
	{
		fread(&dummy.nameNum, sizeof(dummy.nameNum), 1, file);

		dummy.name.resize(dummy.nameNum);
		fread(&dummy.name[0], dummy.nameNum, 1, file);

		fread(&dummy.loop, sizeof(dummy.loop), 1, file);

		fread(&dummy.animCnt, sizeof(dummy.animCnt), 1, file);

		data[fileName][i] = dummy;

		cut[fileName][dummy.name].resize(dummy.animCnt);
		for (int o = 0; o < dummy.animCnt; ++o)
		{
			fread(&cut[fileName][dummy.name][o], sizeof(cut[fileName][dummy.name][o]), 1, file);
			int num = 0;
			fread(&num, sizeof(num), 1, file);
			attack[fileName][dummy.name][o].resize(num);
			for (int p = 0; p < num; ++p)
			{
				fread(&attack[fileName][dummy.name][o][p], sizeof(attack[fileName][dummy.name][o][p]), 1, file);
			}
		}

		dummy = {};
	}

	fclose(file);

	return true;
}

// �ǂݍ���
bool Load::LoadMap(std::string fileName)
{
	FILE* file;

	for (auto itr = st.begin(); itr != st.end(); ++itr)
	{
		if (fileName == itr->first)
		{
			return true;
			break;
		}
	}
	
	//�t�@�C���J�炭
	if ((fopen_s(&file, fileName.c_str(), "rb")) != 0)
	{
		return false;
	}

	fread(&st[fileName], sizeof(st[fileName]), 1, file);

	std::vector<UCHAR>dummy;
	dummy.resize(st[fileName].mapHeight * st[fileName].mapWidth);

	eData[fileName].resize(st[fileName].mapHeight * st[fileName].mapWidth);

	for (UINT i = 0; i < eData[fileName].size(); ++i)
	{
		fread(&dummy[i], sizeof(UCHAR), 1, file);
	}

	fclose(file);

	for (UINT i = 0; i < st[fileName].mapHeight; ++i)
	{
		for (UINT j = 0; j < st[fileName].mapWidth; ++j)
		{
			eData[fileName][j * st[fileName].mapHeight + i] = dummy[i * st[fileName].mapWidth + j];
		}
	}

	return true;
}

// �w�b�_�[�̎擾
ImageHeader Load::GetHeader(std::string m)
{
	return header[m];
}

// �摜�f�[�^�̎擾
ImageData Load::GetImageData(std::string m, USHORT index)
{
	return data[m][index];
}

// �����f�[�^�̎擾
std::vector<CutData> Load::GetCutData(std::string m, std::string name)
{
	return cut[m][name];
}

std::map<std::string, std::map<int, std::vector<Attack>>> Load::GetAttac(std::string m)
{
	return attack[m];
}

// �摜�f�[�^�̃T�C�Y�̎擾
UINT Load::GetImageDataSize(std::string m)
{
	return data[m].size();
}

// �����f�[�^�̃T�C�Y�̎擾
UINT Load::GetCutDataSize(std::string m)
{
	return cut[m].size();
}

// �X�e�[�W�w�b�_�[�̎擾
StageHeader Load::GetStageHeader(std::string fileName)
{
	return st[fileName];
}

// �X�e�[�W�̓G�f�[�^�̎擾
std::vector<UCHAR> Load::GetEnemyData(std::string fileName,int min, int max)
{
	int index = min / 64;
	int indey = max / 64;

	auto it = eData[fileName].begin() + index * st[fileName].mapWidth;
	auto ir = eData[fileName].begin() + indey * st[fileName].mapHeight;
	return std::vector<UCHAR>(it, ir);
}

// �X�e�[�W�̓G�f�[�^�̎擾
std::vector<UCHAR> Load::GetEnemyData(std::string fileName)
{
	return eData[fileName];
}
