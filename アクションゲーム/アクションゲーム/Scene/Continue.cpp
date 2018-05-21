#include "Continue.h"
#include "Game.h"
#include "Play.h"
#include "Over.h"
#include "DxLib.h"

// コンストラクタ
Continue::Continue(std::weak_ptr<Input>in)
{
	//インプットクラス
	this->in = in;
}

// デストラクタ
Continue::~Continue()
{
	DrawString(300, 300, "Continue Scene END", 0xffffff);
}

// 描画
void Continue::Draw(void)
{
	DrawString(300, 300, "Continue Scene", 0xffffff);
}

// 処理
void Continue::UpData(void)
{
	if (in.lock()->CheckTrigger(PAD_INPUT_8))
	{
		Game::Instance().ChangeScene(new Over(in));
	}
	else if (in.lock()->CheckTrigger(PAD_INPUT_9))
	{
		Game::Instance().ChangeScene(new Play(in));
	}
}
