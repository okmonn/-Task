#pragma once

struct IXAudio2;
struct IXAudio2MasteringVoice;

class XAudio2
{
public:
	// デストラクタ
	~XAudio2();

	// インスタンス変数の取得
	static XAudio2& Get(void) {
		static XAudio2 instance;
		return instance;
	}

	// XAudio2の取得
	IXAudio2* GetAudio(void) const {
		return audio;
	}
	// マスタリングボイス
	IXAudio2MasteringVoice* GetMastering(void) const {
		return mastering;
	}

private:
	// コンストラクタ
	XAudio2();
	XAudio2(const XAudio2&) {
	}
	void operator=(const XAudio2&) {
	}

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
