#include "SoundLoader.h"
#include "SoundFunc.h"
#include "ChunkData.h"
#include "../Func/Func.h"
#include <Windows.h>
#include <tchar.h>

// スレッド最大数
#define THREAD_MAX 10

// コンストラクタ
SoundLoader::SoundLoader() : 
	threadFlag(true)
{
	th.resize(THREAD_MAX);
	snd.clear();
}

// デストラクタ
SoundLoader::~SoundLoader()
{
	threadFlag = false;
	for (auto itr = th.begin(); itr != th.end(); ++itr)
	{
		if (itr->joinable() == true)
		{
			itr->join();
		}
	}

	th.clear();
	th.shrink_to_fit();

	snd.clear();
}

// 読み込み
int SoundLoader::Load(const std::string & fileName)
{
	if (snd.find(fileName) != snd.end())
	{
		return 0;
	}

	if (fopen_s(&snd[fileName].file, fileName.c_str(), "rb") != 0)
	{
		OutputDebugString(_T("\nファイルの参照：失敗\n"));
		return -1;
	}

	//チャンク宣言
	RIFF riff = {};
	FMT fmt   = {};
	DATA data = {};

	//RIFF
	if (sound::LoadRIFF(riff, snd[fileName].file) != 0)
	{
		return -1;
	}

	//FMT
	if (sound::LoadFMT(fmt, snd[fileName].file) != 0)
	{
		return -1;
	}

	//DATA
	if (sound::LoadDATA(data, snd[fileName].file) != 0)
	{
		return -1;
	}

	//必要データの格納
	snd[fileName].channel = fmt.channel;
	snd[fileName].sample  = fmt.sample;
	snd[fileName].bit     = fmt.bit;
	snd[fileName].byte    = fmt.byte;

	bool flag = true;
	while (flag)
	{
		for (auto itr = th.begin(); itr != th.end(); ++itr)
		{
			if (itr->joinable() == false)
			{
				snd[fileName].wave = std::make_shared<std::map<int, std::vector<float>>>();
				*itr = std::thread(&SoundLoader::LoadWave, this, fileName);
				flag = false;
				break;
			}
		}
	}

	return 0;
}

// 波形データの読み込み
void SoundLoader::LoadWave(const std::string & fileName)
{
	int read = 0;
	std::vector<float>tmp(snd[fileName].byte / (func::Byte(snd[fileName].bit) * snd[fileName].channel));

	while (std::feof(snd[fileName].file) == 0 && threadFlag == true)
	{
		switch (snd[fileName].channel)
		{
		case 1:
			sound::LoadMono(tmp, snd[fileName].bit, snd[fileName].file);
			break;
		case 2:
			sound::LoadStereo(tmp, snd[fileName].bit, snd[fileName].file);
			break;
		default:
			break;
		}

		snd[fileName].wave->insert(std::make_pair(read, tmp));
		++read;
	}

	fclose(snd[fileName].file);
}
