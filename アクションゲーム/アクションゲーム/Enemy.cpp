#include "Enemy.h"
#include "Load.h"
#include "DxLib.h"

// コンストラクタ
Enemy::Enemy()
{
	//画像
	image = 0;

	//フレーム
	flam = 0;

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

	//待機フラグ
	wait = true;

	//ジャンプフラグ
	fly = false;

	//矩形のサイズ
	attackSize = 2;

	//画像データ
	data.clear();

	//分割データ
	cut.clear();

	//あたり矩形データ
	attack.clear();
}

// デストラクタ
Enemy::~Enemy()
{
}

// 文字列の検索
std::string Enemy::FindString(const std::string path, const char find, int offset, bool start)
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
void Enemy::Load(std::string s)
{
	if (Load::GetInstance()->LoadAct(s.c_str()) != true)
	{
		return;
	}

	std::string path = FindString(s.c_str(), '/', 1, false) + Load::GetInstance()->GetHeader(s).path;
	//画像データの初期化
	image = LoadGraph(path.c_str());

	for (UINT i = 0; i < Load::GetInstance()->GetImageDataSize(s); ++i)
	{
		data[Load::GetInstance()->GetImageData(s, i).name] = Load::GetInstance()->GetImageData(s, i);
	}
	for (auto itr = data.begin(); itr != data.end(); ++itr)
	{
		cut[itr->first] = Load::GetInstance()->GetCutData(s, itr->first);
	}

	attack = Load::GetInstance()->GetAttac(s);
}

// 中心点の設定
void Enemy::SetSenter(Position & pos, bool r)
{
	center = pos;
	if (r == true)
	{
		center.x = cut[mode][index].rect.GetWidth() - pos.x;
	}
}

// 座標の設定
void Enemy::SetPos(Positionf pos)
{
	this->pos = pos;
}

// 座標の設定
void Enemy::SetPos(float y)
{
	pos.y = y;
}

// 座標の取得
Positionf Enemy::GetPos(void)
{
	return pos;
}

// 状態の取得
std::string Enemy::GetMode(void)
{
	return mode;
}

// 分割情報の取得
CutData Enemy::GetCutData(std::string m, UINT index)
{
	return cut[m][index];
}
