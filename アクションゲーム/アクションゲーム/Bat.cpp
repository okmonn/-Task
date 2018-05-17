#include "Bat.h"

const std::string path = "アクション/bat.act";

// コンストラクタ
Bat::Bat(Positionf pos, std::weak_ptr<Player>pl) : pl(pl), range(200.0f)
{
	Load(path);
	this->pos = pos;
	SetMode("Wait", false);
	func = &Bat::Wait;
}

// デストラクタ
Bat::~Bat()
{
}

// 状態の設定
void Bat::SetMode(std::string m, bool r)
{
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
			flam = 0;
		}
		else
		{
			++flam;
		}
	}
	DrawRectRotaGraph2((int)pos.x, (int)pos.y,
		cut[mode][index].rect.GetLeft(), cut[mode][index].rect.GetTop(),
		cut[mode][index].rect.GetWidth(), cut[mode][index].rect.GetHeight(),
		center.x, center.y,
		(float)attackSize, 0.0f, image, true, reverse);

#ifdef _DEBUG
	DrawFormatString(500, 10, GetColor(255, 255, 0), "%d", (int)pos.y);
	DrawPixel((int)pos.x, (int)pos.y, GetColor(255, 255, 255));
	DrawFormatString(550, 10, GetColor(255, 0, 0), "%d", index);
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
			DrawBox((int)pos.x + (attack[mode][index][i].rect.GetLeft() * attackSize),
				(int)pos.y + (attack[mode][index][i].rect.GetTop() * attackSize),
				(int)pos.x + (attack[mode][index][i].rect.GetLeft() + attack[mode][index][i].rect.GetWidth()) * attackSize,
				(int)pos.y + (attack[mode][index][i].rect.GetTop() + attack[mode][index][i].rect.GetHeight()) * attackSize,
				color, false);
		}
		else
		{
			DrawBox((int)pos.x - (attack[mode][index][i].rect.GetLeft() * attackSize),
				(int)pos.y + (attack[mode][index][i].rect.GetTop() * attackSize),
				(int)pos.x - (attack[mode][index][i].rect.GetLeft() + attack[mode][index][i].rect.GetWidth()) * attackSize,
				(int)pos.y + (attack[mode][index][i].rect.GetTop() + attack[mode][index][i].rect.GetHeight()) * attackSize,
				color, false);
		}
	}
#endif
}

// 処理
void Bat::UpData(void)
{
	
	(this->*func)();
}

// 待機の処理
void Bat::Wait(void)
{
	if (mode != "Wait")
	{
		return;
	}
	if (abs(pos.x - pl.lock()->GetPos().x) <= range)
	{
		SetMode("Fly", reverse);
		func = &Bat::Fly;
	}
}

// 飛びの処理
void Bat::Fly(void)
{
	if (mode != "Fly")
	{
		return;
	}

	if (pl.lock()->GetPos().x > pos.x)
	{
		if (reverse == true)
		{
			reverse = false;
		}
		pos.x += 1.0f;
	}
	else if (pl.lock()->GetPos().x < pos.x)
	{
		if (reverse == false)
		{
			reverse = true;
		}
		pos.x -= 1.0f;
	}

	if (pl.lock()->GetPos().y - pl.lock()->GetCut().rect.GetWidth() > pos.y)
	{
		pos.y += 1.0f;
	}
	else if (pl.lock()->GetPos().y - pl.lock()->GetCut().rect.GetWidth() < pos.y)
	{
		pos.y -= 1.0f;
	}

	for (int i = 0; i < pl.lock()->GetAttackNum(); ++i)
	{
		Positionf tmp = pl.lock()->GetPos();
		Attack at = pl.lock()->GetAttack(i);
		for (unsigned int j = 0; j < attack[mode][index].size(); ++j)
		{
			if (CheackHit(pos, attack[mode][index][j], tmp, at) == true)
			{
				if (at.type == RectType::attack && attack[mode][index][j].type == RectType::damage)
				{
					SetMode("Die", reverse);
				}
				else if (at.type == RectType::damage && attack[mode][index][j].type == RectType::attack)
				{
					bool flag = reverse == true ? false : true;
					pl.lock()->SetMode("Damage", flag);
				}
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
}

// 死亡の処理
void Bat::Die(void)
{
	if (mode != "Die")
	{
		return;
	}
}
