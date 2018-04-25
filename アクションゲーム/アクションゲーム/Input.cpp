#include "Input.h"
#include "DxLib.h"

// コンストラクタ
Input::Input()
{
	state = 0;
	old_state = 0;
}

// デストラクタ
Input::~Input()
{
}

// 現在のキー状態のチェック
bool Input::CheckTrigger(int key) const
{

	return false;
}

// 現在のキー押下状態のチェック
bool Input::CheckPress(int key) const
{

	return false;
}

// 処理
void Input::UpData(void)
{

}
