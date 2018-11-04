#include "Character.h"

// コンストラクタ
Character::Character() : 
	index(0), flam(0), speed(1.0f), rect(0.0f), st(State::wait)
{
}

// デストラクタ
Character::~Character()
{
}
