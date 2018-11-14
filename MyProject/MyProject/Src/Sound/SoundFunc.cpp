#include "SoundFunc.h"
#include "ChunkData.h"
#include "../Func/Func.h"
#include <Windows.h>
#include <tchar.h>
#include <iostream>

// short型のオーバーフローの防止
#define OVERFLLOW_SHORT 32768.0f

// char型のオーバーフローの防止
#define OVERFLLOW_CHAR 127.0f

// ステレオ8ビット
struct Stereo8 {
	unsigned char left;
	unsigned char right;

	void operator=(const int& i) {
		left = i;
		right = i;
	}
};

// ステレオ16ビット
struct Stereo16 {
	short left;
	short right;

	void operator=(const int& i) {
		left = i;
		right = i;
	}
};

// RIFFチャンクの読み込み
// 失敗時はdebug出力と、fclose実行済み
int sound::LoadRIFF(RIFF& riff, FILE* file)
{
	if (file == nullptr)
	{
		OutputDebugString(_T("\nファイル読み込み不可\n"));
		return -1;
	}

	fread(&riff, sizeof(RIFF), 1, file);
	if (func::CheckChar("RIFF", riff.id) != true || func::CheckChar("WAVE", riff.type) != true)
	{
		OutputDebugString(_T("\nWAVEファイルではありません\n"));
		fclose(file);
		return -1;
	}

	return 0;
}

// FMTチャンクの読み込み
// 失敗時はdebug出力と、fclose実行済み
int sound::LoadFMT(FMT & fmt, FILE * file)
{
	if (file == nullptr)
	{
		OutputDebugString(_T("\nファイル読み込み不可\n"));
		return -1;
	}

	fread(&fmt, sizeof(FMT), 1, file);
	if (func::CheckChar("fmt ", fmt.id) != true)
	{
		OutputDebugString(_T("\nFMTチャンクではありません\n"));
		fclose(file);
		return -1;
	}

	//拡張部分の読み込み
	std::vector<unsigned char>extended(fmt.size - (sizeof(fmt) - sizeof(fmt.id) - sizeof(fmt.size)));
	if (extended.size() > 0)
	{
		fread(extended.data(), sizeof(unsigned char) * extended.size(), 1, file);
	}


	return 0;
}

// DATAチャンクの読み込み
// 失敗時はdebug出力と、fclose実行済み
int sound::LoadDATA(DATA & data, FILE * file)
{
	if (file == nullptr)
	{
		OutputDebugString(_T("\nファイル読み込み不可\n"));
		return -1;
	}

	//ダミー宣言 
	std::string chunkID;
	chunkID.resize(4);

	while (true)
	{
		fread(&chunkID[0], sizeof(char) * chunkID.size(), 1, file);

		//DATAチャンク発見
		if (chunkID == "data")
		{
			break;
		}
		//その他のチャンク
		else
		{
			unsigned long size = 0;
			fread(&size, sizeof(size), 1, file);

			std::vector<unsigned char>data(size);
			fread(data.data(), sizeof(unsigned char) * data.size(), 1, file);
		}
	}

	//DATAの読み込み 
	data.id = chunkID;
	fread(&data.size, sizeof(data.size), 1, file);

	return 0;
}

// 波形データ モノラル・8ビット
int sound::LoadMono8(std::vector<float>& data, FILE * file)
{
	if (file == nullptr)
	{
		OutputDebugString(_T("\nファイル読み込み不可\n"));
		return -1;
	}

	unsigned char tmp = 0;
	for (auto& i : data)
	{
		if (feof(file) == 0)
		{
			fread(&tmp, sizeof(unsigned char), 1, file);
		}
		else
		{
			tmp = 0;
		}

		//float値に変換・音データを0～2の範囲に正規化
		i = static_cast<float>(tmp) / OVERFLLOW_CHAR;
	}

	return 0;
}

// 波形データ モノラル・16ビット
// 正規化実装
int sound::LoadMono16(std::vector<float>& data, FILE * file)
{
	if (file == nullptr)
	{
		OutputDebugString(_T("\nファイル読み込み不可\n"));
		return -1;
	}

	short tmp = 0;
	for (auto& i : data)
	{
		if (feof(file) == 0)
		{
			fread(&tmp, sizeof(short), 1, file);
		}
		else
		{
			tmp = 0;
		}

		//float値に変換・音データを-1～1の範囲に正規化
		i = static_cast<float>(tmp) / OVERFLLOW_SHORT;
	}

	return 0;
}

// 波形データ　モノラル
int sound::LoadMono(std::vector<float>& data, int bit, FILE * file)
{
	int flag = 0;
	switch (bit)
	{
	case 8:
		flag = LoadMono8(data, file);
		break;
	case 16:
		flag = LoadMono16(data, file);
		break;
	default:
		break;
	}

	return flag;
}

