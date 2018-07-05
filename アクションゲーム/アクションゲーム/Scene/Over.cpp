#include "Over.h"
#include "Game.h"
#include "Title.h"
#include "DxLib.h"

const int time = 300;

// コンストラクタ
Over::Over(std::weak_ptr<Input>in)
{
	//インプットクラス
	this->in = in;
	
	//画像
	image = LoadGraph("img/gameover.png");

	flam = 0;
}

// デストラクタ
Over::~Over()
{
	DeleteGraph(image);
}

// 描画
void Over::Draw(void)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, time - flam);
	DrawGraph(0, 0, image, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// 処理
void Over::UpData(void)
{
	if (in.lock()->CheckTrigger(PAD_INPUT_8)
		|| flam >= time)
	{
		Game::Instance().ChangeScene(new Title(in));
	}
	else
	{
		++flam;
	}
}
