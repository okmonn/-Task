#include "Load.h"
#include <cassert>
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

	for (auto itr = header.begin(); itr != header.end(); ++itr)
	{
		if (fileName == itr->first)
		{
			return true;
		}
	}

	//ファイル開らく
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

// ヘッダーの取得
ImageHeader Load::GetHeader(std::string m)
{
	return header[m];
}

// 画像データの取得
ImageData Load::GetImageData(std::string m, USHORT index)
{
	return data[m][index];
}

// 分割データの取得
std::vector<CutData> Load::GetCutData(std::string m, std::string name)
{
	return cut[m][name];
}

std::map<std::string, std::map<int, std::vector<Attack>>> Load::GetAttac(std::string m)
{
	return attack[m];
}

// 画像データのサイズの取得
UINT Load::GetImageDataSize(std::string m)
{
	return data[m].size();
}

// 分割データのサイズの取得
UINT Load::GetCutDataSize(std::string m)
{
	return cut[m].size();
}
