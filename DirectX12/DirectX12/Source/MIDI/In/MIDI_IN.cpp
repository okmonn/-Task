#include "MIDI_IN.h"

#pragma comment(lib, "winmm.lib")

// バッファーサイズ
#define BUFFER_SIZE 256

//音階
static const char keys[] = "CDEFGAB";

UCHAR MIDI_IN::state = 0;
UCHAR MIDI_IN::data1 = 0;
UCHAR MIDI_IN::data2 = 0;

// コンストラクタ
MIDI_IN::MIDI_IN() :
	result(S_OK), handle(nullptr), devNum(midiInGetNumDevs())
{
	header = {};
	caps.clear();

	if (devNum > 0)
	{
		SetMidiCaps();
		Open();
		Prepare();
		Start();
	}
}

// デストラクタ
MIDI_IN::~MIDI_IN()
{
	if (devNum > 0)
	{
		Stop();
		Reset();
		UnPrepare();
		Close();

		delete header.lpData;
	}
}

// MIDIメッセージの確認
void MIDI_IN::MimData(DWORD param1, DWORD param2)
{
	//受信データの分解
	state = param1 & 0xff;
	data1 = param1 >> 8 & 0xff;
	data2 = param1 >> 16 & 0xff;

	//タイミングクロックメッセージは無視
	if (state == 0xf8)
	{
		return;
	}
	
	//受信データの表示
	printf("%10.3f  %02X %02X %02X : ", (double)param2 / 1000, state, data1, data2);

	if (state >> 4 == 0xf) 
	{
		//システム メッセージ
		switch (state & 0xf)
		{
		case 0x1:
			printf("MTCクォーターフレーム (Type:%d Val:%2d)\n", data1 >> 4 & 0xf, data1 & 0xf);
			break;
		case 0x2:
			printf("ソングポジションポインタ (Beat:%5d)\n", (data1 + data2) << 7);
			break;
		case 0x3:
			printf("ソングセレクト (Num:%3d)\n", data1);
			break;
		case 0x6:
			printf("チューンリクエスト\n");
			break;
		case 0x7:
			printf("エンドオブエクスクルーシブ\n");
			break;
		case 0x8:
			printf("タイミングクロック\n");
			break;
		case 0xA:
			printf("スタート\n");
			break;
		case 0xB:
			printf("コンティニュー\n");
			break;
		case 0xC:
			printf("ストップ\n");
			break;
		case 0xE:
			printf("アクティブセンシング\n");
			break;
		case 0xF:
			printf("システムリセット\n");
			break;
		default:
			printf("不明なメッセージ\n");
			break;
		}
	}
	else
	{
		printf("[%2dch] ", (state & 0xf) + 1);

		//チャネル メッセージ
		switch (state >> 4) 
		{
		case 0x8:
			printf("ノートオフ (Note:%c%-2d Vel:%3d)\n", keys[data1 % 7], data1 / 7 - 3, data2);
			break;
		case 0x9:
			printf("ノートオン %s (Note:%c%-2d Vel:%3d)\n", (data2 ? "on " : "off"), keys[data1 % 7], data1 / 7 - 3, data2);
			break;
		case 0xa:
			printf("ポリフォニックキープレッシャー (Note:%c%-2d Prss:%3d)\n", keys[data1 % 7], data1 / 7 - 3, data2);
			break;
		case 0xb:
			printf("コントロールチェンジ (Num:%3d Val:%3d)\n", data1, data2);
			break;
		case 0xc:
			printf("プログラムチェンジ (Num:%3d)\n", data1);
			break;
		case 0xd:
			printf("チャンネルプレッシャー (Prss:%3d)\n", data1);
			break;
		case 0xe:
			printf("ピッチベンドチェンジ (Val:%5d)\n", (data1 + data2) << 7);
			break;
		default:
			printf("不明なメッセージ\n");
			break;
		}
	}
}

