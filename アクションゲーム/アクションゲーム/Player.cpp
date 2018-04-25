#include "Player.h"
#include "Load.h"

const char* actionPath = "�A�N�V����/player.act";

// �R���X�g���N�^
Player::Player()
{
	//�t���[�����̏�����
	flam = 0;

	//���W
	pos = {50, 330};

	//����
	rect = {};

	//���
	fmode.clear();

	//���[�h
	data.clear();

	//�����f�[�^
	cut.clear();

	//�z��ԍ�
	index = 0;

	//���S�_
	center = {};

	//���]�t���O
	reverse = false;

	if (Load::GetInstance() == nullptr)
	{
		Load::Create();
	}

	Load();
}

// �f�X�g���N�^
Player::~Player()
{
	Load::Destroy();
	DeleteGraph(image);
}

// ������̌���
std::string Player::FindString(const std::string path, const char find, int offset, bool start)
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
void Player::Load(void)
{
	if (Load::GetInstance()->LoadAct(actionPath) != true)
	{
		return;
	}

	std::string path = FindString(actionPath, '/', 1, false) + Load::GetInstance()->GetHeader().path;
	//�摜�f�[�^�̏�����
	image = LoadGraph(path.c_str());

	for (UINT i = 0; i < Load::GetInstance()->GetImageDataSize(); ++i)
	{
		fmode.push_back(Load::GetInstance()->GetImageData(i).name);
		data[Load::GetInstance()->GetImageData(i).name] = Load::GetInstance()->GetImageData(i);
	}
	for (auto itr = data.begin(); itr != data.end(); ++itr)
	{
		cut[itr->first] = Load::GetInstance()->GetCutData(itr->first);
	}

	SetMode(fmode[2]);
}

// �`��
void Player::Draw(void)
{
	index = (flam++ / cut[mode][0].flam) % cut[mode].size();

	DrawRectRotaGraph2((int)pos.x, (int)pos.y, 
		cut[mode][index].rect.GetLeft(), cut[mode][index].rect.GetTop(), 
		cut[mode][index].rect.GetWidth(), cut[mode][index].rect.GetHeight(), 
		center.x, center.y, 
		2.0f, 0.0f, image, true, reverse);

	DrawFormatString(10, 10, GetColor(255, 255, 255), "%d", index);
	DrawFormatString(30, 10, GetColor(255, 255, 255), "%d", Getcenter().x);
	DrawFormatString(50, 10, GetColor(255, 255, 255), "%d", Getcenter().y);
}

// ����
void Player::UpData(void)
{
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		SetMode(fmode[0]);
		pos.x += 1.0f;
	}
	else if (CheckHitKey(KEY_INPUT_LEFT))
	{
		SetMode(fmode[0], true);
		pos.x -= 1.0f;
	}
	else
	{
		SetMode(fmode[2], reverse);
	}
	
}

// ��Ԃ̃Z�b�g
void Player::SetMode(std::string m, bool r)
{
	if (mode == m && reverse == r)
	{
		return;
	}
	index = 0;
	flam = 0;
	mode = m; 
	center = cut[mode][index].center;
	reverse = r;
	if(reverse == true)
	{
		center.x = cut[mode][index].rect.GetWidth() - center.x;
	}
}

// ��Ԃ̎擾
std::string Player::GetMode(void)
{
	return mode;
}

// ���S�_�̎擾
Position Player::Getcenter(void)
{
	Position dummy = { cut[mode][index].center.x + (int)pos.x, cut[mode][index].center.y + (int)pos.y };

	return dummy;
}

// ��Ԉꗗ�̎擾
std::vector<std::string> Player::GetAllMode(void)
{
	return fmode;
}