// 波形データ ステレオ・8ビット
int sound::LoadStereo8(std::vector<float>& data, FILE * file)
{
	if (file == nullptr)
	{
		OutputDebugString(_T("\nファイル読み込み不可\n"));
		return -1;
	}

	Stereo8 tmp = {};
	for (unsigned int i = 0; i < data.size(); i += 2)
	{
		if (feof(file) == 0)
		{
			fread(&tmp, sizeof(Stereo8), 1, file);
		}
		else
		{
			tmp = 0;
		}

		//float値に変換・音データを0～2の範囲に正規化
		data[i]     = static_cast<float>(tmp.left) / OVERFLLOW_CHAR;
		data[i + 1] = static_cast<float>(tmp.right) / OVERFLLOW_CHAR;
	}

	return 0;
}

// 波形データ ステレオ・16ビット
// 正規化実装
int sound::LoadStereo16(std::vector<float>& data, FILE * file)
{
	if (file == nullptr)
	{
		OutputDebugString(_T("\nファイル読み込み不可\n"));
		return -1;
	}

	Stereo16 tmp = {};
	for (unsigned int i = 0; i < data.size(); i += 2)
	{
		if (feof(file) == 0)
		{
			fread(&tmp, sizeof(Stereo16), 1, file);
		}
		else
		{
			tmp = 0;
		}

		//float値に変換
		data[i]     = static_cast<float>(tmp.left) / OVERFLLOW_SHORT;
		data[i + 1] = static_cast<float>(tmp.right) / OVERFLLOW_SHORT;
	}

	return 0;
}

// 波形データ　ステレオ
int sound::LoadStereo(std::vector<float>& data, int bit, FILE * file)
{
	int flag = 0;
	switch (bit)
	{
	case 8:
		flag = LoadStereo8(data, file);
		break;
	case 16:
		flag = LoadStereo16(data, file);
		break;
	default:
		break;
	}

	return flag;
}

// ログ関数
int sound::Log(const int & bottom, const int & value)
{
	return (int)(log(value) / log(bottom));
}

#define PI 3.14159265f

// 高速フーリエ変換
void sound::FFT(const std::vector<float>& waveData, std::vector<float>& real, std::vector<float>& imag, const int & size)
{
	real.resize(size);
	imag.resize(size);

	auto stage = sound::Log(2, size);

	//ダミー宣言
	int tmp1, tmp2, tmp3;
	float real1, real2, real3;
	float imag1, imag2, imag3;
	std::vector<int>index(size, 0);

	for (int s = 1; s <= stage; ++s)
	{
		for (int i = 0; i < pow(2, s - 1); ++i)
		{
			index[(int)pow(2, s - 1) + i] = index[i] + (int)pow(2, stage - s);

			for (int n = 0; n < pow(2, stage - s); ++n)
			{
				tmp1 = (int)pow(2, stage - s + 1) * i + n;
				tmp2 = (int)pow(2, stage - s) + tmp1;
				tmp3 = (int)pow(2, s - 1) * n;

				real1 = waveData[tmp1];
				real2 = waveData[tmp2];
				real3 = cosf((2.0f * PI * tmp3) / size);

				imag1 = 0.0f;
				imag2 = 0.0f;
				imag3 = -sinf((2.0f * PI * tmp3) / size);

				if (s < stage)
				{
					real[tmp1] = real1 + real2;
					real[tmp2] = (real1 - real2) * real3;

					imag[tmp1] = imag1 + imag2;
					imag[tmp2] = (imag1 - imag2) * real3;
				}
				else
				{
					real[tmp1] = real1 + real2;
					real[tmp2] = real1 - real2;

					imag[tmp1] = imag1 + imag2;
					imag[tmp2] = imag1 - imag2;
				}
			}
		}
	}

	float r_dummy;
	float i_dummy;

	//インデックスの並び替え
	for (int i = 0; i < size; ++i)
	{
		if (index[i] > i)
		{
			r_dummy = real[index[i]];
			i_dummy = imag[index[i]];

			real[index[i]] = real[i];
			imag[index[i]] = imag[i];

			real[i] = r_dummy;
			imag[i] = i_dummy;
		}
	}
}

// ディレイ
void sound::Delay(std::vector<float> & waveData, const float & attenuation, const float & delayTime, const int & loop, const int & sample)
{
	float back;
	float tmp;

	for (unsigned int i = 0; i < waveData.size(); ++i)
	{
		back = waveData[i];
		for (int n = 1; n <= loop; ++n)
		{
			tmp = i - n * (sample * delayTime);
			waveData[i] += (tmp >= 0.0f) ? powf(2.0f, (float)n) * back : 0.0f;

			//クリッピング
			if (waveData[i] > 1.0f)
			{
				waveData[i] = 1.0f;
			}
		}
	}
}

// ディストーション
void sound::Distortion(std::vector<float>& waveData, const float & gain, const float & level)
{
	for (unsigned int i = 0; i < waveData.size(); ++i)
	{
		waveData[i] *= gain;

		//クリッピング
		if (waveData[i] > 1.0f)
		{
			waveData[i] = 1.0f;
		}
		else if (waveData[i] < -1.0f)
		{
			waveData[i] = -1.0f;
		}

		//音量調節
		waveData[i] *= level;
	}
}
