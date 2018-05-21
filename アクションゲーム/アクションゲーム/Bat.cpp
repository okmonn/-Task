#include "Bat.h"

const std::string path = "�A�N�V����/bat.act";
const float line = 330;

// �R���X�g���N�^
Bat::Bat(Positionf pos, std::weak_ptr<Player>pl) : pl(pl), range(200.0f), cnt(0)
{
	Load(path);
	this->pos = pos;
	SetMode("Wait", false);
	func = &Bat::Wait;
}

// �f�X�g���N�^
Bat::~Bat()
{
}

// ��Ԃ̐ݒ�
void Bat::SetMode(std::string m, bool r)
{
	flam = 0;
	index = 0;
	mode = m;
	reverse = r;
}

// �`��
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

// ����
void Bat::UpData(void)
{
	if (pos.y >= line)
	{
		pos.y = line;
	}

	(this->*func)();
}

// �ҋ@�̏���
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

// ��т̏���
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
		pos.y += 2.0f * sinf(3.14f * 2 / 270 * cnt);
		cnt++;
	}
	else if (pl.lock()->GetPos().y - pl.lock()->GetCut().rect.GetWidth() < pos.y)
	{
		pos.y -= 2.0f * sinf(3.14f * 2 / 270 * cnt);
		cnt++;
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
					SetMode("Damage", reverse);
					func = &Bat::Damage;
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

// �_���[�W�̏���
void Bat::Damage(void)
{
	if (mode != "Damage")
	{
		return;
	}

	if (pos.y < line)
	{
		pos.y += 1.0f;
	}
	else
	{
		SetMode("Die", reverse);
		func = &Bat::Die;
	}
}

// ���S�̏���
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
