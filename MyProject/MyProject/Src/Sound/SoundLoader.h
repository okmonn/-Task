#pragma once
#include <string>
#include <vector>
#include <map>
#include <thread>

struct _iobuf;
typedef _iobuf FILE;

class SoundLoader
{
	// 波形情報
	struct Sound {
		//ファイル
		FILE* file;
		//チャンネル
		int channel;
		//サンプリング周波数
		int sample;
		//ビット
		int bit;
		//ビットレート
		int byte;
		//波形データ
		std::map<unsigned int, std::vector<float>>data;
	};

public:
	// コンストラクタ
	SoundLoader();
	// デストラクタ
	~SoundLoader();

	// 読み込み
	int Load(const std::string& fileName);

	// チャンネル数の取得
	int GetChannel(const std::string& fileName) {
		return sound[fileName].channel;
	}
	// サンプリング周波数
	int GetSample(const std::string& fileName) {
		return sound[fileName].sample;
	}
	// 量子化ビット数の取得
	int GetBit(const std::string& fileName) {
		return sound[fileName].bit;
	}
	// ビットレートの取得
	int GetByte(const std::string& fileName) {
		return sound[fileName].byte;
	}
	// 波形データの取得
	std::map<unsigned int, std::vector<float>>* GetData(const std::string& fileName) {
		return &sound[fileName].data;
	}

private:
	// 波形の読み込み
	void LoadWave(const std::string& fileName);


	// スレッドフラグ
	bool threadFlag;

	// 波形情報
	std::map<std::string, Sound>sound;

	// 読み込みスレッド
	std::vector<std::thread>th;
};
