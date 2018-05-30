#include "Enemy.h"
#include "Load.h"
#include "DxLib.h"

// �R���X�g���N�^
Enemy::Enemy()
{
	//�摜
	image = 0;

	//�t���[��
	flam = 0;

	//�z��ԍ�
	index = 0;


	ppp = false;

	//���S�_
	center = {};

	//���]�t���O
	reverse = false;

	//���[�v�t���O
	loop = true;

	//���x
	vel = { 2.0f, -8.0f };

	//�ҋ@�t���O
	wait = true;

	//�W�����v�t���O
	fly = false;

	//��`�̃T�C�Y
	attackSize = 2;

	//���S�t���O
	die = false;

	//������t���O
	hit = false;

	//�摜�f�[�^
	data.clear();

	//�����f�[�^
	cut.clear();

	//�������`�f�[�^
	attack.clear();
}

// �f�X�g���N�^
Enemy::~Enemy()
{
}

// ������̌���
std::string Enemy::FindString(const std::string path, const char find, int offset, bool start)
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

// �ǂݍ���
void Enemy::Load(std::string s)
{
	if (Load::GetInstance()->LoadAct(s.c_str()) != true)
	{
		return;
	}

	std::string path = FindString(s.c_str(), '/', 1, false) + Load::GetInstance()->GetHeader(s).path;
	//�摜�f�[�^�̏�����
	image = LoadGraph(path.c_str());

	for (UINT i = 0; i < Load::GetInstance()->GetImageDataSize(s); ++i)
	{
		data[Load::GetInstance()->GetImageData(s, i).name] = Load::GetInstance()->GetImageData(s, i);
	}
	for (auto itr = data.begin(); itr != data.end(); ++itr)
	{
		cut[itr->first] = Load::GetInstance()->GetCutData(s, itr->first);
	}

	attack = Load::GetInstance()->GetAttac(s);
}

// �����蔻��
bool Enemy::CheackHit(Positionf& pos1, Attack& a1, Positionf& pos2, Attack& a2)
{
	if (reverse == true)
	{
		/*if (abs((pos1.x + a1.rect.pos.x * 2) - (pos2.x + a2.rect.pos.x * 2)) < (a1.rect.GetWidth() * 2 + a2.rect.GetWidth() * 2) / 2
			&& abs((pos1.y + a1.rect.pos.y * 2) - (pos2.y + a2.rect.pos.y * 2)) < (a1.rect.GetHeight() * 2 + a2.rect.GetHeight() * 2) / 2)
		{
			return true;
		}*/
		if (abs((pos1.x + a1.rect.pos.x) - (pos2.x + a2.rect.pos.x)) < (a1.rect.GetWidth() + a2.rect.GetWidth())
			&& abs((pos1.y + a1.rect.pos.y) - (pos2.y + a2.rect.pos.y)) < (a1.rect.GetHeight() + a2.rect.GetHeight()))
		{
			return true;
		}
	}
	else
	{
		/*if (abs((pos1.x - a1.rect.pos.x * 2) - (pos2.x - a2.rect.pos.x * 2)) < (a1.rect.GetWidth() * 2 + a2.rect.GetWidth() * 2) / 2
			&& abs((pos1.y + a1.rect.pos.y * 2) - (pos2.y + a2.rect.pos.y * 2)) < (a1.rect.GetHeight() * 2 + a2.rect.GetHeight() * 2) / 2)
		{
			return true;
		}*/
		if (abs((pos1.x - a1.rect.pos.x) - (pos2.x - a2.rect.pos.x)) < (a1.rect.GetWidth() + a2.rect.GetWidth())
			&& abs((pos1.y + a1.rect.pos.y) - (pos2.y + a2.rect.pos.y)) < (a1.rect.GetHeight() + a2.rect.GetHeight()))
		{
			return true;
		}
	}
	return false;
}

// �����蔻��
bool Enemy::CheackHit(Positionf & pos1, Positionf & pos2, Positionf & pos3, Positionf & pos4)
{
	if (pos1.x <= pos4.x
		&& pos2.x >= pos3.x
		&& pos1.y <= pos4.y
		&& pos2.y >= pos3.y)
	{
		return true;
	}

	return false;
}

// ���S�_�̐ݒ�
void Enemy::SetSenter(Position & pos, bool r)
{
	center = pos;
	if (r == true)
	{
		center.x = cut[mode][index].rect.GetWidth() - pos.x;
	}
}

// ���W�̐ݒ�
void Enemy::SetPos(Positionf pos)
{
	this->pos = pos;
}

// ���W�̐ݒ�
void Enemy::SetPos(float y)
{
	pos.y = y;
}

// ���W�̎擾
Positionf Enemy::GetPos(void)
{
	return pos;
}

// ��Ԃ̎擾
std::string Enemy::GetMode(void)
{
	return mode;
}

// �������̎擾
CutData Enemy::GetCutData(std::string m, UINT index)
{
	return cut[m][index];
}

// ���S�t���O�̎擾
bool Enemy::GetDie(void)
{
	return die;
}

// ������t���O�̎擾
bool Enemy::GetHit(void)
{
	return hit;
}

// ������t���O�̕ύX
void Enemy::SetHit(bool flg)
{
	hit = flg;
}

Positionf Enemy::GetCamPos(void)
{
	return camPos;
}

CutData Enemy::GetCut(void)
{
	return cut[mode][index];
}

bool Enemy::GetReverse(void)
{
	return reverse;
}

void Enemy::ChangeDir(void)
{
	if (reverse == true)
	{
		if (dir[0] == false)
		{
			dir[1] = false;
			dir[0] = true;
		}
		reverse = false;
	}
	else
	{
		if (dir[1] == false)
		{
			dir[0] = false;
			dir[1] = true;
		}
		reverse = true;
	}
	go = 1;
}
