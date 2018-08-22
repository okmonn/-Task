#include "WAVE.h"

// IDサイズ
#define ID_MAX 4
// バイト変換
#define BYTE(X) (X) / 8

// 読み込みデータ配列の最大数
#define DATA_MAX 2

// RIFFチャンク
struct RIFF {
	//チャンクID
	unsigned char chunkID[ID_MAX];
	//チャンクサイズ
	unsigned long chunkSize;
	//フォーマットタイプ
	unsigned char chunkFormatType[ID_MAX];
};

// FMTチャンク
struct FMT {
	//チャンクID
	unsigned char chunkID[ID_MAX];
	//チャンクサイズ
	unsigned long chunkSize;
	//フォーマットタイプ
	unsigned short waveFormatType;
	//フォーマットチャンネル
	unsigned short formatChannel;
	//サンプリング周波数
	unsigned long samplesPerSec;
	//ビットレート
	unsigned long bytesPerSec;
	//ブロックサイズ
	unsigned short blockSize;
	//量子化ビット数
	unsigned short bitsPerSample;
};

// DATAチャンク
struct DATA {
	//チャンクID
	std::string chunkID;
	//チャンクサイズ
	unsigned long chunkSize;
};

// コンストラクタ
WAVE::WAVE() :
	file(nullptr), index(0), read(0), readMax(0), start(false), end(false)
{
	format = {};
	data.clear();
	data.resize(DATA_MAX);
}

// デストラクタ
WAVE::~WAVE()
{
	if (file != nullptr)
	{
		fclose(file);
		file = nullptr;
	}
}

// 文字確認
int WAVE::CheckChar(unsigned char * data, int dataSize, const std::string & find)
{
	for (unsigned int i = 0; i < find.size(); ++i)
	{
		for (int d = 0; d < dataSize; ++d)
		{
			if (data[d] == find[i])
			{
				break;
			}
			else
			{
				//文字確認失敗
				if (d >= dataSize - 1)
				{
					return -1;
				}
			}
		}
	}

	return 0;
}

// 読み込み
int WAVE::Load(const std::string & fileName)
{
	//ファイルオープン
	if (fopen_s(&file, fileName.c_str(), "rb") != 0)
	{
		return -1;
	}

	//チャンク宣言
	RIFF riff = {};
	FMT fmt = {};
	DATA data = {};

	//RIFF読み込み
	fread(&riff.chunkID[0], sizeof(riff.chunkID), 1, file);
	if (CheckChar(riff.chunkID, ID_MAX, "RIFF") != 0)
	{
		fclose(file);
		return -1;
	}
	fread(&riff.chunkSize, sizeof(riff.chunkSize), 1, file);
	fread(&riff.chunkFormatType[0], sizeof(riff.chunkFormatType), 1, file);
	if (CheckChar(riff.chunkFormatType, ID_MAX, "WAVE") != 0)
	{
		fclose(file);
		return -1;
	}

	//FMT読み込み
	fread(&fmt.chunkID[0], sizeof(fmt.chunkID), 1, file);
	if (CheckChar(fmt.chunkID, ID_MAX, "fmt ") != 0)
	{
		fclose(file);
		return -1;
	}
	fread(&fmt.chunkSize, sizeof(fmt.chunkSize), 1, file);
	fread(&fmt.waveFormatType, sizeof(fmt.waveFormatType), 1, file);
	fread(&fmt.formatChannel, sizeof(fmt.formatChannel), 1, file);
	fread(&fmt.samplesPerSec, sizeof(fmt.samplesPerSec), 1, file);
	fread(&fmt.bytesPerSec, sizeof(fmt.bytesPerSec), 1, file);
	fread(&fmt.blockSize, sizeof(fmt.blockSize), 1, file);
	fread(&fmt.bitsPerSample, sizeof(fmt.bitsPerSample), 1, file);
	//拡張部分
	std::vector<unsigned char>extended;
	extended.resize(fmt.chunkSize - (sizeof(fmt) - sizeof(fmt.chunkID) - sizeof(fmt.chunkSize)));
	if (extended.size() > 0)
	{
		fread(&extended[0], sizeof(unsigned char) * extended.size(), 1, file);
	}

	//ダミー宣言
	std::string chunkID;
	chunkID.resize(sizeof(unsigned char) * ID_MAX);
	//ID判別
	fread(&chunkID[0], sizeof(unsigned char) * ID_MAX, 1, file);

	//DATA以外の場合
	while (chunkID != "data")
	{
		//サイズ
		unsigned long size = 0;
		fread(&size, sizeof(size), 1, file);
		//データ
		std::vector<unsigned char>data;
		data.resize(size);
		fread(&data[0], sizeof(unsigned char) * size, 1, file);
		//ID
		fread(&chunkID[0], sizeof(unsigned char) * ID_MAX, 1, file);
	}

	//DATA読み込み
	data.chunkID = chunkID;
	fread(&data.chunkSize, sizeof(data.chunkSize), 1, file);

	//フォーマットセット
	format.wFormatTag = fmt.waveFormatType;
	format.nChannels = fmt.formatChannel;
	format.nSamplesPerSec = fmt.samplesPerSec;
	format.wBitsPerSample = fmt.bitsPerSample;
	format.nBlockAlign = format.nChannels * BYTE(format.wBitsPerSample);
	format.nAvgBytesPerSec = format.nSamplesPerSec * format.nBlockAlign;

	//読み込み最大数のセット
	readMax = data.chunkSize;

	//配列のメモリ確保
	for (auto& i : this->data)
	{
		i.resize(format.nAvgBytesPerSec);
	}

	return 0;
}

// 音声データの読み込み
int WAVE::Load(void)
{
	if (file == nullptr || end == true)
	{
		return -1;
	}

	for (unsigned int i = 0; i < format.nAvgBytesPerSec; ++i)
	{
		if (read <= readMax)
		{
			fread(&data[index][i], sizeof(char), 1, file);
			++read;
		}
		else
		{
			data[index][i] = 0;
			if (i + 1 >= format.nAvgBytesPerSec)
			{
				end = true;
				fclose(file);
			}
		}
	}

	//インデックスの更新
	++index;
	index %= DATA_MAX;

	return 0;
}
