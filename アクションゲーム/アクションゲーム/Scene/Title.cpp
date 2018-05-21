#include "Title.h"
#include "Game.h"
#include "Play.h"
#include "DxLib.h"

// コンストラクタ
Title::Title(std::weak_ptr<Input>in)
{
	//インプットクラス
	this->in = in;

	//画像ハンドル
	image = LoadGraph("img/title.png");
}

// デストラクタ
Title::~Title()
{
	DeleteGraph(image);
}

// 描画
void Title::Draw(void)
{
	DrawGraph(0, 0, image, true);

	DrawString(300, 400, "PUSH START (W)", GetColor(255, 0, 0), false);
}

// 処理
void Title::UpData(void)
{
	if (in.lock()->CheckTrigger(PAD_INPUT_8))
	{
		Game::Instance().ChangeScene(new Play(in));
	}
}
