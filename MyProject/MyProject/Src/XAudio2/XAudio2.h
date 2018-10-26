#pragma once

struct IXAudio2;
struct IXAudio2MasteringVoice;

class XAudio2
{
public:
	// コンストラクタ
	XAudio2();
	// デストラクタ
	~XAudio2();

	// XAudio2の取得
	IXAudio2* GetAudio(void) const {
		return audio;
	}
	// マスタリングボイス
	IXAudio2MasteringVoice* GetMastering(void) const {
		return mastering;
	}

private:
	// COMの初期化
	long InitCom(void);

	// XAudio2の生成
	long CreateAudio(void);

	// マスタリングボイスの生成
	long CreateMastering(void);

	// 初期化
	void Init(void);


	// XAudio2
	IXAudio2* audio;

	// マスタリングボイス
	IXAudio2MasteringVoice* mastering;
};
