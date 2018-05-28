#include "Play.h"
#include "../EnemyMane.h"
#include "Game.h"
#include "Continue.h"
#include "../Typedef.h"
#include "DxLib.h"

#define CHIP_SIZE 64

// �R���X�g���N�^
Play::Play(std::weak_ptr<Input>in)
{
	//�C���v�b�g�N���X
	this->in = in;

	blend = 255;
	//�t�H���g�T�C�Y
	fSize = 24;
	SetFontSize(fSize);

	//�֐��|�C���^
	func = &Play::FadeIn;

	Create();
}

// �f�X�g���N�^
Play::~Play()
{
	e_list.clear();
}

// �C���X�^���X��
void Play::Create(void)
{
	//�X�e�[�W�N���X
	st = std::make_shared<Stage>();

	//�J�����N���X
	cam = std::make_shared<Camera>(st);

	//�v���C���[�N���X
	pl = std::make_shared<Player>(in, cam);
	cam->SetFocus(pl);

	//�w�i�N���X
	back = std::make_shared<BackGround>(cam);

	//UI�N���X
	ui = std::make_shared<Interface>(pl);

	//�n�ʃN���X
	ground = std::make_shared<Ground>(pl);

}

// �`��
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
	cam->Draw();
	
	SetDrawBlendMode(DX_BLENDMODE_MULA, blend);
	DrawBox(0, 0, WINDOW_X, WINDOW_Y, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}

// ����
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
			back->UpData();
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
			pl->UpData();
			cam->UpData();
			ground->UpData();
			int y = 0;
			static int x = 0;
			for (auto& e : st->GetEnemyData((int)(cam->GetPos().x), (int)((cam->GetPos().x + WINDOW_X / 2 + CHIP_SIZE * 3))))
			{
				if (e == 1)
				{
					e_list.push_back(EnemyMane::GetInstance()->CreateDeadman((float)(x * CHIP_SIZE), (float)(y * CHIP_SIZE), pl, cam));
				}
				else if (e == 2)
				{
					e_list.push_back(EnemyMane::GetInstance()->CreateBat((float)(x * CHIP_SIZE), (float)(y * CHIP_SIZE), pl, cam));
				}
				++y;
				if (y >= st->GetStageRange().GetHeight() / CHIP_SIZE)
				{
					++x;
					y = 0;
				}
			}

		}
	}
}

// �t�F�[�h�C��
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

// �t�F�[�h�A�E�g
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
