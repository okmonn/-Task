#include "Character.h"

// �R���X�g���N�^
Character::Character() : 
	index(0), flam(0), speed(1.0f), reverse(false), oldPos(0.0f), st(State::wait)
{
}

// �f�X�g���N�^
Character::~Character()
{
}

// �X�e�[�^�X�̃Z�b�g
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
