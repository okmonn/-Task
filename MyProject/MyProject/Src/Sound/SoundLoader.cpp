#include "SoundLoader.h"
#include "SoundFunc.h"
#include "../Func/Func.h"
#include <Windows.h>
#include <tchar.h>

// 解放マクロ
#define Close(X) { if((X) != nullptr) fclose((X)); (X) = nullptr; }

// スレッド最大数
#define THREAD_MAX 10

// コンストラクタ
SoundLoader::SoundLoader() : 
	threadFlag(true)
{
	sound.clear();
	th.resize(THREAD_MAX);
}

// デストラクタ
SoundLoader::~SoundLoader()
{
	threadFlag = false;

	for (auto itr = th.begin(); itr != th.end(); ++itr)
	{
		itr->join();
	}

	for (auto itr = sound.begin(); itr != sound.end(); ++itr)
	{
		itr->second.data.clear();
	}
}

// 読み込み
int SoundLoader::Load(const std::string & fileName)
{
	if (sound.find(fileName) == sound.end())
	{
		if (fopen_s(&sound[fileName].file, fileName.c_str(), "rb") != 0)
		{
			OutputDebugString(_T("\nファイルの参照：失敗\n"));
			return -1;
		}

		sound::RIFF riff = {};
		sound::FMT fmt   = {};
		sound::DATA data = {};

		//RIFF
		if (sound::LoadRIFF(riff, sound[fileName].file) != 0)
		{
			return -1;
		}

		//FMT
		if (sound::LoadFMT(fmt, sound[fileName].file) != 0)
		{
			return -1;
		}

		//DATA
		if (sound::LoadDATA(data, sound[fileName].file) != 0)
		{
			return -1;
		}

		//必要なものの格納
		sound[fileName].channel = fmt.channel;
		sound[fileName].sample  = fmt.sample;
		sound[fileName].bit     = fmt.bit;
		sound[fileName].byte    = fmt.byte;

		for (auto itr = th.begin(); itr != th.end(); ++itr)
		{
			if (itr->joinable() == false)
			{
				*itr = std::thread(&SoundLoader::LoadWave, this, fileName);
				break;
			}
		}
	}

	return 0;
}

// 波形の読み込み
void SoundLoader::LoadWave(const std::string& fileName)
{
	int read = 0;

	while (std::feof(sound[fileName].file) == 0 && threadFlag)
	{
		//配列のメモリ確保
		sound[fileName].data[read].resize(sound[fileName].byte / (func::Byte(sound[fileName].bit) * sound[fileName].channel));
		
		switch (sound[fileName].channel)
		{
		case 1:
			sound::LoadMono(sound[fileName].data[read], sound[fileName].bit, sound[fileName].file);
			break;
		case 2:
			sound::LoadStereo(sound[fileName].data[read], sound[fileName].bit, sound[fileName].file);
			break;
		default:
			break;
		}

		//読み込み位置を次に進める
		++read;
	}

	Close(sound[fileName].file);
}
