#include "Input.h"
#include "DxLib.h"

// コンストラクタ
Input::Input()
{
	//現在のキー状態
	state = 0;

	//直前のキー状態
	old_state = 0;
}

// デストラクタ
Input::~Input()
{
}

// 現在のキー状態のチェック
const bool Input::CheckTrigger(int key) const
{
	return (state & key) && !(old_state & key);
}

// 現在のキー押下状態のチェック
const bool Input::CheckPress(int key) const
{
	return state & key;
}

// 処理
void Input::UpData(void)
{
	old_state = state;
	state = GetJoypadInputState(DX_INPUT_KEY_PAD1);
}
