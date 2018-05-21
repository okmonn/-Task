#include "Title.h"
#include "Game.h"
#include "Play.h"
#include "DxLib.h"

// コンストラクタ
Title::Title(std::weak_ptr<Input>in)
{
	//インプットクラス
	this->in = in;
}

// デストラクタ
Title::~Title()
{
	DrawString(300, 300, "Title Scene END", 0xffffff);
}

// 描画
void Title::Draw(void)
{
	DrawString(300, 300, "Title Scene", 0xffffff);
}

// 処理
void Title::UpData(void)
{
	if (in.lock()->CheckTrigger(PAD_INPUT_8))
	{
		Game::Instance().ChangeScene(new Play(in));
	}
}
