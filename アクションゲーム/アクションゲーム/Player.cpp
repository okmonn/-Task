#include "Player.h"
#include "Load.h"

const char* actionPath = "アクション/player.act";
const float g = 9.8f;

// コンストラクタ
Player::Player(std::weak_ptr<Input>in) : in(in)
{
	//フレーム数の初期化
	flam = 0;

	//座標
	pos = {50, 330};

	//分割
	rect = {};

	//状態
	fmode.clear();

	//モード
	data.clear();

	//分割データ
	cut.clear();

	//配列番号
	index = 0;

	//中心点
	center = {};

	//反転フラグ
	reverse = false;

	//ループフラグ
	loop = true;

	//速度
	vel = {20.0f, -9.8f * 4};

	if (Load::GetInstance() == nullptr)
	{
		Load::Create();
	}
	
	Load();
}

// デストラクタ
Player::~Player()
{
	Load::Destroy();
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

	std::string path = FindString(actionPath, '/', 1, false) + Load::GetInstance()->GetHeader().path;
	//画像データの初期化
	image = LoadGraph(path.c_str());

	for (UINT i = 0; i < Load::GetInstance()->GetImageDataSize(); ++i)
	{
		fmode.push_back(Load::GetInstance()->GetImageData(i).name);
		data[Load::GetInstance()->GetImageData(i).name] = Load::GetInstance()->GetImageData(i);
	}
	for (auto itr = data.begin(); itr != data.end(); ++itr)
	{
		cut[itr->first] = Load::GetInstance()->GetCutData(itr->first);
	}

	SetMode(fmode[0]);
}

// 描画
void Player::Draw(void)
{
	if (data[mode].loop)
	{
		index = (flam++ / cut[mode][index].flam) % cut[mode].size();
		SetCenter(cut[mode][index].center, reverse);
	}
	else
	{
		if (index < cut[mode].size() && loop == true)
		{
			index = (flam++ / cut[mode][index].flam) % (cut[mode].size());
			SetCenter(cut[mode][index].center, reverse);
			if (flam >= cut[mode][index].flam * cut[mode].size())
			{
				loop = false;
			}
		}
		/*else
		{
			SetMode(fmode[2], reverse);
		}*/
		
	}

	DrawRectRotaGraph2((int)pos.x, (int)pos.y, 
		cut[mode][index].rect.GetLeft(), cut[mode][index].rect.GetTop(), 
		cut[mode][index].rect.GetWidth(), cut[mode][index].rect.GetHeight(), 
		center.x, center.y, 
		2.0f, 0.0f, image, true, reverse);

	DrawFormatString(10, 10, GetColor(255, 255, 255), "%d", index);
	DrawFormatString(30, 10, GetColor(255, 255, 255), "%d", center.x);
	DrawFormatString(50, 10, GetColor(255, 255, 255), "%d", (int)pos.y);
	DrawPixel((int)pos.x, (int)pos.y, GetColor(255, 255, 255));
}

// 待機の処理
void Player::Wait(void)
{
}

// 歩きの処理
void Player::Walk(void)
{
	if (mode != "Walk")
	{
		return;
	}

	if (in.lock()->CheckPress(PAD_INPUT_RIGHT) || in.lock()->CheckPress(PAD_INPUT_LEFT))
	{
		pos.x += reverse == false ? 1.0f : -1.0f;
	}
}

// ジャンプの処理
void Player::Jump(void)
{
	if (mode != "Jump")
	{
		return;
	}
	pos += vel;
	vel.y += g;
}

// パンチの処理
void Player::Panch(void)
{
}

// キックの処理
void Player::Kick(void)
{
}

// スライディングの処理
void Player::Sliding(void)
{
}

// しゃがみの処理
void Player::Down(void)
{
}

// ダメージの処理
void Player::Damage(void)
{
}

// 処理
void Player::UpData()
{
	if (in.lock()->CheckTrigger(PAD_INPUT_RIGHT))
	{
		SetMode(fmode[0]);
	}
	else if (in.lock()->CheckTrigger(PAD_INPUT_LEFT))
	{
		SetMode(fmode[0], true);
	}
	else if (CheckHitKey(KEY_INPUT_SPACE))
	{
		SetMode("Jump", reverse);
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
	if (mode == m && reverse == r)
	{
		return;
	}
	flam = 0;
	index = 0;
	mode = m; 
	reverse = r;
	loop = true;
	if (reverse == false)
	{
		vel = { 20.0f, -g * 4 };
	}
	else
	{
		vel = { -20.0f, -g * 4 };
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
