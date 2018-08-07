#include "Input.h"
#include "KeyData.h"
#include "../../Window/Window.h"
#include <tchar.h>

// バージョン
#define VERSION 0x0800

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

// コンストラクタ
Input::Input(std::weak_ptr<Window>win) :
	win(win), input(nullptr), key(nullptr)
{
	memset(&keys, 0, sizeof(keys));
	memset(&olds, 0, sizeof(olds));

	CreateInput();
	CreateKey();
	SetKeyFormat();
	SetKeyCooperative();
}

// デストラクタ
Input::~Input()
{
	Release(key);
	Release(input);
}

// インプットの生成
HRESULT Input::CreateInput(void)
{
	result = DirectInput8Create(GetModuleHandle(0), VERSION, IID_IDirectInput8, (void**)(&input), NULL);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nインプットの生成：失敗\n"));
	}

	return result;
}

// キーデバイスの生成
HRESULT Input::CreateKey(void)
{
	result = input->CreateDevice(GUID_SysKeyboard, &key, NULL);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nキーデバイスの生成：失敗\n"));
	}

	return result;
}

// キーフォーマットのセット
HRESULT Input::SetKeyFormat(void)
{
	result = key->SetDataFormat(&keybord);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nキーフォーマットのセット：失敗\n"));
	}

	return result;
}

// キーの協調レベルのセット
HRESULT Input::SetKeyCooperative(void)
{
	result = key->SetCooperativeLevel(win.lock()->GetHandle(), DISCL_NONEXCLUSIVE | DISCL_BACKGROUND);
	if (FAILED(result))
	{
		OutputDebugString(_T("\nキーの協調レベルのセット：失敗\n"));
		return result;
	}

	//入力デバイスへのアクセス権利を取得
	key->Acquire();

	return result;
}

// キー入力
bool Input::CheckKey(UINT index)
{
	//ダミー宣言
	bool flag = false;

	//キー情報を取得
	key->GetDeviceState(sizeof(keys), &keys);

	if (keys[index] & 0x80)
	{
		flag = true;
	}

	olds[index] = keys[index];

	return flag;
}

// トリガーの入力
bool Input::TriggerKey(UINT index)
{
	//ダミー宣言
	bool flag = false;

	//キー情報を取得
	key->GetDeviceState(sizeof(keys), &keys);

	if ((keys[index] & 0x80) && !(olds[index] & 0x80))
	{
		flag = true;
	}

	olds[index] = keys[index];

	return flag;
}
