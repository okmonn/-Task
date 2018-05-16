#include "Player.h"
#include "Load.h"

const char* actionPath = "アクション/player.act";
const float g = 0.5f;
const int line = 330;

// コンストラクタ
Player::Player(std::weak_ptr<Input>in) : in(in)
{
	//フレーム数の初期化
	flam = 0;

	//座標
	pos = { 50, 330 };

	//状態
	fmode.clear();

	//モード
	data.clear();

	//分割データ
	cut.clear();

	//あたり矩形データ
	attack.clear();

	//配列番号
	index = 0;

	//中心点
	center = {};

	//反転フラグ
	reverse = false;

	//ループフラグ
	loop = true;

	//速度
	vel = { 2.0f, -8.0f };

	//関数ポインタ
	func = &Player::Wait;

	//待機フラグ
	wait = true;

	//ジャンプフラグ
	fly = false;

	//矩形のサイズ
	attackSize = 2;


	Load();
}

// デストラクタ
Player::~Player()
{
	DeleteGraph(image);
}

// 文字列の検索
std::string Player::FindString(const std::string path, const char find, int offset, bool start)
{
	std::string tmp;

	if (start == true)
	{
		auto pos = path.find_first_of(find) + offset;

		tmp = path.substr(pos, path.size());
	}
	else
	{
		auto pos = path.find_last_of(find) + offset;

		tmp = path.substr(0, pos);
	}

	return tmp;
}

// 読み込み
void Player::Load(void)
{
	if (Load::GetInstance()->LoadAct(actionPath) != true)
	{
		return;
	}

	std::string path = FindString(actionPath, '/', 1, false) + Load::GetInstance()->GetHeader(actionPath).path;
	//画像データの初期化
	image = LoadGraph(path.c_str());

	for (UINT i = 0; i < Load::GetInstance()->GetImageDataSize(actionPath); ++i)
	{
		fmode.push_back(Load::GetInstance()->GetImageData(actionPath, i).name);
		data[Load::GetInstance()->GetImageData(actionPath, i).name] = Load::GetInstance()->GetImageData(actionPath, i);
	}
	for (auto itr = data.begin(); itr != data.end(); ++itr)
	{
		cut[itr->first] = Load::GetInstance()->GetCutData(actionPath, itr->first);
	}

	attack = Load::GetInstance()->GetAttac(actionPath);

	SetMode("Wait");
}

