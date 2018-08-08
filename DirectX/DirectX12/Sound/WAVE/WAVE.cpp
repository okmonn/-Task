#define _USE_MATH_DEFINES
#include "WAVE.h"
#include <iostream>
#include <tchar.h>
#include <math.h>

// チャンクIDサイズの最大
#define ID_MAX 4

// オーバーフローの防止
#define OVERFLLOW 32768.0

// バイト変換
#define BYTE(X) (X) / 8

// Hz変換
#define Hz(kHz) (kHz) * 1000

// kHz変換
#define kHz(Hz) (Hz) / 1000

// 円周率
#define PI 3.14159264

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

// 8ビットステレオデータ
struct Sample8 {
	//左
	unsigned char left;
	//右
	unsigned char right;
};

// 16ビットステレオデータ
struct Sample16 {
	//左
	short left;
	//右
	short right;
};

// コンストラクタ
WAVE::WAVE()
{
}

// デストラクタ
WAVE::~WAVE()
{
}

// 代入
void WAVE::operator=(const WAVE & wave)
{
	channel  = wave.channel;
	sample   = wave.sample;
	bit      = wave.bit;
	length   = wave.length;
	monaural = wave.monaural;
	stereo   = wave.stereo;
}

// 文字確認
int sound::CheckChar(unsigned char * data, int dataSize, const std::string & find)
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

// 文字代入
void sound::SetChar(unsigned char * data, const std::string & set)
{
	for (unsigned int i = 0; i < set.size(); ++i)
	{
		data[i] = set[i];
	}
}

// モノラル・8ビット
void sound::LoadMonaural8(WAVE & wave, FILE * file)
{
	wave.monaural.resize(wave.length);

	unsigned char data = 0;
	for (int i = 0; i < wave.length; ++i)
	{
		fread(&data, sizeof(data), 1, file);
		wave.monaural[i] = static_cast<double>(data);
	}
}

// モノラル・16ビット
void sound::LoadMonaural16(WAVE & wave, FILE * file)
{
	wave.monaural.resize(wave.length);

	short data = 0;
	for (int i = 0; i < wave.length; ++i)
	{
		fread(&data, sizeof(data), 1, file);
		/* 音データを-1以上1未満の範囲に正規化する */
		wave.monaural[i] = static_cast<double>(data) / OVERFLLOW;
	}
}

// ステレオ・8ビット
void sound::LoadStereo8(WAVE & wave, FILE * file)
{
	wave.stereo.resize(wave.channel);
	for (auto& w : wave.stereo)
	{
		w.resize(wave.length);
	}

	Sample8 data = {};
	for (int i = 0; i < wave.length; ++i)
	{
		fread(&data, sizeof(data), 1, file);
		wave.stereo[0][i] = static_cast<double>(data.left);
		wave.stereo[1][i] = static_cast<double>(data.right);
	}
}

// ステレオ・16ビット
void sound::LoadStereo16(WAVE & wave, FILE * file)
{
	wave.stereo.resize(wave.channel);
	for (auto& w : wave.stereo)
	{
		w.resize(wave.length);
	}

	Sample16 data = {};
	for (int i = 0; i < wave.length; ++i)
	{
		fread(&data, sizeof(data), 1, file);
		/* 音データを-1以上1未満の範囲に正規化する */
		wave.stereo[0][i] = static_cast<double>(data.left) / OVERFLLOW;
		wave.stereo[1][i] = static_cast<double>(data.right) / OVERFLLOW;
	}
}

// WAVEの読み込み
int sound::LoadWave(const std::string & fileName, WAVE & wave)
{
	FILE* file = nullptr;

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

	wave.channel = fmt.formatChannel;
	wave.sample = fmt.samplesPerSec;
	wave.bit = fmt.bitsPerSample;
	wave.length = data.chunkSize / fmt.blockSize;

	//モノラル
	if (fmt.formatChannel == 1)
	{
		//8ビット
		if (fmt.bitsPerSample == 8)
		{
			LoadMonaural8(wave, file);
		}
		//16ビット
		else if (fmt.bitsPerSample == 16)
		{
			LoadMonaural16(wave, file);
		}
	}
	//ステレオ
	else if (fmt.formatChannel == 2)
	{
		//8ビット
		if (fmt.bitsPerSample == 8)
		{
			LoadStereo8(wave, file);
		}
		//16ビット
		else if (fmt.bitsPerSample == 16)
		{
			LoadStereo16(wave, file);
		}
	}

	//ファイルクローズ
	fclose(file);

	return 0;
}

