#include "Over.h"
#include "Game.h"
#include "Title.h"
#include "DxLib.h"

// コンストラクタ
Over::Over(std::weak_ptr<Input>in)
{
	//インプットクラス
	this->in = in;
}

// デストラクタ
Over::~Over()
{
	DrawString(300, 300, "Over Scene END", 0xffffff);
}

// 描画
void Over::Draw(void)
{
	DrawString(300, 300, "Over Scene", 0xffffff);
}

// 処理
void Over::UpData(void)
{
	if (in.lock()->CheckTrigger(PAD_INPUT_8))
	{
		Game::Instance().ChangeScene(new Title(in));
	}
}
