#include "Player.h"
#include "Load.h"

const char* actionPath = "アクション/player.act";

// コンストラクタ
Player::Player()
{
	//フレーム数の初期化
	flam = 0;

	//座標
	pos = {};

	//分割
	rect = {};

	fmode.clear();

	//状態
	data.clear();

	//分割データ
	cut.clear();

	//配列番号
	index = 0;

	//反転フラグ
	reverse = false;

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

	mode = fmode[2];
}

// 描画
void Player::Draw(void)
{
	if (data[mode].loop == 1)
	{
		
	}
	index = (flam++ / cut[mode][0].flam) % cut[mode].size();

	DrawRectRotaGraph2(300, 300, 
		cut[mode][index].rect.GetLeft(), cut[mode][index].rect.GetTop(), 
		cut[mode][index].rect.GetWidth(), cut[mode][index].rect.GetHeight(), 
		(cut[mode][index].center.x - (int)pos.x), (cut[mode][index].center.y - (int)pos.y), 
		2.0f, 0.0f, image, true, reverse);

	DrawFormatString(10, 10, GetColor(255, 255, 255), "%d", index);
}

// 処理
void Player::UpData(void)
{
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		if (reverse != false)
		{
			reverse = false;
		}
		pos.x += 1.0f;

		if (mode != fmode[0])
		{
			SetMode(fmode[0]);
		}
	}
	else if (CheckHitKey(KEY_INPUT_LEFT))
	{
		reverse = true;
		pos.x -= 1.0f;
		if (mode != fmode[0])
		{
			SetMode(fmode[0]);
		}
	}
	else
	{
		if (mode != fmode[2])
		{
			SetMode(fmode[2]);
		}
	}
	
}

// 状態のセット
void Player::SetMode(std::string m)
{
	flam = 0;
	mode = m;
}

// 状態の取得
std::string Player::GetMode(void)
{
	return mode;
}

// 中心点の取得
Position Player::Getcenter(void)
{
	Position dummy = { cut[mode][index].center.x - (int)pos.x, cut[mode][index].center.y - (int)pos.y };

	return dummy;
}
