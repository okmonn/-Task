#include "Player.h"
#include "DxLib.h"
#include "Typedef.h"

// コンストラクタ
Player::Player()
{
	//画像データの初期化
	image = LoadGraph("img/rick.png");
	
	//フレーム数の初期化
	flam = 0;
}

// デストラクタ
Player::~Player()
{
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
