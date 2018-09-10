#include "MIDI_OUT.h"

#pragma comment(lib, "winmm.lib")

BYTE GMSystemOn[] = { 0xf0, 0x7e, 0x7f, 0x09, 0x01, 0xf7 };

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
void MIDI_OUT::Error(const std::string& m)
{
#ifdef UNICODE
	LPWSTR errmsg = 0;
#else
	LPSTR errmsg = 0;
#endif
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
