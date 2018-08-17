#pragma once
#include "NoteNomber.h"
#include <Windows.h>
#include <mmsystem.h>
#include <vector>
#include <string>

class MIDI_IN
{
public:
	// コンストラクタ
	MIDI_IN();
	MIDI_IN(UINT index);
	// デストラクタ
	~MIDI_IN();

	// MIDI入力デバイスのオープン
	bool Open(UINT index = 0);

	// 接続デバイス数の取得
	UINT GetDevNum(void) const {
		return devNum;
	}

private:
	// エラーメッセージ
	void Error(std::string m);

	// 接続MIDIデバイスの数を取得
	void SearchDevNum(void);

	// 接続済みのMIDIデバイスの性能を格納
	void SetMidiCaps(void);

	// コールバック関数
	static void CALLBACK MidiInProc(HMIDIIN handle, UINT msg,
		DWORD instance, DWORD param1, DWORD param2);

	// SysExのバッファ登録
	bool SetSysExBuffer(void);

	// 指定したMIDIデバイスでの入力の開始
	bool Start(void);

	// 指定したMIDIデバイスでの入力の終了
	bool Stop(void);

	// 指定したMIDIデバイスでの入力の停止
	bool MidiReset(void);

	// SysExバッファのクリーンアップ
	bool ClearHdr(void);

	// MIDI入力デバイスのクローズ
	bool Close(void);

	// リセット
	void Reset(void);


	// 参照結果
	MMRESULT result;

	// MIDIデバイスのハンドル
	HMIDIIN handle;

	// MIDI入力デバイスの数
	UINT devNum;

	// システムエクスクルーシブ格納構造体
	MIDIHDR header;

	//MIDI入力デバイスの性能格納構造体
	std::vector<MIDIINCAPS>caps;

	// システムエクスクルーシブ格納構造体のアドレス
	static MIDIHDR* hdr;

	// ステータスバイト
	static UCHAR state;

	// ノート番号
	static UCHAR note;

	// ベロシティー
	static UCHAR vel;
};

class MIDI_OUT
{
public:
	// コンストラクタ
	MIDI_OUT();
	MIDI_OUT(UINT index);
	// デストラクタ
	~MIDI_OUT();

	// MIDI入力デバイスのオープン
	bool Open(UINT index = 0);

	// 音色のノートオン
	bool TimbreOn(DWORD no, DWORD vel = 0x007f0000);

	// ノートオン
	bool On(DWORD no, DWORD vel = 0x007f0000, DWORD change = AcousticGrandPiano, DWORD channel = Channel1);

	// ノートオフ
	bool Off(DWORD no);

	// 接続デバイス数の取得
	UINT GetDevNum(void) const {
		return devNum;
	}

private:
	// エラーメッセージ
	void Error(std::string m);

	// 接続出力デバイスの数を取得
	void SearchDevNum(void);

	// 接続済みの出力デバイスの性能を格納
	void SetMidiCaps(void);

	// SysExメッセージの送信
	bool SendSysEx(void);

	// プログラムチェンジ
	bool ProgramChange(DWORD no, DWORD channel = Channel1);

	// 指定したMIDIデバイスでの出力の停止
	bool MidiReset(void);

	// SysExバッファのクリーンアップ
	bool ClearHdr(void);

	// MIDI出力デバイスのクローズ
	bool Close(void);


	// 参照結果
	MMRESULT result;

	// ハンドル
	HMIDIOUT handle;

	// MIDI出力デバイスの数
	UINT devNum;

	// システムエクスクルーシブ格納構造体
	MIDIHDR header;

	//MIDI出力デバイスの性能格納構造体
	std::vector<MIDIOUTCAPS>caps;
};
