#pragma once
#include <xaudio2.h>
#include <map>

class WAVE;

class Xaudio2
{
public:
	// コンストラクタ
	Xaudio2();
	// デストラクタ
	~Xaudio2();

	// 読み込み
	HRESULT LoadWAVE(UINT& index, const std::string& fileName);

	// 再生開始
	HRESULT Play(UINT& index);

	// 再生停止
	HRESULT Stop(UINT& index);

	// ソースボイスの消去
	void Delete(UINT& index);

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

	// WAVEデータ
	std::map<UINT*, WAVE>wave;

	// ソースボイス
	std::map<WAVE*, IXAudio2SourceVoice*>voice;
};

