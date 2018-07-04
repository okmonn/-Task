#include "Bat.h"
#include "Load.h"

const std::string path = "アクション/bat.act";
const float line = 330;

// コンストラクタ
Bat::Bat(Positionf pos, std::weak_ptr<Player>pl, std::weak_ptr<Camera>cam) : pl(pl), cam(cam), range(200.0f), cnt(0)
{
	Load(path);
	this->pos = pos;
	camPos = { 0,0 };
	sound["wait"] = Load::GetInstance()->LoadSound("se/habataki.mp3");
	sound["damage"] = Load::GetInstance()->LoadSound("se/bird_damage.mp3");
	SetMode("Wait", false);
	func = &Bat::Wait;
}

// デストラクタ
Bat::~Bat()
{
	DeleteGraph(image);
}

// 状態の設定
void Bat::SetMode(std::string m, bool r)
{
	for (auto itr = sound.begin(); itr != sound.end(); ++itr)
	{
		StopSoundMem(itr->second);
	}
	flam = 0;
	index = 0;
	mode = m;
	reverse = r;
}

// 描画
void Bat::Draw(void)
{
	if (data[mode].loop)
	{
		++flam;
		if (flam >= cut[mode][index].flam)
		{
			if (index >= cut[mode].size() - 1)
			{
				index = 0;
			}
			else
			{
				++index;
			}
			flam = 0;
		}
		SetSenter(cut[mode][index].center, reverse);
	}
	else
	{
		if (flam >= cut[mode][index].flam)
		{
			if (index < cut[mode].size() - 1)
			{
				++index;
			}
			else
			{
				if (mode == "Die")
				{
					die = true;
				}
			}
			flam = 0;
		}
		else
		{
			++flam;
		}
	}

	auto right = cam.lock()->GetViewSize().GetRight();
	auto left = cam.lock()->GetViewSize().GetLeft();
	pos.x = min(max(pos.x, left), right);

	camPos = cam.lock()->CorrectionPos(pos);

	DrawRectRotaGraph2((int)camPos.x, (int)camPos.y,
		cut[mode][index].rect.GetLeft(), cut[mode][index].rect.GetTop(),
		cut[mode][index].rect.GetWidth(), cut[mode][index].rect.GetHeight(),
		center.x, center.y,
		(float)attackSize, 0.0f, image, true, reverse);

#ifdef _DEBUG
	//DrawFormatString(500, 80, GetColor(255, 255, 0), "%d", (int)camPos.x);
	DrawPixel((int)camPos.x, (int)camPos.y, GetColor(255, 255, 255));
	//DrawFormatString(550, 10, GetColor(255, 0, 0), "%d", index);
	for (unsigned int i = 0; i < attack[mode][index].size(); ++i)
	{
		UINT color = 0;
		if (attack[mode][index][i].type == RectType::attack)
		{
			color = GetColor(255, 0, 0);
		}
		else if (attack[mode][index][i].type == RectType::damage)
		{
			color = GetColor(0, 255, 0);
		}
		else
		{
			color = GetColor(0, 0, 255);
		}

		if (reverse == false)
		{
			DrawBox((int)camPos.x + (attack[mode][index][i].rect.GetLeft() * attackSize),
				(int)camPos.y + (attack[mode][index][i].rect.GetTop() * attackSize),
				(int)camPos.x + (attack[mode][index][i].rect.GetLeft() + attack[mode][index][i].rect.GetWidth()) * attackSize,
				(int)camPos.y + (attack[mode][index][i].rect.GetTop() + attack[mode][index][i].rect.GetHeight()) * attackSize,
				color, false);
		}
		else
		{
			DrawBox((int)camPos.x - (attack[mode][index][i].rect.GetLeft() * attackSize),
				(int)camPos.y + (attack[mode][index][i].rect.GetTop() * attackSize),
				(int)camPos.x - (attack[mode][index][i].rect.GetLeft() + attack[mode][index][i].rect.GetWidth()) * attackSize,
				(int)camPos.y + (attack[mode][index][i].rect.GetTop() + attack[mode][index][i].rect.GetHeight()) * attackSize,
				color, false);
		}
	}
#endif
}

// 処理
void Bat::UpData(void)
{
	if (pos.y >= line)
	{
		pos.y = line;
	}

	(this->*func)();
}

void Bat::ChangeDir(void)
{
}

// 待機の処理
void Bat::Wait(void)
{
	if (mode != "Wait")
	{
		return;
	}
	if (abs(camPos.x - pl.lock()->GetCamPos().x) <= range)
	{
		if (camPos.x > pl.lock()->GetCamPos().x)
		{
			reverse = true;
		}
		else
		{
			reverse = false;
		}

		if (pos.y < pl.lock()->GetPos().y - pl.lock()->GetCut().rect.GetHeight() - 64)
		{
			pos.y += 4.0f;
		}
		else
		{
			SetMode("Fly", reverse);
			PlaySoundMem(sound["wait"], DX_PLAYTYPE_BACK);
			func = &Bat::Fly;
		}
	}
}

// 飛びの処理
void Bat::Fly(void)
{
	if (mode != "Fly")
	{
		return;
	}

	if (reverse == true)
	{
		pos.x -= 2.0f;
	}
	else
	{
		pos.x += 2.0f;
	}

	if (camPos.x < -100.0f)
	{
		out = true;
	}



	pos.y += 2.0f * sinf(3.14f * 2 / 90 * cnt);
	cnt++;

	for (int i = 0; i < pl.lock()->GetAttackNum(); ++i)
	{
		Positionf tmp = pl.lock()->GetCamPos();
		Attack at = pl.lock()->GetAttack(i);
		for (unsigned int j = 0; j < attack[mode][index].size(); ++j)
		{
			if (CheackHit(camPos, attack[mode][index][j], tmp, at) == true)
			{
				if (at.type == RectType::attack && attack[mode][index][j].type == RectType::damage)
				{
					if (reverse == true && pl.lock()->GetReverse() == false
						|| reverse == false && pl.lock()->GetReverse() == true)
					{
						SetMode("Damage", reverse);
						PlaySoundMem(sound["damage"], DX_PLAYTYPE_BACK);
						func = &Bat::Damage;
					}
				}
				else if (at.type == RectType::damage && attack[mode][index][j].type == RectType::attack
					&& pl.lock()->GetMuteki() == false && pl.lock()->GetMode() != "Die")
				{
					bool flag = reverse == true ? false : true;
					pl.lock()->SetMode("Damage", flag);
				}
				hit = true;
			}
		}
	}
}

// ダメージの処理
void Bat::Damage(void)
{
	if (mode != "Damage")
	{
		return;
	}

	if (pos.y < line)
	{
		pos.y += 3.0f;
		pos.x += reverse == true ? 1.0f : -1.0f;
	}
	else
	{
		SetMode("Die", reverse);
		func = &Bat::Die;
	}
}

// 死亡の処理
void Bat::Die(void)
{
	if (mode != "Die")
	{
		return;
	}

	if (pos.y <= line + 10.0f)
	{
		pos.y += 2.0f;
	}
}
