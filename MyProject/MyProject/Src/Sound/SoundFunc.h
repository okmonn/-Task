#pragma once
#include <string>
#include <vector>

struct _iobuf;
typedef _iobuf FILE;
struct RIFF;
struct FMT;
struct DATA;

namespace sound
{
	// RIFFチャンクの読み込み
	// 失敗時はdebug出力と、fclose実行済み
	int LoadRIFF(RIFF& riff, FILE* file);

	// FMTチャンクの読み込み
	// 失敗時はdebug出力と、fclose実行済み
	int LoadFMT(FMT& fmt, FILE* file);

	// DATAチャンクの読み込み
	// 失敗時はdebug出力と、fclose実行済み
	int LoadDATA(DATA& data, FILE* file);

	// 波形データ モノラル・8ビット
	// 正規化実装
	int LoadMono8(std::vector<float>&data, FILE* file);

	// 波形データ　モノラル・16ビット
	// 正規化実装
	int LoadMono16(std::vector<float>&data, FILE* file);

	// 波形データ　モノラル
	int LoadMono(std::vector<float>&data, int bit, FILE* file);

	// 波形データ　ステレオ・8ビット
	// 正規化実装
	int LoadStereo8(std::vector<float>&data, FILE* file);

	// 波形データ　ステレオ・16ビット
	// 正規化実装
	int LoadStereo16(std::vector<float>&data, FILE* file);

	// 波形データ　ステレオ
	int LoadStereo(std::vector<float>&data, int bit, FILE* file);

	// ログ関数
	int Log(const int& bottom, const int& value);

	// 高速フーリエ変換
	void FFT(const std::vector<float>& waveData, std::vector<float>& real, std::vector<float>& imag, const int& size);

	// ディレイ
	void Delay(std::vector<float>& waveData, const float& attenuation, const float& delayTime, const int& loop, const int& sample = 44100);

	// ディストーション
	void Distortion(std::vector<float>& waveData, const float& gain, const float& level);

	// リミッター
	void Limiter(std::vector<float>& waveData);
}