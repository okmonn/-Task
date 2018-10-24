#pragma once
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <memory>

struct IXAudio2SourceVoice;

class XAudio2;
class SoundLoader;

class Sound
{
	// サウンドデータ
	struct Snd {
		//ソースボイス
		IXAudio2SourceVoice* voice;
		//波形データ
		std::weak_ptr<std::map<int, std::vector<float>>>wave;
		//配列番号
		unsigned int index;
		//ループフラグ
		bool loop;
		//スレッド
		std::thread th;
	};

public:
	// コンストラクタ
	Sound();
	// デストラクタ
	~Sound();

	// 読み込み
	void Load(const std::string& fileName, int& i);

	// 再生
	long Play(int& i, const bool& loop = false);

	// 停止
	long Stop(int& i);

private:
	// ソースボイスの生成
	long Create(int* i, const int channel, const int sample, const int bit);

	// 非同期
	void Stream(int* i);


	// XAudio2
	XAudio2& audio;

	// サウンドローダー
	std::shared_ptr<SoundLoader>loader;

	// スレッドフラグ
	bool threadFlag;

	// サウンドデータ
	std::map<int*, Snd>snd;
};
