#include "Spear.h"

const std::string path = "�A�N�V����/pike.act";
const int line = 330;

// �R���X�g���N�^
Spear::Spear(Positionf pos, std::weak_ptr<Player>pl, std::weak_ptr<Camera>cam) : pl(pl), cam(cam)
{
	Load(path);
	this->pos = pos;
}

// �f�X�g���N�^
Spear::~Spear()
{
}

// ��Ԃ̃Z�b�g
void Spear::SetMode(std::string m, bool r)
{
}

// �`��
void Spear::Draw(void)
{
}

// ����
void Spear::UpData(void)
{
}
