#include "Play.h"
#include "../EnemyMane.h"
#include "Game.h"
#include "Continue.h"
#include "../Typedef.h"
#include "DxLib.h"

// �R���X�g���N�^
Play::Play(std::weak_ptr<Input>in)
{
	//�C���v�b�g�N���X
	this->in = in;

	blend = 255;
	//�t�H���g�T�C�Y
	fSize = 18;
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
	//�w�i�N���X
	back = std::make_shared<BackGround>();

	//�v���C���[�N���X
	pl = std::make_shared<Player>(in);

	//UI�N���X
	ui = std::make_shared<Interface>(pl);

	//�G�N���X
	e_list.push_back(EnemyMane::GetInstance()->CreateDeadman(300, 330, pl));

	e_list.push_back(EnemyMane::GetInstance()->CreateBat(500, 100, pl));

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
