#include "Deadman.h"

const std::string path = "アクション/deadman.act";
const int line = 330;

// コンストラクタ
Deadman::Deadman()
{
	Load(path);
}

// コンストラクタ
Deadman::Deadman(Positionf pos, std::weak_ptr<Player>pl, std::weak_ptr<Camera>cam) : pl(pl), cam(cam), down(18.0f), wait(0), go(0)
{
	Load(path);
	this->pos = pos;
	camPos = { 0,0 };
	SetMode("Walk", true);
	func = &Deadman::Walk;
	memset(dir, false, sizeof(dir));
}

// デストラクタ
Deadman::~Deadman()
{
	DeleteGraph(image);
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
				else if (mode == "Die2")
				{
					pos.y += down;
					down += 12.0f;
				}
			}
			else
			{
				if (mode == "Die" || mode == "Die2")
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
	DrawFormatString(500, 100, GetColor(255, 255, 0), "%d", (int)camPos.x);
	DrawPixel((int)camPos.x, (int)camPos.y, GetColor(255, 255, 255));
//	DrawFormatString(550, 10, GetColor(255, 0, 0), "%d", index);
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
void Deadman::UpData(void)
{
	if (mode != "Die" && mode != "Die2")
	{
		if (pos.y < line || pos.y >= line)
		{
			pos.y = line;
		}
	}
	(this->*func)();
}


void Deadman::ChangeDir(void)
{
	if (reverse == true)
	{
		if (dir[0] == false)
		{
			dir[1] = false;
			dir[0] = true;
		}
		reverse = false;
	}
	else
	{
		if (dir[1] == false)
		{
			dir[0] = false;
			dir[1] = true;
		}
		reverse = true;
	}
	go = 1;
}

Positionf Deadman::GetCamPos(void)
{
	return camPos;
}

CutData Deadman::GetCut(void)
{
	return cut[mode][index];
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
		if (pl.lock()->GetCamPos().x > camPos.x)
		{
			if (reverse == true)
			{
				reverse = false;
			}
			dir[0] = true;
			++go;
		}
		else if (pl.lock()->GetCamPos().x < camPos.x)
		{
			if (reverse == false)
			{
				reverse = true;
			}
			dir[1] = true;
			++go;
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
		if (go < 120)
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
		Positionf tmp = pl.lock()->GetCamPos();
		Attack at = pl.lock()->GetAttack(i);
		for (unsigned int j = 0; j < attack[mode][index].size(); ++j)
		{
			if (CheackHit(camPos, attack[mode][index][j], tmp, at) == true)
			{
				if (at.type == RectType::attack && attack[mode][index][j].type == RectType::damage
					&& pl.lock()->GetMode() == "Punch")
				{
					if (reverse == true && pl.lock()->GetReverse() == false
						|| reverse == false && pl.lock()->GetReverse() == true)
					{
						SetMode("Die", reverse);
					}
				}
				else if (at.type == RectType::attack && attack[mode][index][j].type == RectType::damage
					&& pl.lock()->GetMode() != "Punch")
				{
					if (reverse == true && pl.lock()->GetReverse() == false
						|| reverse == false && pl.lock()->GetReverse() == true)
					{
						SetMode("Die2", reverse);
					}
				}
				else if (at.type == RectType::damage && attack[mode][index][j].type == RectType::attack
					&& pl.lock()->GetMuteki() == false)
				{
					bool flag = reverse == true ? false : true;
					pl.lock()->SetMode("Damage", flag);
				}
				hit = true;
			}
		}
	}
}

// 死亡の処理
void Deadman::Die(void)
{
	die = true;
	++wait;
	if (wait > 200)
	{
		if ((wait - 200) >= cut[mode][index].flam)
		{
			if (mode == "Die")
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
			else
			{
				--index;
				pos.y -= down;
				down -= 12.0f;
				if (index == 0)
				{
					SetMode("Walk", reverse);
					func = &Deadman::Walk;
				}
			}
		}
	}
}
