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
