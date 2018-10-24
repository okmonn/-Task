#pragma once
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <thread>

class SoundLoader
{
	// サウンドデータ
	struct Data {
		//チャンネル数
		int channel;
		//サンプリング周波数
		int sample;
		//量子化ビット数
		int bit;
		//ビットレート
		int byte;
		//ファイル
		FILE* file;
		//波形データ
		std::shared_ptr<std::map<int, std::vector<float>>>wave;
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
		return snd[fileName].channel;
	}
	// サンプリング周波数の取得
	int GetSample(const std::string& fileName) {
		return snd[fileName].sample;
	}
	// 量子化ビット数の取得
	int GetBit(const std::string& fileName) {
		return snd[fileName].bit;
	}
	// ビットレートの取得
	int GetByte(const std::string& fileName) {
		return snd[fileName].byte;
	}
	// 波形データの取得
	std::shared_ptr<std::map<int, std::vector<float>>> GetData(const std::string& fileName) {
		return snd[fileName].wave;
	}

private:
	// 波形データの読み込み
	void LoadWave(const std::string& fileName);


	// スレッドフラグ
	bool threadFlag;

	// スレッド
	std::vector<std::thread>th;

	// サウンドデータ
	std::map<std::string, Data>snd;
};
