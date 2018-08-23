#pragma once
#include <Windows.h>
#include <vector>
#include <string>

class MIDI_IN
{
public:
	// コンストラクタ
	MIDI_IN();
	// デストラクタ
	~MIDI_IN();

	// 接続MIDIデバイスの数を取得
	UINT GetDevNum(void) const {
		return devNum;
	}

private:
	// MIDIメッセージの確認
	static void MimData(DWORD param1, DWORD param2);

	// コールバック関数
	static void CALLBACK MidiInProc(HMIDIIN handle, UINT msg,
		DWORD instance, DWORD param1, DWORD param2);

	// エラーメッセージ
	void Error(const std::string& m);

	// 接続MIDIデバイスの性能を格納
	void SetMidiCaps(void);

	// MIDI入力デバイスのオープン
	HRESULT Open(UINT index = 0);

	// システムエクスクルーシブの追加
	HRESULT Prepare(void);

	// MIDIデバイスの入力開始
	HRESULT Start(void);

	// MIDIデバイスの入力停止
	HRESULT Stop(void);

	// MIDIデバイスのリセット
	HRESULT Reset(void);

	// システムエクスクルーシブのクリーンアップ
	HRESULT UnPrepare(void);

	// MIDIデバイスの入力終了
	HRESULT Close(void);


	// 参照結果
	HRESULT result;

	// MIDIデバイスのハンドル
	HMIDIIN handle;

	// MIDI入力デバイスの数
	UINT devNum;

	// システムエクスクルーシブ格納構造体
	MIDIHDR header;

	//MIDI入力デバイスの性能格納構造体
	std::vector<MIDIINCAPS>caps;

	// ステータスバイト
	static UCHAR state;

	// データバイト1
	static UCHAR data1;

	// データバイト2
	static UCHAR data2;
};
