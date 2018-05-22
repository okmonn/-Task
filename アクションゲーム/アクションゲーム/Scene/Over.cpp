#include "Over.h"
#include "Game.h"
#include "Title.h"
#include "DxLib.h"

// コンストラクタ
Over::Over(std::weak_ptr<Input>in)
{
	//インプットクラス
	this->in = in;
	
	//画像
	image = LoadGraph("img/gameover.png");
}

// デストラクタ
Over::~Over()
{
	DeleteGraph(image);
}

// 描画
void Over::Draw(void)
{
	DrawGraph(0, 0, image, true);
}

// 処理
void Over::UpData(void)
{
	if (in.lock()->CheckTrigger(PAD_INPUT_8))
	{
		Game::Instance().ChangeScene(new Title(in));
	}
}
