#pragma once
#include <string>
#include <vector>
#include <map>
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
	// デストラクタ
	~Wave();

	// 読み込み
	int Load(const std::string& fileName);

private:
	// ソースボイスの生成
	long Create(void);

	// 波形をすべて読み込み終わってない場合の処理
	void Loading(void);

	// 波形をすべて読み込み終わっている場合の処理
	void Loaded(void);

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

	// 波形の開始位置
	int start;

	// 読み込み配列のインデックス
	int index;

	// 波形データ
	std::map<int, std::vector<float>>data;

	// 関数ポインタ
	void (Wave::*func)(void);
};
