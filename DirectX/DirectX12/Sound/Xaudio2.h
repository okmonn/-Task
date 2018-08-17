#pragma once
#include "WAVE.h"
#include <map>

class Xaudio2
{
public:
	// コンストラクタ
	Xaudio2();
	// デストラクタ
	~Xaudio2();

	// 読み込み
	HRESULT LoadWAVE(const std::string& fileName, WAVE& wave);

	// ソースボイスの消去
	void Delete(WAVE& wave);

private:
	// 初期化
	HRESULT Init(void);

	// XAudio2の生成
	HRESULT CreateAudio(void);

	// デバッグのセット
	void SetDebug(void);

	// マスターボイスの生成
	HRESULT CreateMaster(void);


	// 参照結果
	HRESULT result;

	// XAudio2
	IXAudio2 * audio;

	// マスターボイス
	IXAudio2MasteringVoice* master;

	// ソースボイス
	std::map<WAVE*, IXAudio2SourceVoice*>voice;
};

