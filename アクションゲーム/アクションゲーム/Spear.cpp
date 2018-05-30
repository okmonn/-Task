#include "Spear.h"
#include "DxLib.h"

const std::string path = "アクション/pike.act";
const int line = 330;

// コンストラクタ
Spear::Spear(Positionf pos, std::weak_ptr<Player>pl, std::weak_ptr<Camera>cam) : pl(pl), cam(cam)
{
	Load(path);
	this->pos = pos;
	camPos = { 0,0 };
	SetMode("Attack", false);
	func = &Spear::Attacker;
}

// デストラクタ
Spear::~Spear()
{
}

// 状態のセット
void Spear::SetMode(std::string m, bool r)
{
	wait = false;
	flam = 0;
	index = 0;
	mode = m;
	reverse = r;
}

// 描画
void Spear::Draw(void)
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
void Spear::UpData(void)
{
	if (pos.y >= line)
	{
		pos.y = line;
	}

	(this->*func)();
}

// 攻撃の処理
void Spear::Attacker(void)
{
	for (int i = 0; i < pl.lock()->GetAttackNum(); ++i)
	{
		Positionf tmp = pl.lock()->GetCamPos();
		Attack at = pl.lock()->GetAttack(i);
		for (unsigned int j = 0; j < attack[mode][index].size(); ++j)
		{
			if (CheackHit(camPos, attack[mode][index][j], tmp, at) == true)
			{
				if (at.type == RectType::damage && attack[mode][index][j].type == RectType::attack
					&& pl.lock()->GetMuteki() == false)
				{
					pl.lock()->SetMode("Damage", pl.lock()->GetReverse());
				}
			}
		}
	}
}
