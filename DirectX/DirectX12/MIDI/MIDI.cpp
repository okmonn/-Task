#include "MIDI.h"
#pragma comment(lib, "winmm.lib")
#include <tchar.h>

BYTE GMSystemOn[] = { 0xf0, 0x7e, 0x7f, 0x09, 0x01, 0xf7 };

MIDIHDR* MIDI_IN::hdr = nullptr;
UCHAR MIDI_IN::state = 0;
UCHAR MIDI_IN::note = 0;
UCHAR MIDI_IN::vel = 0;

// バッファサイズ
#define BUFFER_SIZE 256

// コンストラクタ
MIDI_IN::MIDI_IN() 
	: result(MMSYSERR_NOERROR), handle(nullptr), devNum(0)
{
	Reset();

	header = {};

	SearchDevNum();
	SetMidiCaps();
}

// コンストラクタ
MIDI_IN::MIDI_IN(UINT index)
	: result(MMSYSERR_NOERROR), handle(nullptr), devNum(0)
{
	Reset();

	header = {};

	SearchDevNum();
	SetMidiCaps();
	Open(index);
}

// デストラクタ
MIDI_IN::~MIDI_IN()
{
	Stop();
	MidiReset();
	ClearHdr();
	Close();
	Reset();
}

// エラーメッセージ
void MIDI_IN::Error(std::string m)
{
	LPWSTR errmsg = 0;
	midiInGetErrorText(result, errmsg, sizeof(errmsg));
	printf(m.c_str());
	printf("：%ls\n", errmsg);
}

// 接続MIDIデバイスの数を取得
void MIDI_IN::SearchDevNum(void)
{
	devNum = midiInGetNumDevs();
}

// 接続済みのMIDIデバイスの性能を格納
void MIDI_IN::SetMidiCaps(void)
{
	caps.resize(devNum);
	for (UINT i = 0; i < devNum; ++i)
	{
		result = midiInGetDevCaps(i, &caps[i], sizeof(MIDIINCAPS));
		if (result != MMSYSERR_NOERROR)
		{
			Error("キャプチャー失敗");
		}
	}
}

// コールバック関数
void MIDI_IN::MidiInProc(HMIDIIN handle, UINT msg, DWORD instance, DWORD param1, DWORD param2)
{
	printf("MidiInProc: msg=%08X, p1=%08X, p2=%08X\n", msg, param1, param2);

	switch (msg)
	{
	case MIM_OPEN:
		printf("MIDI入力デバイスが開かれたときにこの値が設定されてコールバック関数が呼び出される\n");
		break;
	case MIM_CLOSE:
		printf("MIDI入力デバイスが閉じられたときにこの値が設定されてコールバック関数が呼び出される\n");
		break;
	case MIM_DATA:
		//ステータスバイト
		state = param1 & 0x000000ff;
		if (state == 0xfe)
		{
			printf("アクティブセンシング\n");
		}
		if (state == 0x90)
		{
			printf("ノートオンメッセージ\n");

			//ノート番号
			note = (param1 & 0x0000ff00) >> 8;

			//ベロシティー
			vel = static_cast<UCHAR>((param1 & 0x00ff0000) >> 16);
			if (vel == 0)
			{
				printf("押されてない\n");
			}

			printf("ノート番号：%u, ベロシティー：%u\n", note, vel);
		}

		printf("MIDIメッセージを受信したときにこの値が設定されてコールバック関数が呼び出される\n");
		break;
	case MIM_LONGDATA:
		hdr = (MIDIHDR*)param1;
		for (UINT i = 0; i < hdr->dwBytesRecorded; ++i)
		{
			//バイトごとに16進数で表示
			printf("%02X \n", (unsigned char)((hdr->lpData)[i]));
		}

		//再度バッファを準備して登録
		midiInPrepareHeader(handle, hdr, sizeof(MIDIHDR));
		midiInAddBuffer(handle, hdr, sizeof(MIDIHDR));

		printf("SysEx用バッファが埋まりアプリケーションに返されるときにこの値が設定されてコールバック関数が呼び出される\n");
		break;
	case MIM_ERROR:
		printf("無効なMIDIメッセージを受信したときにこの値が設定されてコールバック関数が呼び出される\n");
		break;
	case MIM_LONGERROR:
		printf("無効または不完全なSysExを受信したときにこの値が設定されてコールバック関数が呼び出される\n");
		break;
	case MIM_MOREDATA:
		printf("MIDIメッセージを受信したがアプリケーションがMIM_DATAを処理しきれていない場合にこの値が設定されてコールバック関数が呼び出される。これはOpen関数の呼び出し時にMIDI_IO_STATUSが指定された場合にのみ呼び出される\n");
		break;
	default:
		break;
	}
}

