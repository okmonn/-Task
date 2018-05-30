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


	ppp = false;

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

	//死亡フラグ
	die = false;

	//あたりフラグ
	hit = false;

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

// あたり判定
bool Enemy::CheackHit(Positionf& pos1, Attack& a1, Positionf& pos2, Attack& a2)
{
	if (reverse == true)
	{
		/*if (abs((pos1.x + a1.rect.pos.x * 2) - (pos2.x + a2.rect.pos.x * 2)) < (a1.rect.GetWidth() * 2 + a2.rect.GetWidth() * 2) / 2
			&& abs((pos1.y + a1.rect.pos.y * 2) - (pos2.y + a2.rect.pos.y * 2)) < (a1.rect.GetHeight() * 2 + a2.rect.GetHeight() * 2) / 2)
		{
			return true;
		}*/
		if (abs((pos1.x + a1.rect.pos.x) - (pos2.x + a2.rect.pos.x)) < (a1.rect.GetWidth() + a2.rect.GetWidth())
			&& abs((pos1.y + a1.rect.pos.y) - (pos2.y + a2.rect.pos.y)) < (a1.rect.GetHeight() + a2.rect.GetHeight()))
		{
			return true;
		}
	}
	else
	{
		/*if (abs((pos1.x - a1.rect.pos.x * 2) - (pos2.x - a2.rect.pos.x * 2)) < (a1.rect.GetWidth() * 2 + a2.rect.GetWidth() * 2) / 2
			&& abs((pos1.y + a1.rect.pos.y * 2) - (pos2.y + a2.rect.pos.y * 2)) < (a1.rect.GetHeight() * 2 + a2.rect.GetHeight() * 2) / 2)
		{
			return true;
		}*/
		if (abs((pos1.x - a1.rect.pos.x) - (pos2.x - a2.rect.pos.x)) < (a1.rect.GetWidth() + a2.rect.GetWidth())
			&& abs((pos1.y + a1.rect.pos.y) - (pos2.y + a2.rect.pos.y)) < (a1.rect.GetHeight() + a2.rect.GetHeight()))
		{
			return true;
		}
	}
	return false;
}

// あたり判定
bool Enemy::CheackHit(Positionf & pos1, Positionf & pos2, Positionf & pos3, Positionf & pos4)
{
	if (pos1.x <= pos4.x
		&& pos2.x >= pos3.x
		&& pos1.y <= pos4.y
		&& pos2.y >= pos3.y)
	{
		return true;
	}

	return false;
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

// 死亡フラグの取得
bool Enemy::GetDie(void)
{
	return die;
}

// あたりフラグの取得
bool Enemy::GetHit(void)
{
	return hit;
}

// あたりフラグの変更
void Enemy::SetHit(bool flg)
{
	hit = flg;
}

Positionf Enemy::GetCamPos(void)
{
	return camPos;
}

CutData Enemy::GetCut(void)
{
	return cut[mode][index];
}

bool Enemy::GetReverse(void)
{
	return reverse;
}

void Enemy::ChangeDir(void)
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