// モノラル・8ビット
void sound::WriteMonaural8(WAVE & wave, FILE * file)
{
	unsigned char data = 0;
	for (int i = 0; i < wave.length; ++i)
	{
		data = static_cast<unsigned char>(wave.monaural[i]);
		fwrite(&data, sizeof(data), 1, file);
	}
}

// モノラル・16ビット
void sound::WriteMonaural16(WAVE & wave, FILE * file)
{
	double dummy = 0.0;
	short data = 0;
	for (int i = 0; i < wave.length; ++i)
	{
		dummy = (wave.monaural[i] + 1.0) / static_cast<double>(BYTE(wave.bit)) * OVERFLLOW * 2.0;

		/* クリッピング */
		if (dummy > OVERFLLOW * 2.0 - 1.0)
		{
			dummy = OVERFLLOW * 2.0 - 1.0;
		}
		else if (dummy < 0.0)
		{
			dummy = 0.0;
		}

		data = static_cast<short>(dummy + 0.5) - static_cast<short>(OVERFLLOW);

		fwrite(&data, sizeof(data), 1, file);
	}
}

// ステレオ・8ビット
void sound::WriteStereo8(WAVE & wave, FILE * file)
{
	unsigned char data = 0;
	for (int i = 0; i < wave.length; ++i)
	{
		for (unsigned int p = 0; p < wave.stereo.size(); ++p)
		{
			data = static_cast<unsigned char>(wave.stereo[p][i]);
			fwrite(&data, sizeof(data), 1, file);
		}
	}
}

// ステレオ・16ビット
void sound::WriteStereo16(WAVE & wave, FILE * file)
{
	double dummy = 0.0;
	short data = 0;
	for (int i = 0; i < wave.length; ++i)
	{
		for (unsigned int p = 0; p < wave.stereo.size(); ++p)
		{
			dummy = (wave.stereo[p][i] + 1.0) / static_cast<double>(BYTE(wave.bit)) * OVERFLLOW * 2.0;

			/* クリッピング */
			if (dummy > OVERFLLOW * 2.0 - 1.0)
			{
				dummy = OVERFLLOW * 2.0 - 1.0;
			}
			else if (dummy < 0.0)
			{
				dummy = 0.0;
			}

			data = static_cast<short>(dummy + 0.5) - static_cast<short>(OVERFLLOW);

			fwrite(&data, sizeof(data), 1, file);
		}
	}
}

