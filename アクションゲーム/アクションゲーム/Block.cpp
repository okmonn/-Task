#include "Block.h"

const std::string path = "アクション/event.act";
const int line = 330;

// コンストラクタ
Block::Block(Positionf pos, std::shared_ptr<Player>pl, std::shared_ptr<Camera>cam): pl(pl), cam(cam)
{
	attackSize = 1;
	this->pos = pos;
	Load(path);
	mode = "Block";
	flag = false;
}

// デストラクタ
Block::~Block()
{
}

// 描画
void Block::Draw(void)
{
	if (data[mode].loop)
	{
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
		cut[mode][index].center.x, cut[mode][index].center.y,
		(float)attackSize, 0.0f, image, true, false);

#ifdef _DEBUG
	DrawPixel((int)camPos.x, (int)camPos.y, GetColor(255, 255, 255));
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

		DrawBox((int)camPos.x + (attack[mode][index][i].rect.GetLeft() * attackSize),
			(int)camPos.y + (attack[mode][index][i].rect.GetTop() * attackSize),
			(int)camPos.x + (attack[mode][index][i].rect.GetLeft() + attack[mode][index][i].rect.GetWidth()) * attackSize,
			(int)camPos.y + (attack[mode][index][i].rect.GetTop() + attack[mode][index][i].rect.GetHeight()) * attackSize,
			color, false);
	}
#endif
}

// 処理
void Block::UpData(void)
{
	if (pos.y >= line)
	{
		pos.y = (float)(line - (attack[mode][index][0].rect.GetHeight() - attack[mode][index][0].rect.pos.y));
	}

	for (int i = 0; i < pl.lock()->GetAttackNum(); ++i)
	{
		Positionf tmp = pl.lock()->GetCamPos();
		Attack at = pl.lock()->GetAttack(i);
		for (unsigned int j = 0; j < attack[mode][index].size(); ++j)
		{
			if (CheackHit2(camPos, attack[mode][index][j], tmp, at) == true)
			{
				if (pl.lock()->GetMode() == "Jump" && pl.lock()->GetBlock() == false)
				{
					pl.lock()->SetBlock(true);
					pl.lock()->SetMode("Wait", pl.lock()->GetReverse());
				}
				else if ((pl.lock()->GetMode() == "Walk" || pl.lock()->GetMode() == "Wait") && pl.lock()->GetBlock() == false)
				{
					pl.lock()->SetDamagePW(2.0f);
					pl.lock()->SetMode("Wait", pl.lock()->GetReverse());
				}
				else if (pl.lock()->GetMode() == "Damage" && pl.lock()->GetBlock() == false)
				{
					pl.lock()->SetDamagePW(0.0f);
				}
			}
			else
			{
				if (pl.lock()->GetMode() == "Walk" || pl.lock()->GetMode() == "Damage")
				{
					pl.lock()->SetBlock(false);
					pl.lock()->SetDamagePW(2.0f);
				}
			}
		}
	}
}
