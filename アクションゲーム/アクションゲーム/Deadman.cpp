#include "Deadman.h"

const std::string path = "アクション/deadman.act";
const int line = 330;

// コンストラクタ
Deadman::Deadman()
{
	Load(path);
}

// コンストラクタ
Deadman::Deadman(Positionf pos, std::weak_ptr<Player>pl) : pl(pl), down(18.0f), wait(0), go(0)
{
	Load(path);
	this->pos = pos;
	SetMode("Walk", true);
	func = &Deadman::Walk;
	memset(dir, false, sizeof(dir));
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
	index = 0;
	mode = m;
	reverse = r;
	memset(dir, false, sizeof(dir));
	go = 0;

	if (m == "Die")
	{
		wait = 30;
	}
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
				if (mode == "Die")
				{
					pos.y += down;
					down -= 1.0f;
				}
			}
			else
			{
				if (mode == "Die")
				{
					func = &Deadman::Die;
				}
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
void Deadman::UpData(void)
{
	if (mode != "Die")
	{
		if (pos.y < line || pos.y >= line)
		{
			pos.y = line;
		}
	}
	(this->*func)();
}

// あたり判定
bool Deadman::CheackHit(Positionf& pos1, Attack& a1, Positionf& pos2, Attack& a2)
{
	if (reverse == true)
	{
		if (abs((pos1.x + a1.rect.pos.x) - (pos2.x + a2.rect.pos.x)) < (a1.rect.GetWidth() + a2.rect.GetWidth())
			&& abs((pos1.y + a1.rect.pos.y) - (pos2.y + a2.rect.pos.y)) < (a1.rect.GetHeight() + a2.rect.GetHeight()))
		{
			return true;
		}
	}
	else
	{
		if (abs((pos1.x - a1.rect.pos.x) - (pos2.x - a2.rect.pos.x)) < (a1.rect.GetWidth() + a2.rect.GetWidth())
			&& abs((pos1.y - a1.rect.pos.y) - (pos2.y - a2.rect.pos.y)) < (a1.rect.GetHeight() + a2.rect.GetHeight()))
		{
			return true;
		}
	}
	return false;
}

// 歩きの処理
void Deadman::Walk(void)
{
	if (mode != "Walk")
	{
		return;
	}

	if (go == 0)
	{
		if (pl.lock()->GetPos().x > pos.x)
		{
			if (reverse == true)
			{
				reverse = false;
			}
			dir[0] = true;
			pos.x += 1.0f;
		}
		else if (pl.lock()->GetPos().x < pos.x)
		{
			if (reverse == false)
			{
				reverse = true;
			}
			dir[1] = true;
			pos.x -= 1.0f;
		}
	}
	else
	{
		if (dir[0] == true)
		{
			pos.x += 1.0f;
		}
		else if (dir[1] == true)
		{
			pos.x -= 1.0f;
		}
		if (go < 60)
		{
			++go;
		}
		else
		{
			memset(dir, false, sizeof(dir));
			go = 0;
		}
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
					pl.lock()->SetMode("Damage", pl.lock()->GetReverse());
				}
			}
		}
	}
}

// 死亡の処理
void Deadman::Die(void)
{
	++wait;
	if (wait > 200)
	{
		if ((wait - 200) >= cut[mode][index].flam)
		{
			--index;
			pos.y -= down;
			down += 1.0f;
			if (index == 0)
			{
				SetMode("Walk", reverse);
				func = &Deadman::Walk;
			}
		}
	}
}
