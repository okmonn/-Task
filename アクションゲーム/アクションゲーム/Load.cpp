#include "Load.h"

#include <tchar.h>

Load* Load::s_Instance = nullptr;

// �R���X�g���N�^
Load::Load()
{
	header = {};
	data.clear();
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

	//�t�@�C���J�炭
	if ((fopen_s(&file, fileName.c_str(), "rb")) != 0)
	{
		return false;
	}

	fread(&header.pathNum, sizeof(header.pathNum), 1, file);

	header.path.resize(header.pathNum);
	fread(&header.path[0], header.pathNum, 1, file);

	fread(&header.dataNum, sizeof(header.dataNum), 1, file);

	data.resize(header.dataNum);
	for (int i = 0; i < data.size(); ++i)
	{
		fread(&data[i].nameNum, sizeof(data[i].nameNum), 1, file);

		data[i].name.resize(data[i].nameNum);
		fread(&data[i].name[0], data[i].nameNum, 1, file);

		fread(&data[i].animCnt, sizeof(int), 1, file);
	}

	fclose(file);

	return true;
}
