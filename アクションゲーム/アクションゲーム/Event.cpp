#include "Event.h"
#include "Load.h"

// コンストラクタ
Event::Event()
{
	//画像
	image = 0;

	//座標
	pos = { 0.0f,0.0f };

	//カメラ座標
	camPos = pos;

	//矩形サイズ
	attackSize = 2;

	//フレーム
	flam = 0;

	//配列番号
	index = 0;

	//画像データ
	data.clear();

	//分割データ
	cut.clear();

	//あたり矩形データ
	attack.clear();
}

// デストラクタ
Event::~Event()
{
}

// 文字列の検索
std::string Event::FindString(const std::string path, const char find, int offset, bool start)
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
void Event::Load(std::string fileName)
{
	if (Load::GetInstance()->LoadAct(fileName.c_str()) != true)
	{
		return;
	}

	std::string path = FindString(fileName.c_str(), '/', 1, false) + Load::GetInstance()->GetHeader(fileName).path;
	//画像データの初期化
	image = LoadGraph(path.c_str());

	for (UINT i = 0; i < Load::GetInstance()->GetImageDataSize(fileName); ++i)
	{
		data[Load::GetInstance()->GetImageData(fileName, i).name] = Load::GetInstance()->GetImageData(fileName, i);
	}
	for (auto itr = data.begin(); itr != data.end(); ++itr)
	{
		cut[itr->first] = Load::GetInstance()->GetCutData(fileName, itr->first);
	}

	attack = Load::GetInstance()->GetAttac(fileName);
}

// あたり判定
bool Event::CheackHit(Positionf& pos1, Attack& a1, Positionf& pos2, Attack& a2)
{
	if (abs((pos1.x + a1.rect.pos.x) - (pos2.x + a2.rect.pos.x)) < (a1.rect.GetWidth() + a2.rect.GetWidth())
		&& abs((pos1.y + a1.rect.pos.y) - (pos2.y + a2.rect.pos.y)) < (a1.rect.GetHeight() + a2.rect.GetHeight()))
	{
		return true;
	}

	return false;
}

bool Event::CheackHit2(Positionf & pos1, Attack & a1, Positionf & pos2, Attack & a2)
{
	if (abs((pos1.x + a1.rect.pos.x) - (pos2.x + a2.rect.pos.x)) < (a1.rect.GetWidth() / 2 + a2.rect.GetWidth())
		&& abs((pos1.y + a1.rect.pos.y) - (pos2.y + a2.rect.pos.y)) < (a1.rect.GetHeight() / 2 + a2.rect.GetHeight()))
	{
		return true;
	}

	return false;
}

Positionf Event::GetCamPos(void)
{
	return camPos;
}

CutData Event::GetCut(void)
{
	return cut[mode][index];
}