// コールバック関数
void MIDI_IN::MidiInProc(HMIDIIN handle, UINT msg, DWORD instance, DWORD param1, DWORD param2)
{
	MIDIHDR *hdr = (MIDIHDR*)param1;

	switch (msg)
	{
	case MIM_OPEN:
		printf("MIDI入力デバイスが開かれたときにこの値が設定されてコールバック関数が呼び出される\n");
		break;
	case MIM_CLOSE:
		printf("MIDI入力デバイスが閉じられたときにこの値が設定されてコールバック関数が呼び出される\n");
		break;
	case MIM_DATA:
		printf("MIDIメッセージを受信したときにこの値が設定されてコールバック関数が呼び出される\n");
		MimData(param1, param2);
		break;
	case MIM_LONGDATA:
		printf("SysEx用バッファが埋まりアプリケーションに返されるときにこの値が設定されてコールバック関数が呼び出される\n");
		break;
	case MIM_ERROR:
		printf("無効なMIDIメッセージを受信したときにこの値が設定されてコールバック関数が呼び出される\n");
		break;
	case MIM_LONGERROR:
		printf("無効または不完全なSysExを受信したときにこの値が設定されてコールバック関数が呼び出される\n");

		//再度バッファを準備して登録
		midiInPrepareHeader(handle, hdr, sizeof(MIDIHDR));
		midiInAddBuffer(handle, hdr, sizeof(MIDIHDR));
		break;
	case MIM_MOREDATA:
		printf("MIDIメッセージを受信したがアプリケーションがMIM_DATAを処理しきれていない場合にこの値が設定されてコールバック関数が呼び出される。これはOpen関数の呼び出し時にMIDI_IO_STATUSが指定された場合にのみ呼び出される\n");
		break;
	default:
		break;
	}
}

// エラーメッセージ
void MIDI_IN::Error(const std::string& m)
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

// 接続済みのMIDIデバイスの性能を格納
void MIDI_IN::SetMidiCaps(void)
{
	//配列のメモリ確保
	caps.resize(devNum);

	for (UINT i = 0; i < devNum; ++i)
	{
		result = midiInGetDevCaps(i, &caps[i], sizeof(MIDIINCAPS));
		if (result != MMSYSERR_NOERROR)
		{
			Error("キャプチャー失敗");
		}
		else
		{
			printf("[%d]\n", i + 1);
			printf("メーカーID          ：%d\n", caps[i].wMid);
			printf("製品ID              ：%d\n", caps[i].wPid);
			printf("ドライバのバージョン：%d\n", caps[i].vDriverVersion);
			printf("製品名              ：%ls\n", caps[i].szPname);
		}
	}
}

// MIDI入力デバイスのオープン
HRESULT MIDI_IN::Open(UINT index)
{
	result = midiInOpen(&handle, index, (DWORD_PTR)MidiInProc, 0, CALLBACK_FUNCTION);
	if (result != MMSYSERR_NOERROR)
	{
		Error("MIDIデバイスのオープン失敗");
	}

	return result;
}

// システムエクスクルーシブの追加
HRESULT MIDI_IN::Prepare(void)
{
	header.lpData         = new char[BUFFER_SIZE];
	header.dwBufferLength = BUFFER_SIZE;
	header.dwFlags        = 0;

	result = midiInPrepareHeader(handle, &header, sizeof(MIDIHDR));
	if (result != MMSYSERR_NOERROR)
	{
		Error("システムエクスクルーシブの準備失敗");
		return result;
	}

	result = midiInAddBuffer(handle, &header, sizeof(MIDIHDR));
	if (result != MMSYSERR_NOERROR)
	{
		Error("システムエクスクルーシブの追加失敗");
	}

	return result;
}

// MIDIデバイスの入力開始
HRESULT MIDI_IN::Start(void)
{
	result = midiInStart(handle);
	if (result != MMSYSERR_NOERROR)
	{
		Error("入力開始の失敗");
	}

	return result;
}

// MIDIデバイスの入力停止
HRESULT MIDI_IN::Stop(void)
{
	result = midiInStop(handle);
	if (result != MMSYSERR_NOERROR)
	{
		Error("入力停止の失敗");
	}

	return result;
}

// MIDIデバイスのリセット
HRESULT MIDI_IN::Reset(void)
{
	result = midiInReset(handle);
	if (result != MMSYSERR_NOERROR)
	{
		Error("リセットの失敗");
	}

	return result;
}

// システムエクスクルーシブのクリーンアップ
HRESULT MIDI_IN::UnPrepare(void)
{
	result = midiInUnprepareHeader(handle, &header, sizeof(MIDIHDR));
	if (result != MMSYSERR_NOERROR)
	{
		Error("クリーンアップの失敗");
	}

	return result;
}

// MIDIデバイスの入力終了
HRESULT MIDI_IN::Close(void)
{
	result = midiInClose(handle);
	if (result != MMSYSERR_NOERROR)
	{
		Error("クローズ失敗");
	}

	return result;
}
