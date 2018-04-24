#include "Player.h"
#include "DxLib.h"
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

	std::string path = FindString(actionPath, '/', 1, false) + FindString(Load::GetInstance()->GetHeader().path, '/', 1);
	//画像データの初期化
	image = LoadGraph(path.c_str());

}

// 描画
void Player::Draw(void)
{
	DrawRotaGraph2(0, 0, 50, 50, 2.0f, 0.0f, image, true);
	
	++flam;
}

// 処理
void Player::UpData(void)
{
}