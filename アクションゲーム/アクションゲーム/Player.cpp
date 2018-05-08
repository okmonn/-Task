#include "Player.h"
#include "Load.h"

const char* actionPath = "�A�N�V����/player.act";
const float g = 0.5f;

// �R���X�g���N�^
Player::Player(std::weak_ptr<Input>in) : in(in)
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

	//���[�v�t���O
	loop = true;

	//���x
	vel = {2.0f, -10.0f};

	//�֐��|�C���^
	func = &Player::Wait;

	//�ҋ@�t���O
	wait = true;

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

	SetMode("Wait");
}

// �`��
void Player::Draw(void)
{
	if (data[mode].loop)
	{
		if (wait == false)
		{
			++flam;
		}
		index = (flam / cut[mode][index].flam) % cut[mode].size();
		SetCenter(cut[mode][index].center, reverse);
	}
	else
	{
		if (index < cut[mode].size() && loop == true)
		{
			flam++;
			if ((int)flam >= cut[mode][index].flam)
			{
				if (cut[mode].size() - 1 > index)
				{

					if (mode == "Ground")
					{
						int i = 0;
					}
					++index;
					flam = 0;
				}
				else
				{
					loop = false;
				}
			}
			SetCenter(cut[mode][index].center, reverse);
		}
		else
		{
			if (mode != "Jump")
			{
				SetMode("Wait", reverse);
			}
			else
			{
				index = cut[mode].size() - 1;
			}
		}
	}

	DrawRectRotaGraph2((int)pos.x, (int)pos.y, 
		cut[mode][index].rect.GetLeft(), cut[mode][index].rect.GetTop(), 
		cut[mode][index].rect.GetWidth(), cut[mode][index].rect.GetHeight(), 
		center.x, center.y, 
		2.0f, 0.0f, image, true, reverse);

	DrawFormatString(10, 10, GetColor(255, 255, 255), "%d", index);
	DrawFormatString(30, 10, GetColor(255, 255, 255), "%d", center.x);
	DrawFormatString(50, 10, GetColor(255, 255, 255), "%d", (int)pos.y);
	DrawPixel((int)pos.x, (int)pos.y, GetColor(255, 255, 255));
	DrawFormatString(100, 10, GetColor(255, 255, 255), "%s", mode.c_str());
}

// �ҋ@�̏���
void Player::Wait(void)
{
	//����
	if (in.lock()->CheckTrigger(PAD_INPUT_RIGHT) && mode != "Jump")
	{
		SetMode(fmode[0]);
		func = &Player::Walk;
	}
	else if (in.lock()->CheckTrigger(PAD_INPUT_LEFT) && mode != "Jump")
	{
		SetMode(fmode[0], true);
		func = &Player::Walk;
	}

	//�W�����v
	if (in.lock()->CheckTrigger(PAD_INPUT_A))
	{
		SetMode("Jump", reverse);
		func = &Player::Jump;
	}

	//�p���`
	if (in.lock()->CheckTrigger(PAD_INPUT_B) && mode != "Punch")
	{
		SetMode("Punch", reverse);
		func = &Player::Punch;
	}
}

// �����̏���
void Player::Walk(void)
{
	if (mode != "Walk")
	{
		return;
	}

	if (in.lock()->CheckPress(PAD_INPUT_RIGHT) || in.lock()->CheckPress(PAD_INPUT_LEFT))
	{
		pos.x += reverse == false ? 1.0f : -1.0f;
	}
	else
	{
		SetMode("Wait", reverse);
		func = &Player::Wait;
	}
}

// �W�����v�̏���
void Player::Jump(void)
{
	if (mode != "Jump")
	{
		return;
	}
	pos += vel;
	vel.y += g;
}

// �p���`�̏���
void Player::Punch(void)
{
}

// �L�b�N�̏���
void Player::Kick(void)
{
}

// �X���C�f�B���O�̏���
void Player::Sliding(void)
{
}

// ���Ⴊ�݂̏���
void Player::Down(void)
{
}

// �_���[�W�̏���
void Player::Damage(void)
{
}

// ��Ԃ̕ύX
void Player::ChangeMode(void)
{
	
}

// ����
void Player::UpData()
{
	if (wait == true)
	{
		func = &Player::Wait;
	}

	(this->*func)();
}

// ���S���W�̃Z�b�g
void Player::SetCenter(Position & pos, bool r)
{
	center = pos;
	if (r == true)
	{
		center.x = cut[mode][index].rect.GetWidth() - pos.x;
	}
}

// ��Ԃ̃Z�b�g
void Player::SetMode(std::string m, bool r)
{
	if (mode == m && reverse == r && wait == false)
	{
		return;
	}

	if (m == "Wait")
	{
		wait = true;
		mode = "Walk";
	}
	else
	{
		wait = false;
		mode = m;
	}

	flam = 0;
	index = 0;
	reverse = r;
	loop = true;
	if (reverse == false)
	{
		vel = { 2.0f, -10.0f };
	}
	else
	{
		vel = { -2.0f, -10.0f };
	}
	SetCenter(cut[mode][index].center, reverse);
}

// ��Ԃ̎擾
std::string Player::GetMode(void)
{
	return mode;
}

// ���W�̎擾
Positionf Player::GetPos(void)
{
	return pos;
}

// ���W�̃Z�b�g
void Player::SetPos(Positionf pos)
{
	this->pos = pos;
}

// ���W�̃Z�b�g
void Player::SetPos(float y)
{
	pos.y = y;
}

// ��Ԉꗗ�̎擾
std::vector<std::string> Player::GetAllMode(void)
{
	return fmode;
}

// ���]�t���O�̎擾
bool Player::GetReverse(void)
{
	return reverse;
}
