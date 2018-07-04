#include "Title.h"
#include "../Load.h"
#include "Game.h"
#include "Play.h"
#include "DxLib.h"

// コンストラクタ
Title::Title(std::weak_ptr<Input>in) : flam(0)
{
	//インプットクラス
	this->in = in;
	cnt = 60;

	//画像ハンドル
	image = LoadGraph("img/title.png");
	sound = Load::GetInstance()->LoadSound("se/start.mp3");

	//フォントサイズ
	fSize = 36;

	AddFontResourceEx("フォント/GN-Kin-iro_SansSerif.ttf", FR_PRIVATE, NULL);
	SetFontSize(fSize);
	ChangeFont("GN-きんいろサンセリフ", DX_CHARSET_DEFAULT);
	
	// 関数ポインタ
	func = &Title::FadeIn;
}

// デストラクタ
Title::~Title()
{
	DeleteGraph(image);
}

// 描画
void Title::Draw(void)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, blend);
	DrawGraph(0, 0, image, true);

	if ((flam / cnt) % 2 == 0)
	{
		DrawString(280, 400, "PUSH START (W)", GetColor(255, 0, 0), false);
	}
	
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// フェードイン
void Title::FadeIn(void)
{
	if (alpha != false)
	{
		return;
	}

	if (blend <= 255)
	{
		blend += 5;
	}

	
}

// フェードイン
void Title::FadeOut(void)
{
	if (alpha != true)
	{
		return;
	}

	if (blend >= 0)
	{
		blend -= 5;
		if (blend <= 0)
		{
			PlaySoundMem(sound, DX_PLAYTYPE_BACK);
			Game::Instance().ChangeScene(new Play(in));
		}
	}
}

// 処理
void Title::UpData(void)
{
	flam++;
	if (in.lock()->CheckTrigger(PAD_INPUT_8))
	{
		alpha = true;
		cnt = 6;
		flam = 0;
		func = &Title::FadeOut;
	}
	
	(this->*func)();
}
