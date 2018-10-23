#pragma once
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <thread>

struct IXAudio2SourceVoice;

class XAudio2;
class SoundLoader;

class Sound
{
	// サウンド
	struct Snd {
		//ソースボイス
		IXAudio2SourceVoice* voice;
		//波形データ
		std::map<unsigned int, std::vector<float>>*data;
		//読み込み番号
		int index;
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
	long Load(const std::string& fileName, int& i);

	// 再生
	long Play(int& i, const bool& loop = false);

	// 停止
	long Stop(int& i);

	// 最初の位置にリセット
	void ZeroPos(int& i);

private:
	// ソースボイスの生成
	long Create(int* i, const int& channel, const int& sample, const int& bit);

	// 非同期処理
	void Stream(int* i);


	// XAudio2
	XAudio2& audio;

	// サウンドローダー
	std::shared_ptr<SoundLoader>loader;

	// スレッドフラグ
	bool threadFlag;

	// 波形データ
	std::map<int*, Snd>snd;
};