// 描画
void Player::Draw(void)
{
	if (data[mode].loop)
	{
		if (wait == false)
		{
			++flam;
		}
		index = (flam / cut[mode][index].flam) % cut[mode].size();
		SetCenter(cut[mode][index].center, reverse);
	}
	else
	{
		if (index < cut[mode].size() && loop == true)
		{
			flam++;
			if ((int)flam >= cut[mode][index].flam)
			{
				if (cut[mode].size() - 1 > index)
				{

					if (mode == "Ground")
					{
						int i = 0;
					}
					++index;
					flam = 0;
				}
				else
				{
					loop = false;
				}
			}
			SetCenter(cut[mode][index].center, reverse);
		}
		else
		{
			if (mode != "Jump" && mode != "Crouch" && mode != " Damage" && mode != "Kick")
			{
				SetMode("Wait", reverse);
			}
			else
			{
				index = cut[mode].size() - 1;
			}
		}
	}

	DrawRectRotaGraph2((int)pos.x, (int)pos.y,
		cut[mode][index].rect.GetLeft(), cut[mode][index].rect.GetTop(),
		cut[mode][index].rect.GetWidth(), cut[mode][index].rect.GetHeight(),
		center.x, center.y,
		(float)attackSize, 0.0f, image, true, reverse);

#ifdef _DEBUG
	DrawFormatString(10, 10, GetColor(255, 255, 255), "%d", index);
	DrawFormatString(30, 10, GetColor(255, 255, 255), "%d", center.x);
	DrawFormatString(50, 10, GetColor(255, 255, 255), "%d", (int)pos.y);
	DrawPixel((int)pos.x, (int)pos.y, GetColor(255, 255, 255));
	DrawFormatString(100, 10, GetColor(255, 255, 255), "%s", mode.c_str());
	DrawFormatString(200, 10, GetColor(255, 255, 255), "%d", fly);

	for (unsigned int i = 0; i < attack[mode][index].size(); ++i)
	{
		UINT color = 0;
		if (attack[mode][index][i].type == RectType::attack)
		{
			color = GetColor(255, 0, 0);
		}
		else if(attack[mode][index][i].type == RectType::damage)
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

// 待機の処理
void Player::Wait(void)
{
	//歩き
	if (in.lock()->CheckTrigger(PAD_INPUT_RIGHT) && mode != "Jump")
	{
		SetMode("Walk");
		func = &Player::Walk;
	}
	else if (in.lock()->CheckTrigger(PAD_INPUT_LEFT) && mode != "Jump")
	{
		SetMode("Walk", true);
		func = &Player::Walk;
	}

	//ジャンプ
	if (in.lock()->CheckTrigger(PAD_INPUT_A))
	{
		SetMode("Jump", reverse);
		func = &Player::Jump;
	}

	//パンチ
	if (in.lock()->CheckTrigger(PAD_INPUT_B) && mode != "Punch")
	{
		SetMode("Punch", reverse);
		func = &Player::Punch;
	}

	//しゃがみ
	if (in.lock()->CheckTrigger(PAD_INPUT_DOWN))
	{
		SetMode("Crouch", reverse);
		func = &Player::Down;
	}
}

// 歩きの処理
void Player::Walk(void)
{
	if (mode != "Walk" || fly == true)
	{
		return;
	}

	if (in.lock()->CheckPress(PAD_INPUT_RIGHT))
	{
		if (reverse == true)
		{
			reverse = false;
		}
		pos.x += 1.0f;
	}
	else if (in.lock()->CheckPress(PAD_INPUT_LEFT))
	{
		if (reverse == false)
		{
			reverse = true;
		}
		pos.x += -1.0f;
	}
	else
	{
		SetMode("Wait", reverse);
		func = &Player::Wait;
	}

	//ジャンプ
	if (in.lock()->CheckTrigger(PAD_INPUT_A))
	{
		SetMode("Jump", reverse);
		func = &Player::Jump;
	}
}

// ジャンプの処理
void Player::Jump(void)
{
	if (mode != "Jump")
	{
		return;
	}
	if (fly == false)
	{
		fly = true;
	}
	pos += vel;
	vel.y += g;

	//しゃがみ
	if (in.lock()->CheckTrigger(PAD_INPUT_DOWN))
	{
		SetMode("Crouch", reverse);
		func = &Player::Down;
	}

	//パンチ
	if (in.lock()->CheckTrigger(PAD_INPUT_B))
	{
		SetMode("Punch", reverse);
		func = &Player::Punch;
	}

	//スライディング
	if (in.lock()->CheckTrigger(PAD_INPUT_D))
	{
		SetMode("Sliding", reverse);
		func = &Player::Sliding;
	}
}

// 着地の処理
void Player::Ground(void)
{
	if (in.lock()->CheckTrigger(PAD_INPUT_A))
	{
		SetMode("Sliding", reverse);
		func = &Player::Sliding;
	}
}

// パンチの処理
void Player::Punch(void)
{
}

// キックの処理
void Player::Kick(void)
{
	if ((int)flam >= cut[mode][cut[mode].size() - 1].flam)
	{
		SetMode("Crouch", reverse);
		func = &Player::Down;
	}
}

// スライディングの処理
void Player::Sliding(void)
{
	pos.x += reverse == false ? 3.0f : -3.0f;
}

// しゃがみの処理
void Player::Down(void)
{
	if (!(in.lock()->CheckPress(PAD_INPUT_DOWN)) && pos.y >= line)
	{
		SetMode("Wait", reverse);
	}
	else
	{
		if (in.lock()->CheckTrigger(PAD_INPUT_B))
		{
			SetMode("Kick", reverse);
			func = &Player::Kick;
		}
	}

	if (in.lock()->CheckTrigger(PAD_INPUT_LEFT))
	{
		reverse = true;
	}
	else if (in.lock()->CheckTrigger(PAD_INPUT_RIGHT))
	{
		reverse = false;
	}
}

// ダメージの処理
void Player::Damage(void)
{
	if (wait == false)
	{
		if (reverse == false)
		{
			pos.x -= 2.0f;
			
		}
		else
		{
			pos.x += 2.0f;
		}
	}
}

// 処理
void Player::UpData()
{
	if (mode != "Jump")
	{
		if (pos.y < line)
		{
			pos.y += vel.y;
			vel.y += g;
		}
		else
		{
			fly = false;
		}
	}

	if (pos.y >= line)
	{
		SetPos((float)line);
	}

	if (wait == true)
	{
		func = &Player::Wait;
	}

	if (mode == "Damage")
	{
		func = &Player::Damage;
	}

	(this->*func)();
}

// 中心座標のセット
void Player::SetCenter(Position & pos, bool r)
{
	center = pos;
	if (r == true)
	{
		center.x = cut[mode][index].rect.GetWidth() - pos.x;
	}
}

// 状態のセット
void Player::SetMode(std::string m, bool r)
{
	if (mode == m && reverse == r && wait == false)
	{
		return;
	}

	if (m == "Wait")
	{
		wait = true;
		mode = "Walk";
	}
	else
	{
		wait = false;
		mode = m;
	}

	if (mode == "Ground")
	{
		func = &Player::Ground;
	}

	flam = 0;
	index = 0;
	reverse = r;
	loop = true;
	if (reverse == false)
	{
		if (mode != "Crouch" && mode != "Kick" && mode != "Punch" && wait == false && fly == false)
		{
			vel = { 2.0f, -8.0f };
		}
	}
	else
	{
		if (mode != "Crouch" && mode != "Kick" && mode != "Punch" && wait == false && fly == false)
		{
			vel = { -2.0f, -8.0f };
		}
	}
	SetCenter(cut[mode][index].center, reverse);
}

// 状態の取得
std::string Player::GetMode(void)
{
	return mode;
}

// 座標の取得
Positionf Player::GetPos(void)
{
	return pos;
}

// 座標のセット
void Player::SetPos(Positionf pos)
{
	this->pos = pos;
}

// 座標のセット
void Player::SetPos(float y)
{
	pos.y = y;
}

// 状態一覧の取得
std::vector<std::string> Player::GetAllMode(void)
{
	return fmode;
}

// 反転フラグの取得
bool Player::GetReverse(void)
{
	return reverse;
}

// あたり矩形の数
int Player::GetAttackNum(void)
{
	return attack[mode][index].size();
}

// あたり矩形の取得
Attack Player::GetAttack(USHORT num)
{
	return attack[mode][index][num];
}

// あたり座標の取得
Positionf Player::GetAttackPos(USHORT num, bool flag)
{
	Positionf tmp;
	if (flag == false)
	{
		if (reverse == false)
		{
			tmp.x = pos.x + (attack[mode][index][num].rect.GetLeft() + attack[mode][index][num].rect.GetWidth()) * attackSize;
			tmp.y = pos.y + (attack[mode][index][num].rect.GetTop() + attack[mode][index][num].rect.GetHeight()) * attackSize;
		}
		else
		{
			tmp.x = pos.x - (attack[mode][index][num].rect.GetLeft() + attack[mode][index][num].rect.GetWidth()) * attackSize;
			tmp.y = pos.y + (attack[mode][index][num].rect.GetTop() + attack[mode][index][num].rect.GetHeight()) * attackSize;
		}
	}
	else
	{
		if (reverse == false)
		{
			tmp.x = pos.x + (attack[mode][index][num].rect.GetLeft()) * attackSize;
			tmp.y = pos.y + (attack[mode][index][num].rect.GetTop()) * attackSize;
		}
		else
		{
			tmp.x = pos.x - (attack[mode][index][num].rect.GetLeft()) * attackSize;
			tmp.y = pos.y + (attack[mode][index][num].rect.GetTop()) * attackSize;
		}
	}

	return tmp;
}

// 分割情報の取得
CutData Player::GetCut(void)
{
	return cut[mode][index];
}
