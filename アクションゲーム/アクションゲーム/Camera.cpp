#include "Camera.h"
#include "Player.h"
#include "DxLib.h"
#include <cassert>

// コンストラクタ
Camera::Camera(std::weak_ptr<Stage>st) : st(st)
{
}

// デストラクタ
Camera::~Camera()
{
}
// 中心オブジェクトの設定
void Camera::SetFocus(std::shared_ptr<Player>pl)
{
	this->pl = pl;
}

// 座標の取得
Positionf & Camera::GetPos(void)
{
	return pos;
}

// 目視サイズの取得
Rect & Camera::GetViewSize(void)
{
	return st.lock()->GetStageRange();
}

// 描画
void Camera::Draw(void)
{
	DrawFormatString(620, 50, GetColor(0, 255, 0), "%d", (int)pos.x);
	DrawFormatString(650, 50, GetColor(0, 255, 0), "%d", (int)pos.y);
}

// 処理
void Camera::UpData(void)
{
	assert(!pl.expired());
	auto& range = st.lock()->GetStageRange();
	
	pos = pl.lock()->GetPos();
	//カメラ座標の補正 
	if (pos.x - WINDOW_X / 2 < range.GetLeft()) 
	{
		pos.x = (float)(range.GetLeft() + WINDOW_X / 2);
	}
	else if (pos.x + WINDOW_X / 2 > range.GetRight()) 
	{
		pos.x = (float)(range.GetRight() - WINDOW_X / 2);
	}
}

// 座標修正
Positionf Camera::CorrectionPos(Positionf pos)
{
	//座標からカメラの座標を引く
	Positionf tmp = pos;
	
	if (tmp.x > WINDOW_X / 2)
	{
		tmp.x -= this->pos.x - WINDOW_X / 2;
	}

	return tmp;
}
