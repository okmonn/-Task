#include "Play.h"
#include "../EnemyMane.h"
#include "Game.h"
#include "Continue.h"
#include "../Typedef.h"
#include "DxLib.h"

// コンストラクタ
Play::Play(std::weak_ptr<Input>in)
{
	//インプットクラス
	this->in = in;

	blend = 255;
	//フォントサイズ
	fSize = 18;
	SetFontSize(fSize);

	//関数ポインタ
	func = &Play::FadeIn;

	Create();
}

// デストラクタ
Play::~Play()
{
	e_list.clear();
}

// インスタンス化
void Play::Create(void)
{
	//背景クラス
	back = std::make_shared<BackGround>();

	//プレイヤークラス
	pl = std::make_shared<Player>(in);

	//UIクラス
	ui = std::make_shared<Interface>(pl);

	//敵クラス
	e_list.push_back(EnemyMane::GetInstance()->CreateDeadman(300, 330, pl));

	e_list.push_back(EnemyMane::GetInstance()->CreateBat(500, 100, pl));

	//地面クラス
	ground = std::make_shared<Ground>(pl);
}

// 描画
void Play::Draw(void)
{
	back->Draw();
	ui->Draw();
	pl->Draw();
	for (auto itr = e_list.begin(); itr != e_list.end(); ++itr)
	{
		(*itr)->Draw();
	}
	ground->Draw();
	
	SetDrawBlendMode(DX_BLENDMODE_MULA, blend);
	DrawBox(0, 0, WINDOW_X, WINDOW_Y, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// 処理
void Play::UpData(void)
{
	if ((this->*func)() == true)
	{
		if (in.lock()->CheckTrigger(PAD_INPUT_8))
		{
			alpha = true;
			func = &Play::FadeOut;
		}
		else
		{

			pl->UpData();
			if (pl->GetPos().x == 300)
			{
				if (e_list.size() < 10)
				{
					e_list.push_back(EnemyMane::GetInstance()->CreateDeadman(50, 330, pl));
				}
			}

			for (auto itr = e_list.begin(); itr != e_list.end();)
			{
				(*itr)->UpData();
				if ((*itr)->GetDie() == true)
				{
					itr = e_list.erase(itr);
				}
				else
				{
					++itr;
				}
			}

			ground->UpData();
		}
	}
}

// フェードイン
bool Play::FadeIn(void)
{
	if (alpha != false)
	{
		return false;
	}

	if (blend > 0)
	{
		blend -= 5;
	}
	else
	{
		return true;
	}

	return false;
}

// フェードアウト
bool Play::FadeOut(void)
{
	if (alpha != true)
	{
		return true;
	}

	if (blend < 255)
	{
		blend += 5;
		if (blend >= 255)
		{
			Game::Instance().ChangeScene(new Continue(in));
		}
	}

	return false;
}