// WAVEの書き込み
int sound::WriteWave(const std::string & fileName, WAVE & wave)
{
	FILE* file = nullptr;

	//ファイルオープン
	if (fopen_s(&file, fileName.c_str(), "wb") != 0)
	{
		return -1;
	}

	//チャンク宣言
	RIFF riff = {};
	FMT fmt = {};
	DATA data = {};

	//RIFFF書き込み
	SetChar(riff.chunkID, "RIFF");
	fwrite(&riff.chunkID[0], sizeof(riff.chunkID), 1, file);
	riff.chunkSize = sizeof(RIFF) + sizeof(FMT) + wave.length * (BYTE(wave.bit) * wave.channel);
	fwrite(&riff.chunkSize, sizeof(riff.chunkSize), 1, file);
	SetChar(riff.chunkFormatType, "WAVE");
	fwrite(&riff.chunkFormatType[0], sizeof(riff.chunkFormatType), 1, file);

	//FMT書き込み
	SetChar(fmt.chunkID, "fmt ");
	fwrite(&fmt.chunkID[0], sizeof(fmt.chunkID), 1, file);
	fmt.chunkSize = sizeof(FMT) - sizeof(fmt.chunkID) - sizeof(fmt.chunkSize);
	fwrite(&fmt.chunkSize, sizeof(fmt.chunkSize), 1, file);
	fmt.waveFormatType = 1;
	fwrite(&fmt.waveFormatType, sizeof(fmt.waveFormatType), 1, file);
	fmt.formatChannel = wave.channel;
	fwrite(&fmt.formatChannel, sizeof(fmt.formatChannel), 1, file);
	fmt.samplesPerSec = wave.sample;
	fwrite(&fmt.samplesPerSec, sizeof(fmt.samplesPerSec), 1, file);
	fmt.bytesPerSec = wave.sample * BYTE(wave.bit) * wave.channel;
	fwrite(&fmt.bytesPerSec, sizeof(fmt.bytesPerSec), 1, file);
	fmt.blockSize = BYTE(wave.bit) * wave.channel;
	fwrite(&fmt.blockSize, sizeof(fmt.blockSize), 1, file);
	fmt.bitsPerSample = wave.bit;
	fwrite(&fmt.bitsPerSample, sizeof(fmt.bitsPerSample), 1, file);

	//DATA書き込み
	data.chunkID = "data";
	fwrite(&data.chunkID[0], sizeof(unsigned char) * ID_MAX, 1, file);
	data.chunkSize = wave.length * BYTE(fmt.bitsPerSample) * fmt.formatChannel;
	fwrite(&data.chunkSize, sizeof(data.chunkSize), 1, file);

	//モノラル
	if (fmt.formatChannel == 1)
	{
		//8ビット
		if (fmt.bitsPerSample == 8)
		{
			WriteMonaural8(wave, file);
		}
		//16ビット
		else if (fmt.bitsPerSample == 16)
		{
			WriteMonaural16(wave, file);
		}
	}
	//ステレオ
	else if (fmt.formatChannel == 2)
	{
		//8ビット
		if (fmt.bitsPerSample == 8)
		{
			WriteStereo8(wave, file);
		}
		//16ビット
		else if (fmt.bitsPerSample == 16)
		{
			WriteStereo16(wave, file);
		}
	}

	//ファイルクローズ
	fclose(file);

	return 0;
}

// サイン波
void sound::SineWave(int sample, std::vector<double>&data, const double& amplitude, const double& period)
{
	/* 公式 */
	// 基本周波数
	/* 1.0 / (サンプル数 / サンプリング周波数) */
	// サイン波
	/* 振幅 * sin((2.0 * 円周率 * 基本周波数 * n) / サンプリング周波数)) */

	for (unsigned int i = 0; i < data.size(); ++i)
	{
		data[i] = amplitude * std::sin(2.0 * M_PI * (1.0 / (period / static_cast<double>(sample))) * i / static_cast<double>(sample));
	}
}

// サイン波・のこぎり波
void sound::SineSawtoothWave(int sample, std::vector<double>& data, int count, const double& amplitude, const double& period)
{
	/* 考え方 */
	/*サイン波を加算（重ね合わせて）していく*/

	for (unsigned int i = 0; i < data.size(); ++i)
	{
		for (int n = 1; n <= count; ++n)
		{
			data[i] += amplitude * std::sin(2.0 * M_PI * (1.0 / (period / static_cast<double>(sample))) * i * n / static_cast<double>(sample));
		}
	}
}

// サイン波・短形波
void sound::SineShortformWave(int sample, std::vector<double>& data, int count, const double& amplitude, const double& period)
{
	/* 考え方 */
	/*サイン波を奇数倍のものを加算（重ね合わせて）していく*/

	for (unsigned int i = 0; i < data.size(); ++i)
	{
		for (int n = 1; n <= count; ++n)
		{
			if (n % 2 == 0)
			{
				continue;
			}
			data[i] += amplitude * std::sin(2.0 * M_PI * (1.0 / (period / static_cast<double>(sample))) * i * n / static_cast<double>(sample));
		}
	}
}

