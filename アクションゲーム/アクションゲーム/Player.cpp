#include "Player.h"
#include "DxLib.h"
#include "Load.h"

// コンストラクタ
Player::Player()
{

	//画像データの初期化
	image = LoadGraph(_T("img/rick.png"));
	
	//フレーム数の初期化
	flam = 0;
	Load::Create();
	Load::GetInstance()->LoadAct("アクション/player.act");
}

// デストラクタ
Player::~Player()
{
	Load::Destroy();
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
