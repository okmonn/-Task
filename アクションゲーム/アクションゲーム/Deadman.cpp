#include "Deadman.h"

const std::string path = "アクション/deadman.act";
const int line = 330;

// コンストラクタ
Deadman::Deadman()
{
	Load(path);
}

// コンストラクタ
Deadman::Deadman(Positionf pos, std::weak_ptr<Player>pl) : pl(pl)
{
	Load(path);
	this->pos = pos;
	SetMode("Walk", true);
	func = &Deadman::Walk;
}

// デストラクタ
Deadman::~Deadman()
{
}

// 状態の設定
void Deadman::SetMode(std::string m, bool r)
{
	wait = false;
	flam = 0;
	mode = m;
	reverse = r;
}

// 描画
void Deadman::Draw(void)
{
	if (data[mode].loop)
	{
		if (wait == false)
		{
			++flam;
		}
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
	DrawPixel((int)pos.x, (int)pos.y, GetColor(255, 0, 255));
	DrawFormatString(500, 10, GetColor(255, 255, 0), "%f", pos.y);

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
void Deadman::UpData(void)
{
	(this->*func)();
}

// あたり判定
bool Deadman::CheackHit(void)
{
	for (int i = 0; i < pl.lock()->GetAttackNum(); ++i)
	{
		if (reverse == true)
		{
			if (pl.lock()->GetAttack(i).type == RectType::attack
				&& pl.lock()->GetAttackPos(i).x >= pos.x - cut[mode][index].rect.GetWidth()
				&& pl.lock()->GetAttackPos(i).y >= pos.y - cut[mode][index].rect.GetHeight())
			{
				return true;
			}
		}
	}
	return false;
}

// 歩きの処理
void Deadman::Walk(void)
{
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

	if (CheackHit() == true)
	{
		SetMode("Die", reverse);
		func = &Deadman::Die;
	}
}

// 死亡の処理
void Deadman::Die(void)
{
}
