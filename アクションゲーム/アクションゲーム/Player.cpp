#include "Player.h"
#include "DxLib.h"
#include "Load.h"

const char* actionPath = "�A�N�V����/player.act";

// �R���X�g���N�^
Player::Player()
{
	//�t���[�����̏�����
	flam = 0;

	//���W
	pos = {};

	//����
	rect = {};

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

	std::string path = FindString(actionPath, '/', 1, false) + FindString(Load::GetInstance()->GetHeader().path, '/', 1);
	//�摜�f�[�^�̏�����
	image = LoadGraph(path.c_str());

}

// �`��
void Player::Draw(void)
{
	DrawRotaGraph2(0, 0, 50, 50, 2.0f, 0.0f, image, true);
	
	++flam;
}

// ����
void Player::UpData(void)
{
}