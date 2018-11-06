#include "Player.h"
#include "../../Func/Func.h"
#include "../Camera/Camerah.h"
#include "../../Func/Func.h"

// 各ステータスごとのアニメーション数
const int anim[] = {
	4, 6, 5, 4, 1, 1
};

// アニメーション遷移フレーム
const int animTime = 10;

// ノックバックの威力
const float nockBack = 10.0f;

// コンストラクタ
Player::Player(std::weak_ptr<Camera> cam, const Vec2f & pos, const Vec2f & size)
{
	this->cam = cam;

	this->pos = pos;
	this->size = size * this->cam.lock()->GetLarge();
	rectPos = 0.0f;
	rectSize = { 100.0f, 59.0f };

	Init();
}

// デストラクタ
Player::~Player()
{
	func::DeleteImg(image);
}

// 初期化
void Player::Init(void)
{
	func::LoadImg("Rsc/Player.png", image);

	updata = &Player::Wait;
}

// アニメーションの終了フラグ
bool Player::CheckAnimEnd(void)
{
	if (index + 1 >= anim[(int)st]
		&& flam + 1 >= animTime)
	{
		return true;
	}

	return false;
}

// 描画
void Player::Draw(void)
{
	++flam;
	lpos = cam.lock()->ChangeLocal(pos);
	rectPos = { rectSize.x * index, rectSize.y * (int)st };
	func::DrawRectImg(image, lpos.x, lpos.y, size.x, size.y, rectPos.x, rectPos.y, rectSize.x, rectSize.y, reverse);
	if (flam >= animTime)
	{
		index = (index + 1 >= anim[(int)st]) ? 0 : ++index;
		flam = 0;
	}
}

// 処理
void Player::UpData(void)
{
	(this->*updata)();
}

// 待機
void Player::Wait(void)
{
	if (st != State::wait)
	{
		return;
	}

	//攻撃
	if (func::CheckKey(INPUT_Z))
	{
		SetState(State::attack);
		updata = &Player::Attack;
	}

	//移動
	if (func::CheckKey(INPUT_RIGHT))
	{
		reverse = false;
		SetState(State::walk);
		updata = &Player::Walk;
	}
	else if (func::CheckKey(INPUT_LEFT))
	{
		reverse = true;
		SetState(State::walk);
		updata = &Player::Walk;
	}

	//しゃがみ
	if (func::CheckKey(INPUT_DOWN))
	{
		SetState(State::crouch);
		updata = &Player::Crouch;
	}
}

// 歩き
void Player::Walk(void)
{
	if (st != State::walk)
	{
		return;
	}

	if (func::CheckKey(INPUT_RIGHT))
	{
		reverse = false;
		pos.x += (pos.x + speed - size.x / 2.0f <= func::GetWinSizeX() * cam.lock()->GetLarge()) ? speed : 0.0f;
	}
	else if (func::CheckKey(INPUT_LEFT))
	{
		reverse = true;
		pos.x -= (pos.x - speed + size.x / 2.0f >= 0.0f) ? speed : 0.0f;
	}
	else
	{
		SetState(State::wait);
		updata = &Player::Wait;
	}
}

// 攻撃
void Player::Attack(void)
{
	if (st != State::attack)
	{
		return;
	}

	if (CheckAnimEnd() == true)
	{
		SetState(State::wait);
		updata = &Player::Wait;
	}
}

// しゃがみ
void Player::Crouch(void)
{
	if (st != State::crouch)
	{
		return;
	}

	if (!func::CheckKey(INPUT_DOWN))
	{
		SetState(State::wait);
		updata = &Player::Wait;
	}
}

// ダメージ
void Player::Damage(void)
{
	if (st != State::damage)
	{
		return;
	}

	if (reverse == false)
	{
		pos.x -= nockBack / animTime;
		if (std::fabsf(pos.x - oldPos.x) >= nockBack)
		{
			SetState(State::wait);
			updata = &Player::Wait;
		}
	}
}