// コサイン波
void sound::CosineWave(int sample, std::vector<double>& data, const double& amplitude, const double& period)
{
	/* 公式 */
	// 基本周波数
	/* 1.0 / (サンプル数 / サンプリング周波数) */
	// コサイン波
	/* 振幅 * cos((2.0 * 円周率 * 基本周波数 * n) / サンプリング周波数)) */

	for (unsigned int i = 0; i < data.size(); ++i)
	{
		data[i] = amplitude * std::cos(2.0 * M_PI * (1.0 / (period / static_cast<double>(sample))) * i / static_cast<double>(sample));
	}
}

// コサイン波・のこぎり波
void sound::CosineSawtoothWave(int sample, std::vector<double>& data, int count, const double& amplitude, const double& period)
{
	/* 考え方 */
	/*コサイン波を加算（重ね合わせて）していく*/

	for (unsigned int i = 0; i < data.size(); ++i)
	{
		for (int n = 1; n <= count; ++n)
		{
			data[i] += amplitude / n * std::cos(2.0 * M_PI * (1.0 / (period / static_cast<double>(sample))) * i * n / static_cast<double>(sample));
		}
	}
}

// コサイン波・短形波
void sound::CosineShortformWave(int sample, std::vector<double>& data, int count, const double& amplitude, const double& period)
{
	/* 考え方 */
	/*コサイン波を奇数倍のものを加算（重ね合わせて）していく*/

	for (unsigned int i = 0; i < data.size(); ++i)
	{
		for (int n = 1; n <= count; ++n)
		{
			if (n % 2 == 0)
			{
				continue;
			}
			data[i] += amplitude / n * std::cos(2.0 * M_PI * (1.0 / (period / static_cast<double>(sample))) * i * n / static_cast<double>(sample));
		}
	}
}

// 対数
int sound::Log(int i, int n)
{
	return static_cast<int>(std::log(n) / std::log(i));
}

// 累乗
int sound::Pow(int i, int n)
{
	return (n == 0) ? 1 : i << (n - 1);
}

// 窓関数
std::vector<double> sound::HanningWindow(int size)
{
	/*波形に釣鐘状の窓関数
	　波形の繰り返しに伴う端部の不連続点をあいまいにする*/
	std::vector<double>data;
	data.resize(size);

	if (size % 2 == 0)
	{
		for (int i = 0; i < size; ++i)
		{
			data[i] = 0.5 - 0.5 * std::cos(2.0 * M_PI * i / size);
			if (data[i] == 0.0)
			{
				data[i] = 1.0;
			}
		}
	}
	else
	{
		for (int i = 0; i < size; ++i)
		{
			data[i] = 0.5 - 0.5 * std::cos(2.0 * M_PI * (i + 0.5) / size);
			if (data[i] == 0.0)
			{
				data[i] = 1.0;
			}
		}
	}

	return data;
}

// 離散フーリエ変換
void sound::DFT(int size, std::vector<double>&real, std::vector<double>&imag)
{
	//実数
	double w_r;
	auto o_r = real;
	//虚数
	double w_i;
	auto o_i = imag;

	//ハミングデータ
	std::vector<double>hanning = HanningWindow(size);

	for (int i = 0; i < size; ++i)
	{
		real[i] = 0.0;
		imag[i] = 0.0;
		for (int n = 0; n < size; n++)
		{
			w_r = std::cos(2.0 * M_PI * i * n / size);
			w_i = -(std::sin(2.0 * M_PI * i * n / size));

			//実数部
			real[i] += w_r * (o_r[n] * hanning[n]) - w_i * o_i[n];
			//虚数部
			imag[i] += w_r * o_i[n] + w_i * (o_r[n] * hanning[n]);
		}
	}
}

