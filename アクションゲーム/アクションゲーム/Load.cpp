#include "Load.h"

#include <tchar.h>

Load* Load::s_Instance = nullptr;

// コンストラクタ
Load::Load()
{
	header = {};
	data.clear();
	cut.clear();
	attack.clear();
}

// デストラクタ
Load::~Load()
{
}

// インスタンス化
void Load::Create(void)
{
	if (s_Instance == nullptr)
	{
		s_Instance = new Load;
	}
}

// 破棄
void Load::Destroy(void)
{
	if (s_Instance != nullptr)
	{
		delete s_Instance;
	}
	s_Instance = nullptr;
}

// 読み込み
bool Load::LoadAct(std::string fileName)
{
	FILE* file;

	//ファイル開らく
	if ((fopen_s(&file, fileName.c_str(), "rb")) != 0)
	{
		return false;
	}

	fread(&header.ver, sizeof(header.ver), 1, file);

	fread(&header.pathNum, sizeof(header.pathNum), 1, file);

	header.path.resize(header.pathNum);
	fread(&header.path[0], header.pathNum, 1, file);

	fread(&header.dataNum, sizeof(header.dataNum), 1, file);

	ImageData dummy = {};
	for (int i = 0; i < header.dataNum; ++i)
	{
		fread(&dummy.nameNum, sizeof(dummy.nameNum), 1, file);

		dummy.name.resize(dummy.nameNum);
		fread(&dummy.name[0], dummy.nameNum, 1, file);

		fread(&dummy.loop, sizeof(dummy.loop), 1, file);

		fread(&dummy.animCnt, sizeof(dummy.animCnt), 1, file);

		data[i] = dummy;

		cut[dummy.name].resize(dummy.animCnt);
		for (int o = 0; o < dummy.animCnt; ++o)
		{
			fread(&cut[dummy.name][o], sizeof(cut[dummy.name][o]), 1, file);
			int num = 0;
			fread(&num, sizeof(num), 1, file);
			attack[dummy.name].resize(num);
			for (int p = 0; p < num; ++p)
			{
				fread(&attack[dummy.name][p], sizeof(attack[dummy.name][p]), 1, file);
			}
		}

		dummy = {};
	}

	fclose(file);

	return true;
}

// ヘッダーの取得
ImageHeader Load::GetHeader(void)
{
	return header;
}

// 画像データの取得
ImageData Load::GetImageData(USHORT index)
{
	return data[index];
}

// 分割データの取得
std::vector<CutData> Load::GetCutData(std::string name)
{
	return cut[name];
}

// 画像データのサイズの取得
UINT Load::GetImageDataSize(void)
{
	return data.size();
}

// 分割データのサイズの取得
UINT Load::GetCutDataSize(void)
{
	return cut.size();
}
