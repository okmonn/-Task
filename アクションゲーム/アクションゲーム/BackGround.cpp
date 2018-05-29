#include "BackGround.h"
#include "Typedef.h"
#include <tchar.h>

#define BACK_SIZE_X 576

// コンストラクタ
BackGround::BackGround(std::weak_ptr<Camera>cam) : cam(cam)
{
	//背景
	back = LoadGraph(_T("img/splatterhouse.png"));

	//座標
	pos = { 0,0 };
	pos.x = this->cam.lock()->GetPos().x;
	z = this->cam.lock()->GetPos().x;;
}

// デストラクタ
BackGround::~BackGround()
{
	DeleteGraph(back);
}

// 座標のセット
void BackGround::SetPos(Positionf pos)
{
	this->pos.x = pos.x;
}

// 描画
void BackGround::Draw(void)
{
	//背景
	DrawGraph(-((int)pos.x % (BACK_SIZE_X * 2)), (int)pos.y, back, false);
	DrawTurnGraph(-((int)pos.x % (BACK_SIZE_X * 2)) + BACK_SIZE_X, (int)pos.y, back, false);
	DrawGraph(-((int)pos.x % (BACK_SIZE_X * 2)) + BACK_SIZE_X * 2, (int)pos.y, back, false);
	DrawTurnGraph(-((int)pos.x % (BACK_SIZE_X * 2)) + BACK_SIZE_X * 3, (int)pos.y, back, false);
}

// 処理
void BackGround::UpData(void)
{
	pos.x = cam.lock()->GetPos().x;
}