// 逆離散フーリエ変換
void sound::IDFT(int size, std::vector<double>& real, std::vector<double>& imag)
{
	//実数
	double w_r;
	auto o_r = real;
	//虚数
	double w_i;
	auto o_i = imag;

	//ハミングデータ
	std::vector<double>hanning = HanningWindow(size);

	for (int i = 0; i < size; ++i)
	{
		real[i] = 0.0;
		imag[i] = 0.0;
		for (int n = 0; n < size; n++)
		{
			w_r = std::cos(2.0 * M_PI * i * n / size);
			w_i = std::sin(2.0 * M_PI * i * n / size);
			//実数部
			real[i] += (w_r * (o_r[n]) - w_i * o_i[n]) / size;
			//虚数部
			imag[i] += (w_r * o_i[n] + w_i * (o_r[n])) / size;
		}

		real[i] /= hanning[i];
		imag[i] /= hanning[i];
	}
}

// 高速フーリエ変換
void sound::FFT(int size, std::vector<double>& real, std::vector<double>& imag, int log, int pow)
{
	//配列番号
	int index1 = 0;
	int index2 = 0;

	//配列テーブル
	std::vector<int>indexTbl;
	indexTbl.resize(size);

	//累乗
	int p = 0;

	//変換データ
	double re[3];
	double im[3];

	//高速フーリエの段数
	int stage = Log(log, size);

	//バタフライ計算
	for (int s = 1; s <= stage; ++s)
	{
		for (int i = 0; i < Pow(pow, s - 1); ++i)
		{
			for (int n = 0; n < Pow(pow, stage - s); ++n)
			{
				index1 = Pow(pow, stage - s + 1) * i + n;
				index2 = Pow(pow, stage - s) + index1;

				p = Pow(pow, s - 1) * n;

				re[0] = real[index1];
				im[0] = imag[index1];
				re[1] = real[index2];
				im[1] = imag[index2];
				re[2] = std::cos((2.0 * M_PI * p) / size);
				im[2] = -(std::sin((2.0 * M_PI * p) / size));

				if (s < stage)
				{
					real[index1] = re[0] + re[1];
					imag[index1] = im[0] + im[1];
					real[index2] = (re[0] - re[1]) * re[2] - (im[0] - im[1]) * im[2];
					imag[index2] = (im[0] - im[1]) * re[2] + (re[0] - re[1]) * im[2];
				}
				else
				{
					real[index1] = re[0] + re[1];
					imag[index1] = im[0] + im[1];
					real[index2] = re[0] - re[1];
					imag[index2] = im[0] - im[1];
				}
			}

			//インデックス並び変えようのテーブル作成
			indexTbl[Pow(pow, s - 1) + i] = indexTbl[i] + Pow(pow, stage - s);
		}
	}

	//インデックスの並び替え
	double dr = 0.0;
	double di = 0.0;
	for (int i = 0; i < size; ++i)
	{
		if (indexTbl[i] > i)
		{
			dr = real[indexTbl[i]];
			di = imag[indexTbl[i]];
			real[indexTbl[i]] = real[i];
			imag[indexTbl[i]] = imag[i];
			real[i] = dr;
			imag[i] = di;
		}
	}
}

// 逆高速フーリエ変換
void sound::IFFT(int size, std::vector<double>& real, std::vector<double>& imag, int log, int pow)
{
	//配列番号
	int index1 = 0;
	int index2 = 0;

	//配列テーブル
	std::vector<int>indexTbl;
	indexTbl.resize(size);

	//累乗
	int p = 0;

	//変換データ
	//変換データ
	double re[3];
	double im[3];

	//高速フーリエの段数
	int stage = Log(log, size);

	//バタフライ計算
	for (int s = 1; s <= stage; ++s)
	{
		for (int i = 0; i < Pow(pow, s - 1); ++i)
		{
			for (int n = 0; n < Pow(pow, stage - s); ++n)
			{
				index1 = Pow(pow, stage - s + 1) * i + n;
				index2 = Pow(pow, stage - s) + index1;

				p = Pow(pow, s - 1) * n;

				re[0] = real[index1];
				im[0] = imag[index1];
				re[1] = real[index2];
				im[1] = imag[index2];
				re[2] = std::cos((2.0 * M_PI * p) / size);
				im[2] = std::sin((2.0 * M_PI * p) / size);

				if (s < stage)
				{
					real[index1] = re[0] + re[1];
					imag[index1] = im[0] + im[1];
					real[index2] = (re[0] - re[1]) * re[2] - (im[0] - im[1]) * im[2];
					imag[index2] = (im[0] - im[1]) * re[2] + (re[0] - re[1]) * im[2];
				}
				else
				{
					real[index1] = re[0] + re[1];
					imag[index1] = im[0] + im[1];
					real[index2] = re[0] - re[1];
					imag[index2] = im[0] - im[1];
				}
			}

			//インデックス並び変えようのテーブル作成
			indexTbl[Pow(pow, s - 1) + i] = indexTbl[i] + Pow(pow, stage - s);
		}
	}

	//インデックスの並び替え
	double dr = 0.0;
	double di = 0.0;
	for (int i = 0; i < size; ++i)
	{
		if (indexTbl[i] > i)
		{
			dr = real[indexTbl[i]];
			di = imag[indexTbl[i]];
			real[indexTbl[i]] = real[i];
			imag[indexTbl[i]] = imag[i];
			real[i] = dr;
			imag[i] = di;
		}

		//計算結果をサイズで割る
		real[i] /= size;
		imag[i] /= size;
	}
}

