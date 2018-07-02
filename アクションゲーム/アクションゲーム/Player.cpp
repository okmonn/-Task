#include "Player.h"
#include "Load.h"
#include "Scene/Game.h"

const char* actionPath = "アクション/player.act";
const float g = 0.5f;
const int line = 330;
const float speed = 2.0f;

// コンストラクタ
Player::Player(std::weak_ptr<Input>in, std::weak_ptr<Camera>cam) : in(in), cam(cam)
{
	//フレーム数の初期化
	flam = 0;

	//座標
	pos = { 0, 330 };

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

	//修正座標
	camPos = { 0,0 };

	bl = false;

	d = 2.0f;
	
	m = false;
	mTime = 0;

	hp = 5;

	die = false;

	clear = false;

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

// ラスタースクロール
void Player::RasterScroll(int image, const Vector2D& pos, const Vector2D& rect, const Vector2D& size, float expansion, float rotation, float period, float vibration, bool trans, bool xturn, bool yturn)
{
	static float  correction = 0.0f;

	for (int i = 0; i < size.y; ++i)
	{
		DrawRectRotaGraph2(
			(int)((float)((pos.x + (size.x * expansion) / 2) - size.x / (2 * 20)) + cosf((i + correction) / 180.0f * 3.141592f * period) * vibration), (int)(pos.y + (size.y * expansion) / 2) - (size.y / (2 * 20)) + i,
			rect.x, rect.y + i,
			size.x, 1,
			(size.x / 2), (size.y / 2),
			(double)expansion, (double)rotation,
			image, trans, xturn, yturn);
	}

	++correction;
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
			if (mode != "Jump" && mode != "Crouch" && mode != " Damage" && mode != "Kick" && mode != "Die")
			{
				SetMode("Wait", reverse);
			}
			else
			{
				index = cut[mode].size() - 1;
			}
		}
	}

	auto right = cam.lock()->GetViewSize().GetRight();
	auto left = cam.lock()->GetViewSize().GetLeft();
	pos.x = min(max(pos.x, left), right);

	camPos = cam.lock()->CorrectionPos(pos);

	if (mTime % 4 == 0)
	{
		DrawRectRotaGraph2((int)camPos.x, (int)camPos.y,
			cut[mode][index].rect.GetLeft(), cut[mode][index].rect.GetTop(),
			cut[mode][index].rect.GetWidth(), cut[mode][index].rect.GetHeight(),
			center.x, center.y,
			(float)attackSize, 0.0f, image, true, reverse);
		//RasterScroll(image, { (int)camPos.x, (int)camPos.y }, { cut[mode][index].rect.GetLeft(), cut[mode][index].rect.GetTop() }, { cut[mode][index].rect.GetWidth(), cut[mode][index].rect.GetHeight() }, 2.0f, 0.0f, 5.0f, 5.0f, true, reverse);
	}

#ifdef _DEBUG
	DrawFormatString(10, 100, GetColor(255, 255, 255), "%d", (int)mTime);
	DrawPixel((int)camPos.x, (int)camPos.y, GetColor(255, 255, 255));
	DrawFormatString(150, 10, GetColor(255, 255, 255), "%s", mode.c_str());
	if (bl == true)
	{
		DrawString(200, 150, "ブロックに乗っている", GetColor(255, 0, 0), false);
	}
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
	if (in.lock()->CheckTrigger(PAD_INPUT_A) && camPos.x - 1 > 0)
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
		pos.x += speed;
	}
	else if (in.lock()->CheckPress(PAD_INPUT_LEFT))
	{
		if (reverse == false)
		{
			reverse = true;
		}

		if (camPos.x - 1 > 0)
		{
			pos.x += -speed;
		}
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
		bl = false;
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
	if (camPos.x - 1 > 0)
	{
		pos.x += reverse == false ? 3.0f : -3.0f;
	}
}

