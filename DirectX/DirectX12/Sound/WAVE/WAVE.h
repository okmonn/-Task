#pragma once
#include <vector>
#include <string>

class WAVE
{
public:
	// コンストラクタ
	WAVE();
	// デストラクタ
	~WAVE();

	// 代入
	void operator=(const WAVE& wave);


	// チャンネル数
	int channel;

	// サンプリング周波数
	int sample;

	// 量子化ビット数
	int bit;

	// 時間
	int length;

	// 音楽データ
	//std::vector<double>data;

	// 音楽データ・モノラル
	std::vector<double>monaural;

	// 音楽データ・ステレオ
	std::vector <std::vector<double>>stereo;
};

// 名前空間指定
namespace sound {
	// 文字確認
	int CheckChar(unsigned char* data, int dataSize, const std::string& find);
	// 文字代入
	void SetChar(unsigned char* data, const std::string& set);

	// モノラル・8ビット
	void LoadMonaural8(WAVE& wave, FILE* file);
	// モノラル・16ビット
	void LoadMonaural16(WAVE& wave, FILE* file);
	// ステレオ・8ビット
	void LoadStereo8(WAVE& wave, FILE* file);
	// ステレオ・16ビット
	void LoadStereo16(WAVE& wave, FILE* file);
	// WAVEの読み込み
	int LoadWave(const std::string& fileName, WAVE& wave);

	// モノラル・8ビット
	void WriteMonaural8(WAVE& wave, FILE* file);
	// モノラル・16ビット
	void WriteMonaural16(WAVE& wave, FILE* file);
	// ステレオ・8ビット
	void WriteStereo8(WAVE& wave, FILE* file);
	// ステレオ・16ビット
	void WriteStereo16(WAVE& wave, FILE* file);
	// WAVEの書き込み
	int WriteWave(const std::string& fileName, WAVE& wave);

	// サイン波
	void SineWave(int sample, std::vector<double>&data, const double& amplitude, const double& period);
	// サイン波・のこぎり波
	void SineSawtoothWave(int sample, std::vector<double>& data, int count, const double& amplitude, const double& period);
	// サイン波・短形波
	void SineShortformWave(int sample, std::vector<double>& data, int count, const double& amplitude, const double& period);

	// コサイン波
	void CosineWave(int sample, std::vector<double>& data, const double& amplitude, const double& period);
	// コサイン波・のこぎり波
	void CosineSawtoothWave(int sample, std::vector<double>& data, int count, const double& amplitude, const double& period);
	// コサイン波・短形波
	void CosineShortformWave(int sample, std::vector<double>& data, int count, const double& amplitude, const double& period);

	// 対数
	int Log(int i, int n);
	// 累乗
	int Pow(int i, int n);
	
	// 窓関数
	std::vector<double> HanningWindow(int size);
	// 離散フーリエ変換
	void DFT(int size, std::vector<double>&real, std::vector<double>&imag);
	// 逆離散フーリエ変換
	void IDFT(int size, std::vector<double>&real, std::vector<double>&imag);
	/* 高速フーリエ変換・sizeは2の累乗で指定 */
	void FFT(int size, std::vector<double>&real, std::vector<double>&imag, int log = 2, int pow = 2);
	/* 逆高速フーリエ変換・sizeは2の累乗で指定 */
	void IFFT(int size, std::vector<double>&real, std::vector<double>&imag, int log = 2, int pow = 2);

	/* ディレイ・遅延時間を短くして繰り返しを多くするとリバーブ効果 */
	void Delay(int sample, std::vector<double>&data, double attenuation, double delayTime, int loopCnt = 2, int startPos = 0);

	// ディストーション
	void Distortion(std::vector<double>& data, double amplification, double level = 0.5);

	// オーバードライブ
	void OverDrive(std::vector<double>& data, double amplification, double level = 0.5);

	// ファズ
	void Fuzz(std::vector<double>&data, double amplification, double level = 0.5);

	// デバッグ描画
	void Debug(int max, double data);
}