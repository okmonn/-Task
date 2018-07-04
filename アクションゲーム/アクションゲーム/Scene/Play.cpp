#include "Play.h"
#include "../Load.h"
#include "../EventMane.h"
#include "../EnemyMane.h"
#include "Game.h"
#include "Continue.h"
#include "../Typedef.h"
#include "DxLib.h"
#include <iostream>

#define CHIP_SIZE 64

// コンストラクタ
Play::Play(std::weak_ptr<Input>in)
{
	//インプットクラス
	this->in = in;
	sound = Load::GetInstance()->LoadSound("bgm/stage1.mp3");

	x = 2;
	ex = 2;
	image = LoadGraph("img/continue.png");
	mm = false;
	flam = 0;
	blend = 255;
	//フォントサイズ
	fSize = 24;
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
	//ステージクラス
	st.reset(new Stage());

	//カメラクラス
	cam.reset(new Camera(st));

	//プレイヤークラス
	pl.reset(new Player(in, cam));
	cam->SetFocus(pl);

	//背景クラス
	back.reset(new BackGround(cam));

	//UIクラス
	ui.reset(new Interface(pl));

	//地面クラス
	ground.reset(new Ground(pl));

}


// 描画
void Play::Draw(void)
{
	back->Draw();
	for (auto itr = list.begin(); itr != list.end(); ++itr)
	{
		(*itr)->Draw();
	}
	
	for (auto itr = e_list.begin(); itr != e_list.end(); ++itr)
	{
		(*itr)->Draw();
	}
	pl->Draw();
	ui->Draw();
	ground->Draw();
	cam->Draw();
	
	SetDrawBlendMode(DX_BLENDMODE_MULA, blend);
	DrawBox(0, 0, WINDOW_X, WINDOW_Y, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	if (mm == true)
	{
		DrawGraph(350, 300, image, true);
		++flam;
		if (flam >= 60)
		{
			Game::Instance().ChangeFlag(true);
		}
	}
}

// 処理
void Play::UpData(void)
{
	if ((this->*func)() == true)
	{
		if (in.lock()->CheckTrigger(PAD_INPUT_8)
			|| pl->GetDie() == true || pl->GetClear() == true)
		{
			alpha = true;
			func = &Play::FadeOut;
		}
		else
		{
			if (CheckSoundMem(sound) == false)
			{
				PlaySoundMem(sound, DX_PLAYTYPE_LOOP);
			}

			for (auto itr = e_list.begin(); itr != e_list.end();)
			{
				(*itr)->UpData();
				if ((*itr)->GetDie() == true
					|| (*itr)->GetOut() == true)
				{
					itr = e_list.erase(itr);
				}
				else
				{
					++itr;
				}
			}
			for (auto itr = list.begin(); itr != list.end(); ++itr)
			{
				(*itr)->UpData();
			}
			pl->UpData();
			cam->UpData();
			back->UpData();
			ground->UpData();
			int y = 0;
			for (auto& e : st->GetEnemyData((int)(cam->GetPos().x), (int)((cam->GetPos().x + WINDOW_X / 2 + CHIP_SIZE * 3))))
			{
				if (e == 1)
				{
					e_list.push_back(EnemyMane::GetInstance()->CreateDeadman((float)(x * CHIP_SIZE), (float)(y * CHIP_SIZE) + BAR_SIZE_Y + BAR_SIZE_Y / 2, pl, cam));
				}
				else if (e == 2)
				{
					e_list.push_back(EnemyMane::GetInstance()->CreateBat((float)(x * CHIP_SIZE), (float)(y * CHIP_SIZE) + BAR_SIZE_Y + BAR_SIZE_Y / 2, pl, cam));
				}
				else if (e == 3)
				{
					e_list.push_back(EnemyMane::GetInstance()->CreateSpear((float)(x * CHIP_SIZE), (float)(y * CHIP_SIZE), pl, cam));
				}
				++y;
				if (y >= st->GetStageRange().GetHeight() / CHIP_SIZE)
				{
					++x;
					y = 0;
				}
			}

			y = 0;
			for (auto& e : st->GetEventData((int)(cam->GetPos().x), (int)((cam->GetPos().x + WINDOW_X / 2 + CHIP_SIZE * 3))))
			{
				if (e == 1)
				{
					list.push_back(EventMane::GetInstance()->CreateLadder((float)(ex * CHIP_SIZE), (float)(y * CHIP_SIZE) - 460, pl, cam));
				}
				else if (e == 2)
				{
					list.push_back(EventMane::GetInstance()->CreateBlock((float)(ex * CHIP_SIZE), (float)(y * CHIP_SIZE) + BAR_SIZE_Y + BAR_SIZE_Y / 2, pl, cam));
				}
				++y;
				if (y >= st->GetStageRange().GetHeight() / CHIP_SIZE)
				{
					++ex;
					y = 0;
				}
			}

			for (auto itr = e_list.begin(); itr != e_list.end(); ++itr)
			{
				auto epos = (*itr)->GetCamPos();
				auto ecut = (*itr)->GetCut();
				for (auto e = list.begin(); e != list.end(); ++e)
				{
					auto evpos = (*e)->GetCamPos();
					auto evcut = (*e)->GetCut();

					if ((*itr)->GetReverse() == true)
					{
						if (epos.x - ecut.rect.GetWidth() <= evpos.x + evcut.rect.GetWidth() / 2
							&& epos.y >= evpos.y && (*itr)->GetMode() != "Die")
						{
							(*itr)->SetPPP(true);
						}
						else
						{
							(*itr)->SetPPP(false);
						}
					}
					else
					{
						if (epos.x + ecut.rect.GetWidth() >= evpos.x - evcut.rect.GetWidth() / 2
							&& epos.y >= evpos.y && (*itr)->GetMode() != "Die")
						{
							(*itr)->SetPPP(true);
						}
						else
						{
							(*itr)->SetPPP(false);
						}
					}
				}
			}
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
			if (pl->GetDie() == true)
			{
				StopSoundMem(sound);
				Game::Instance().ChangeScene(new Continue(in));
			}
			else
			{
				mm = true;
			}
		}
	}

	return false;
}