// しゃがみの処理
void Player::Down(void)
{
	if (!(in.lock()->CheckPress(PAD_INPUT_DOWN)) && (pos.y >= line || bl == true))
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

// 登りの処理
void Player::Climb(void)
{
	if (mode != "Climb")
	{
		return;
	}

	pos.y -= speed;
	if (pos.y < 0)
	{
		clear = true;
	}
}

// ダメージの処理
void Player::Damage(void)
{
	if (mode != "Damage")
	{
		return;
	}
	
	m = true;

	if (wait == false)
	{
		if (reverse == false)
		{
			pos.x -= d;
			
		}
		else
		{
			pos.x += d;
		}
	}
}

// 死亡の処理
void Player::Die(void)
{
	if (mode != "Die")
	{
		return;
	}

	if ((int)flam >= cut[mode][cut[mode].size() - 1].flam)
	{
		die = true;
	}
}

// 処理
void Player::UpData()
{
	if (mode != "Jump" && mode != "Climb")
	{
		if (pos.y < line && bl == false)
		{
			pos.y += vel.y;
			vel.y += g;
		}
		fly = false;
	}

	if (pos.y >= line)
	{
		SetPos((float)line);
	}

	if (wait == true)
	{
		func = &Player::Wait;
	}

	if (mode == "Climb")
	{
		func = &Player::Climb;
	}

	if (mode == "Damage")
	{
		func = &Player::Damage;
	}

	if (mode == "Die")
	{
		func = &Player::Die;
	}

	(this->*func)();

	if (m == true)
	{
		if (mTime < 180)
		{
			++mTime;
		}
		else
		{
			m = false;
			mTime = 0;
		}
	}

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

	if (mode == "Damage")
	{
		--hp;
		if (hp <= 0)
		{
			mode = "Die";
		}
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
		if (mode != "Walk" && mode != "Crouch" && mode != "Kick" && mode != "Punch" && wait == false && fly == false)
		{
			vel = { 2.0f, -8.0f };
		}
	}
	else
	{
		if (mode != "Walk" && mode != "Crouch" && mode != "Kick" && mode != "Punch" && wait == false && fly == false)
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

// 座標の取得
Positionf Player::GetCamPos(void)
{
	return camPos;
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
			tmp.x = camPos.x + (attack[mode][index][num].rect.GetLeft() + attack[mode][index][num].rect.GetWidth()) * attackSize;
			tmp.y = camPos.y + (attack[mode][index][num].rect.GetTop() + attack[mode][index][num].rect.GetHeight()) * attackSize;
		}
		else
		{
			tmp.x = camPos.x - (attack[mode][index][num].rect.GetLeft() + attack[mode][index][num].rect.GetWidth()) * attackSize;
			tmp.y = camPos.y + (attack[mode][index][num].rect.GetTop() + attack[mode][index][num].rect.GetHeight()) * attackSize;
		}
	}
	else
	{
		if (reverse == false)
		{
			tmp.x = camPos.x + (attack[mode][index][num].rect.GetLeft()) * attackSize;
			tmp.y = camPos.y + (attack[mode][index][num].rect.GetTop()) * attackSize;
		}
		else
		{
			tmp.x = camPos.x - (attack[mode][index][num].rect.GetLeft()) * attackSize;
			tmp.y = camPos.y + (attack[mode][index][num].rect.GetTop()) * attackSize;
		}
	}

	return tmp;
}

// 分割情報の取得
CutData Player::GetCut(void)
{
	return cut[mode][index];
}

void Player::SetBlock(bool b)
{
	bl = b;
}

bool Player::GetBlock(void)
{
	return bl;
}

void Player::SetVel(Vector2Df v)
{
	vel = v;
}

void Player::SetDamagePW(float pw)
{
	d = pw;
}


bool Player::GetMuteki(void)
{
	return m;
}

int Player::GetHp(void)
{
	return hp;
}

bool Player::GetDie(void)
{
	return die;
}

bool Player::GetClear(void)
{
	return clear;
}