// MIDI入力デバイスのオープン
bool MIDI_IN::Open(UINT index)
{
	if (devNum <= index)
	{
		return false;
	}

	//オープン
	result = midiInOpen(&handle, index, (DWORD_PTR)MidiInProc, 0, CALLBACK_FUNCTION);
	if (result != MMSYSERR_NOERROR)
	{
		Error("MIDIデバイスのオープン失敗");
		return false;
	}

	if (SetSysExBuffer() == false)
	{
		return false;
	}

	if (Start() == false)
	{
		return  false;
	}

	return true;
}

// SysExのバッファ登録
bool MIDI_IN::SetSysExBuffer(void)
{
	header.lpData = (char*)malloc(BUFFER_SIZE);
	header.dwBufferLength = BUFFER_SIZE;
	header.dwFlags = 0;

	result = midiInPrepareHeader(handle, &header, sizeof(MIDIHDR));
	if (result != MMSYSERR_NOERROR)
	{
		Error("SysExのセット失敗");
		return false;
	}

	result = midiInAddBuffer(handle, &header, sizeof(MIDIHDR));
	if (result != MMSYSERR_NOERROR)
	{
		Error("SysExの追加失敗");
		return false;
	}

	return true;
}

// 指定したMIDIデバイスでの入力の開始
bool MIDI_IN::Start(void)
{
	result = midiInStart(handle);
	if (result != MMSYSERR_NOERROR)
	{
		Error("入力開始の失敗");
		return false;
	}

	return true;
}

// 指定したMIDIデバイスでの入力の終了
bool MIDI_IN::Stop(void)
{
	result = midiInStop(handle);
	if (result != MMSYSERR_NOERROR)
	{
		Error("入力終了の失敗");
		return false;
	}

	return true;
}

// 指定したMIDIデバイスでの入力の停止
bool MIDI_IN::MidiReset(void)
{
	result = midiInReset(handle);
	if (result != MMSYSERR_NOERROR)
	{
		Error("リセット失敗");
		return false;
	}

	return true;
}

// SysExバッファのクリーンアップ
bool MIDI_IN::ClearHdr(void)
{
	result = midiInUnprepareHeader(handle, &header, sizeof(MIDIHDR));
	if (result != MMSYSERR_NOERROR)
	{
		Error("SysExのクリーンアップ失敗");
		return false;
	}

	free(header.lpData);

	return true;
}

// MIDI入力デバイスのクローズ
bool MIDI_IN::Close(void)
{
	result = midiInClose(handle);
	if (result != MMSYSERR_NOERROR)
	{
		Error("クローズ失敗");
		return false;
	}

	return true;
}

// リセット
void MIDI_IN::Reset(void)
{
	caps.clear();
}


// コンストラクタ
MIDI_OUT::MIDI_OUT() 
	: result(MMSYSERR_NOERROR), handle(nullptr), devNum(0)
{
	header = {};

	SearchDevNum();
	SetMidiCaps();
}

// コンストラクタ
MIDI_OUT::MIDI_OUT(UINT index) 
	: result(MMSYSERR_NOERROR), handle(nullptr), devNum(0)
{
	header = {};

	SearchDevNum();
	SetMidiCaps();
	Open(index);
}

// デストラクタ
MIDI_OUT::~MIDI_OUT()
{
	MidiReset();
	ClearHdr();
	Close();
}

// エラーメッセージ
void MIDI_OUT::Error(std::string m)
{
	LPWSTR errmsg = 0;
	midiInGetErrorText(result, errmsg, sizeof(errmsg));
	printf(m.c_str());
	printf("：%ls\n", errmsg);
}

