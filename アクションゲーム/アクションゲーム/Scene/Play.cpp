#include "Play.h"
#include "../EnemyMane.h"
#include "Game.h"
#include "Continue.h"
#include "DxLib.h"

// コンストラクタ
Play::Play(std::weak_ptr<Input>in)
{
	//インプットクラス
	this->in = in;

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
}

// 処理
void Play::UpData(void)
{
	if (in.lock()->CheckTrigger(PAD_INPUT_8))
	{
		Game::Instance().ChangeScene(new Continue(in));
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
