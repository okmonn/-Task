#include "Character.h"

// コンストラクタ
Character::Character() : 
	index(0), flam(0), speed(1.0f), reverse(false), oldPos(0.0f), st(State::wait)
{
}

// デストラクタ
Character::~Character()
{
}

// ステータスのセット
void Character::SetState(const State & st)
{
	if (this->st == st)
	{
		return;
	}

	this->st  = st;
	index     = 0;
	flam      = 0;
	rectPos.x = rectSize.x * index;
	oldPos    = pos;
}
