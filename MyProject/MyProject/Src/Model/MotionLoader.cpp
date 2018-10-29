#include "MotionLoader.h"
#include "../etc/Release.h"
#include <algorithm>

struct MotionData {
	//�{�[����
	char name[15];
	//�t���[���ԍ�
	unsigned long flam;
	//�ʒu
	DirectX::XMFLOAT3 pos;
	//��]
	DirectX::XMFLOAT4 rotation;
	//�⊮
	std::vector<float>inter;
};

// �R���X�g���N�^
MotionLoader::MotionLoader()
{
	motion.clear();
}

// �f�X�g���N�^
MotionLoader::~MotionLoader()
{
}

// �ǂݍ���
int MotionLoader::Load(const std::string & fileName)
{
	FILE* file = nullptr;
	if (fopen_s(&file, fileName.c_str(), "rb") != 0)
	{
		OutputDebugString(_T("\nVMD�̓ǂݍ��݁F���s\n"));
		return -1;
	}

	fseek(file, 50, SEEK_SET);

	unsigned int num = 0;
	fread(&num, sizeof(unsigned int), 1, file);

	//���[�V����
	MotionData m = {};
	m.inter.resize(64);
	motion[fileName] = std::make_shared<std::map<std::string, std::vector<vmd::Motion>>>();

	for (unsigned int i = 0; i < num; ++i)
	{
		fread(&m.name,     sizeof(m.name),                         1, file);
		fread(&m.flam,     sizeof(m.flam),                         1, file);
		fread(&m.pos,      sizeof(m.pos),                          1, file);
		fread(&m.rotation, sizeof(m.rotation),                     1, file);
		fread(&m.inter[0], sizeof(unsigned char) * m.inter.size(), 1, file);

		if (motion[fileName]->find(m.name) == motion[fileName]->end())
		{
			motion[fileName]->insert(std::make_pair(m.name, std::vector<vmd::Motion>()));
		}

		for (auto& n : m.inter)
		{
			n /= 127.0f;
		}

		motion[fileName]->at(m.name).push_back({ m.flam, m.rotation, m.inter});
	}

	fclose(file);

	for (auto itr = motion[fileName]->begin(); itr != motion[fileName]->end(); ++itr)
	{
		std::sort(itr->second.begin(), itr->second.end(),
			[](vmd::Motion& m1, vmd::Motion& m2) {
			return m1.flam < m2.flam;
		});
	}

	return 0;
}