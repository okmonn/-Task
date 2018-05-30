#include "Ladder.h"

const std::string path = "アクション/event.act";
const int line = 330;

// コンストラクタ
Ladder::Ladder(Positionf pos, std::shared_ptr<Player>pl, std::shared_ptr<Camera>cam) : pl(pl), cam(cam)
{
	this->pos = pos;
	Load(path);
	mode = "Ladder";
}

// デストラクタ
Ladder::~Ladder()
{
}

// 描画
void Ladder::Draw(void)
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
void Ladder::UpData(void)
{
	if (pl.lock()->GetCamPos().x >= camPos.x)
	{
		if (pl.lock()->GetMode() != "Climb")
		{
			pl.lock()->SetMode("Climb", pl.lock()->GetReverse());
		}
	}
}