// 接続出力デバイスの数を取得
void MIDI_OUT::SearchDevNum(void)
{
	devNum = midiOutGetNumDevs();
}

// 接続済みの出力デバイスの性能を格納
void MIDI_OUT::SetMidiCaps(void)
{
	caps.resize(devNum);
	for (UINT i = 0; i < devNum; ++i)
	{
		result = midiOutGetDevCaps(i, &caps[i], sizeof(MIDIOUTCAPS));
		if (result != MMSYSERR_NOERROR)
		{
			Error("キャプチャー失敗");
		}
	}
}

// MIDI入力デバイスのオープン
bool MIDI_OUT::Open(UINT index)
{
	if (devNum <= index)
	{
		return false;
	}

	result = midiOutOpen(&handle, index, 0, 0, 0);
	if (result != MMSYSERR_NOERROR)
	{
		Error("オープン失敗");
		return false;
	}

	if (SendSysEx() == false)
	{
		return false;
	}

	return true;
}

// SysExメッセージの送信
bool MIDI_OUT::SendSysEx(void)
{
	header.lpData = (LPSTR)GMSystemOn;
	header.dwBufferLength = sizeof(GMSystemOn);
	header.dwFlags = 0;

	result = midiOutPrepareHeader(handle, &header, sizeof(MIDIHDR));
	if (result != MMSYSERR_NOERROR)
	{
		Error("SysExのセット失敗");
		return false;
	}

	result = midiOutLongMsg(handle, &header, sizeof(MIDIHDR));
	if (result != MMSYSERR_NOERROR)
	{
		Error("SysExの送信失敗");
		return false;
	}

	return true;
}

// プログラムチェンジ
bool MIDI_OUT::ProgramChange(DWORD no, DWORD channel)
{
	if (channel == 0x000000c9)
	{
		return false;
	}

	DWORD note = channel | no;
	result = midiOutShortMsg(handle, note);
	if (result != MMSYSERR_NOERROR)
	{
		Error("プログラムチェンジ失敗");
		return false;
	}
	return true;
}

// 音色のノートオン
bool MIDI_OUT::TimbreOn(DWORD no, DWORD vel)
{
	DWORD note = 0x00000099 | no | vel;
	result = midiOutShortMsg(handle, note);
	if (result != MMSYSERR_NOERROR)
	{
		Error("音色のノートオン失敗");
		return false;
	}

	return true;
}

// ノートオン
bool MIDI_OUT::On(DWORD no, DWORD vel, DWORD change, DWORD channel)
{
	if (ProgramChange(change, channel) == false)
	{
		return false;
	}

	DWORD note = 0x00000090 | no | vel;
	result = midiOutShortMsg(handle, note);
	if (result != MMSYSERR_NOERROR)
	{
		Error("ノートオン失敗");
		return false;
	}

	return true;
}

// ノートオフ
bool MIDI_OUT::Off(DWORD no)
{
	DWORD note = 0x007f0080 | no;
	result = midiOutShortMsg(handle, note);
	if (result != MMSYSERR_NOERROR)
	{
		Error("ノートオフ失敗");
		return false;
	}

	return true;
}

// 指定したMIDIデバイスでの出力の停止
bool MIDI_OUT::MidiReset(void)
{
	result = midiOutReset(handle);
	if (result != MMSYSERR_NOERROR)
	{
		Error("リセット失敗");
		return false;
	}

	return true;
}

// SysExバッファのクリーンアップ
bool MIDI_OUT::ClearHdr(void)
{
	result = midiOutUnprepareHeader(handle, &header, sizeof(MIDIHDR));
	if (result != MMSYSERR_NOERROR)
	{
		Error("SysExのクリーンアップ失敗");
		return false;
	}

	return true;
}

// MIDI出力デバイスのクローズ
bool MIDI_OUT::Close(void)
{
	result = midiOutClose(handle);
	if (result != MMSYSERR_NOERROR)
	{
		Error("クローズ失敗");
		return false;
	}
	return true;
}
