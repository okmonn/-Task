#pragma once
#include <string>
#include <vector>
#include <map>
#include <thread>
#include <memory>

struct _iobuf;
typedef _iobuf FILE;

struct IXAudio2SourceVoice;

class XAudio2;
class VoiceCallback;

class Wave
{
public:
	// コンストラクタ
	Wave();
	Wave(const std::string& fileName);
	// デストラクタ
	~Wave();

	// 代入
	void operator=(const Wave& w);

	// 読み込み
	int Load(const std::string& fileName);

	// 再生
	long Play(const bool& loop = false);

	// 停止
	long Stop(void);

	// 波形データの削除
	void Delete(void);

	// チャンネル数の取得
	int GetChannel(void) const {
		return channel;
	}
	// サンプリング周波数の取得
	int GetSample(void) const {
		return sample;
	}
	// 量子化ビット数の取得
	int GetBit(void) const {
		return bit;
	}
	// ビットレートの取得
	int GetByte(void) const {
		return byte;
	}
	// 波形データが終わりに到達したか確認
	bool GetArrival(void) const {
		return arrival;
	}

private:
	// ソースボイスの生成
	long Create(void);

	// 波形をすべて読み込み終わってない場合の処理
	void Loading(void);

	// 波形をすべて読み込み終わっている場合の処理
	void Loaded(void);

	// 非同期
	void Stream(void);


	// XAudio2
	XAudio2& audio;

	// コールバック
	std::unique_ptr<VoiceCallback>back;

	// ソースボイス
	IXAudio2SourceVoice* voice;

	// ファイル
	FILE* file;

	// チャンネル数
	int channel;

	// サンプリング周波数
	int sample;

	// 量子化ビット数
	int bit;

	// ビットレート
	int byte;

	// 読み込み配列のインデックス
	unsigned int index;

	// 波形データが終わりに到達したかのフラグ
	bool arrival;

	// スレッドフラグ
	bool threadFlag;

	// 波形データ
	std::map<int, std::vector<float>>data;

	// スレッド
	std::thread th;

	// 関数ポインタ
	void (Wave::*func)(void);
};