// ディレイ
void sound::Delay(int sample, std::vector<double>&data, double attenuation, double delayTime, int loopCnt, int startPos)
{
	//適応データ
	std::vector<double>wave;
	wave.resize(data.size() - startPos);

	//加算データ
	int d = 0;

	for (unsigned int i = 0; i < wave.size(); ++i)
	{
		wave[i] = data[startPos + i];

		for (int l = 1; l <= loopCnt; ++l)
		{
			d = (int)((double)i - (double)l * ((double)sample * delayTime));
			if (d >= 0)
			{
				wave[i] += std::pow(attenuation, static_cast<double>(l)) * data[startPos + d];
			}
		}

		// 入れ替え
		data[startPos + i] = wave[i];
	}
}

// ディストーション
void sound::Distortion(std::vector<double>& data, double amplification, double level)
{
	for (unsigned int i = 0; i < data.size(); ++i)
	{
		//音声の増幅
		data[i] *= amplification;

		//クリッピング
		if (data[i] > 1.0)
		{
			data[i] = 1.0;
		}
		else if (data[i] < -1.0)
		{
			data[i] = -1.0;
		}

		//音量の調節
		data[i] *= level;
	}
}

// オーバードライブ
void sound::OverDrive(std::vector<double>& data, double amplification, double level)
{
	for (unsigned int i = 0; i < data.size(); ++i)
	{
		data[i] *= amplification;
		
		//クリッピング
		if (data[i] >= 0.0)
		{
			data[i] = std::atan(data[i]) / (M_PI / 2.0);
		}
		else if (data[i] < -0.0)
		{
			data[i] = std::atan(data[i]) / (M_PI / 2.0) * 0.1;
		}

		//音量の調節
		data[i] *= level;
	}
}

// ファズ
void sound::Fuzz(std::vector<double>& data, double amplification, double level)
{
	for (unsigned int i = 0; i < data.size(); ++i)
	{
		//音データの全波整流
		if (data[i] < 0.0)
		{
			data[i] *= -1.0;
		}

		//クリッピング
		if (data[i] > 1.0)
		{
			data[i] = 1.0;
		}
		else if (data[i] < -1.0)
		{
			data[i] = -1.0;
		}

		//音量の調節
		data[i] *= level;
	}
}

// デバッグ描画
void sound::Debug(int max, double data)
{
	for (int i = -max; i <= max; ++i)
	{
		if (static_cast<int>(data) < 0)
		{
			if (i >= static_cast<int>(data) && i <= 0)
			{
				printf("-");
			}
			else
			{
				printf(" ");
			}
		}
		else if (static_cast<int>(data) > 0)
		{
			if (i <= static_cast<int>(data) && i >= 0)
			{
				printf("-");
			}
			else
			{
				printf(" ");
			}
		}
		else if (static_cast<int>(data) == 0)
		{
			if (i == 0)
			{
				printf("・");
			}
			else
			{
				printf(" ");
			}
		}
	}
	printf("\n");
}
