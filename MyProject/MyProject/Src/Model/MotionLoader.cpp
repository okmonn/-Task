#include "MotionLoader.h"
#include "../etc/Release.h"

struct MotionData {
	//ボーン名
	char name[15];
	//フレーム番号
	unsigned long flam;
	//位置
	DirectX::XMFLOAT3 pos;
	//回転
	DirectX::XMFLOAT4 rotation;
	//補完
	unsigned char interpolation[64];
};

// コンストラクタ
MotionLoader::MotionLoader()
{
	motion.clear();
}

// デストラクタ
MotionLoader::~MotionLoader()
{
}

// 読み込み
int MotionLoader::Load(const std::string & fileName)
{
	FILE* file = nullptr;
	if (fopen_s(&file, fileName.c_str(), "rb") != 0)
	{
		OutputDebugString(_T("\nVMDの読み込み：失敗\n"));
		return -1;
	}

	fseek(file, 50, SEEK_SET);

	unsigned int num = 0;
	fread(&num, sizeof(unsigned int), 1, file);

	//モーション
	MotionData m = {};
	motion[fileName] = std::make_shared<std::map<std::string, std::vector<vmd::Motion>>>();

	for (unsigned int i = 0; i < num; ++i)
	{
		fread(&m.name,          sizeof(m.name),          1, file);
		fread(&m.flam,          sizeof(m.flam),          1, file);
		fread(&m.pos,           sizeof(m.pos),           1, file);
		fread(&m.rotation,      sizeof(m.rotation),      1, file);
		fread(&m.interpolation, sizeof(m.interpolation), 1, file);

		if (motion[fileName]->find(m.name) == motion[fileName]->end())
		{
			motion[fileName]->insert(std::make_pair(m.name, std::vector<vmd::Motion>()));
		}

		motion[fileName]->at(m.name).push_back({ m.flam, m.rotation });
	}

	fclose(file);

	return 0;
}
