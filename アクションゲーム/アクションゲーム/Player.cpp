#include "Player.h"
#include "Load.h"

const char* actionPath = "�A�N�V����/player.act";
const float g = 0.5f;
const int line = 330;

// �R���X�g���N�^
Player::Player(std::weak_ptr<Input>in) : in(in)
{
	//�t���[�����̏�����
	flam = 0;

	//���W
	pos = { 50, 330 };

	//���
	fmode.clear();

	//���[�h
	data.clear();

	//�����f�[�^
	cut.clear();

	//�������`�f�[�^
	attack.clear();

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

	//�֐��|�C���^
	func = &Player::Wait;

	//�ҋ@�t���O
	wait = true;

	//�W�����v�t���O
	fly = false;

	//��`�̃T�C�Y
	attackSize = 2;


	Load();
}

// �f�X�g���N�^
Player::~Player()
{
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

	std::string path = FindString(actionPath, '/', 1, false) + Load::GetInstance()->GetHeader(actionPath).path;
	//�摜�f�[�^�̏�����
	image = LoadGraph(path.c_str());

	for (UINT i = 0; i < Load::GetInstance()->GetImageDataSize(actionPath); ++i)
	{
		fmode.push_back(Load::GetInstance()->GetImageData(actionPath, i).name);
		data[Load::GetInstance()->GetImageData(actionPath, i).name] = Load::GetInstance()->GetImageData(actionPath, i);
	}
	for (auto itr = data.begin(); itr != data.end(); ++itr)
	{
		cut[itr->first] = Load::GetInstance()->GetCutData(actionPath, itr->first);
	}

	attack = Load::GetInstance()->GetAttac(actionPath);

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
			if (mode != "Jump" && mode != "Crouch" && mode != " Damage" && mode != "Kick")
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
		(float)attackSize, 0.0f, image, true, reverse);

#ifdef _DEBUG
	DrawFormatString(10, 10, GetColor(255, 255, 255), "%d", index);
	DrawFormatString(30, 10, GetColor(255, 255, 255), "%d", center.x);
	DrawFormatString(50, 10, GetColor(255, 255, 255), "%d", (int)pos.y);
	DrawPixel((int)pos.x, (int)pos.y, GetColor(255, 255, 255));
	DrawFormatString(100, 10, GetColor(255, 255, 255), "%s", mode.c_str());
	DrawFormatString(200, 10, GetColor(255, 255, 255), "%d", fly);

	for (unsigned int i = 0; i < attack[mode][index].size(); ++i)
	{
		UINT color = 0;
		if (attack[mode][index][i].type == RectType::attack)
		{
			color = GetColor(255, 0, 0);
		}
		else if(attack[mode][index][i].type == RectType::damage)
		{
			color = GetColor(0, 255, 0);
		}
		else
		{
			color = GetColor(0, 0, 255);
		}

		if (reverse == false)
		{
			DrawBox((int)pos.x + (attack[mode][index][i].rect.GetLeft() * attackSize),
				(int)pos.y + (attack[mode][index][i].rect.GetTop() * attackSize),
				(int)pos.x + (attack[mode][index][i].rect.GetLeft() + attack[mode][index][i].rect.GetWidth()) * attackSize,
				(int)pos.y + (attack[mode][index][i].rect.GetTop() + attack[mode][index][i].rect.GetHeight()) * attackSize,
				color, false);
		}
		else
		{
			DrawBox((int)pos.x - (attack[mode][index][i].rect.GetLeft() * attackSize),
				(int)pos.y + (attack[mode][index][i].rect.GetTop() * attackSize),
				(int)pos.x - (attack[mode][index][i].rect.GetLeft() + attack[mode][index][i].rect.GetWidth()) * attackSize,
				(int)pos.y + (attack[mode][index][i].rect.GetTop() + attack[mode][index][i].rect.GetHeight()) * attackSize,
				color, false);
		}
	}
#endif
}

// �ҋ@�̏���
void Player::Wait(void)
{
	//����
	if (in.lock()->CheckTrigger(PAD_INPUT_RIGHT) && mode != "Jump")
	{
		SetMode("Walk");
		func = &Player::Walk;
	}
	else if (in.lock()->CheckTrigger(PAD_INPUT_LEFT) && mode != "Jump")
	{
		SetMode("Walk", true);
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

	//���Ⴊ��
	if (in.lock()->CheckTrigger(PAD_INPUT_DOWN))
	{
		SetMode("Crouch", reverse);
		func = &Player::Down;
	}
}

// �����̏���
void Player::Walk(void)
{
	if (mode != "Walk" || fly == true)
	{
		return;
	}

	if (in.lock()->CheckPress(PAD_INPUT_RIGHT))
	{
		if (reverse == true)
		{
			reverse = false;
		}
		pos.x += 1.0f;
	}
	else if (in.lock()->CheckPress(PAD_INPUT_LEFT))
	{
		if (reverse == false)
		{
			reverse = true;
		}
		pos.x += -1.0f;
	}
	else
	{
		SetMode("Wait", reverse);
		func = &Player::Wait;
	}

	//�W�����v
	if (in.lock()->CheckTrigger(PAD_INPUT_A))
	{
		SetMode("Jump", reverse);
		func = &Player::Jump;
	}
}

// �W�����v�̏���
void Player::Jump(void)
{
	if (mode != "Jump")
	{
		return;
	}
	if (fly == false)
	{
		fly = true;
	}
	pos += vel;
	vel.y += g;

	//���Ⴊ��
	if (in.lock()->CheckTrigger(PAD_INPUT_DOWN))
	{
		SetMode("Crouch", reverse);
		func = &Player::Down;
	}

	//�p���`
	if (in.lock()->CheckTrigger(PAD_INPUT_B))
	{
		SetMode("Punch", reverse);
		func = &Player::Punch;
	}

	//�X���C�f�B���O
	if (in.lock()->CheckTrigger(PAD_INPUT_D))
	{
		SetMode("Sliding", reverse);
		func = &Player::Sliding;
	}
}

// ���n�̏���
void Player::Ground(void)
{
	if (in.lock()->CheckTrigger(PAD_INPUT_A))
	{
		SetMode("Sliding", reverse);
		func = &Player::Sliding;
	}
}

// �p���`�̏���
void Player::Punch(void)
{
}

// �L�b�N�̏���
void Player::Kick(void)
{
	if ((int)flam >= cut[mode][cut[mode].size() - 1].flam)
	{
		SetMode("Crouch", reverse);
		func = &Player::Down;
	}
}

// �X���C�f�B���O�̏���
void Player::Sliding(void)
{
	pos.x += reverse == false ? 3.0f : -3.0f;
}

// ���Ⴊ�݂̏���
void Player::Down(void)
{
	if (!(in.lock()->CheckPress(PAD_INPUT_DOWN)) && pos.y >= line)
	{
		SetMode("Wait", reverse);
	}
	else
	{
		if (in.lock()->CheckTrigger(PAD_INPUT_B))
		{
			SetMode("Kick", reverse);
			func = &Player::Kick;
		}
	}

	if (in.lock()->CheckTrigger(PAD_INPUT_LEFT))
	{
		reverse = true;
	}
	else if (in.lock()->CheckTrigger(PAD_INPUT_RIGHT))
	{
		reverse = false;
	}
}

// �_���[�W�̏���
void Player::Damage(void)
{
	if (wait == false)
	{
		if (reverse == false)
		{
			pos.x -= 2.0f;
			
		}
		else
		{
			pos.x += 2.0f;
		}
	}
}

// ����
void Player::UpData()
{
	if (mode != "Jump")
	{
		if (pos.y < line)
		{
			pos.y += vel.y;
			vel.y += g;
		}
		else
		{
			fly = false;
		}
	}

	if (pos.y >= line)
	{
		SetPos((float)line);
	}

	if (wait == true)
	{
		func = &Player::Wait;
	}

	if (mode == "Damage")
	{
		func = &Player::Damage;
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

	if (mode == "Ground")
	{
		func = &Player::Ground;
	}

	flam = 0;
	index = 0;
	reverse = r;
	loop = true;
	if (reverse == false)
	{
		if (mode != "Crouch" && mode != "Kick" && mode != "Punch" && wait == false && fly == false)
		{
			vel = { 2.0f, -8.0f };
		}
	}
	else
	{
		if (mode != "Crouch" && mode != "Kick" && mode != "Punch" && wait == false && fly == false)
		{
			vel = { -2.0f, -8.0f };
		}
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

// �������`�̐�
int Player::GetAttackNum(void)
{
	return attack[mode][index].size();
}

// �������`�̎擾
Attack Player::GetAttack(USHORT num)
{
	return attack[mode][index][num];
}

// ��������W�̎擾
Positionf Player::GetAttackPos(USHORT num, bool flag)
{
	Positionf tmp;
	if (flag == false)
	{
		if (reverse == false)
		{
			tmp.x = pos.x + (attack[mode][index][num].rect.GetLeft() + attack[mode][index][num].rect.GetWidth()) * attackSize;
			tmp.y = pos.y + (attack[mode][index][num].rect.GetTop() + attack[mode][index][num].rect.GetHeight()) * attackSize;
		}
		else
		{
			tmp.x = pos.x - (attack[mode][index][num].rect.GetLeft() + attack[mode][index][num].rect.GetWidth()) * attackSize;
			tmp.y = pos.y + (attack[mode][index][num].rect.GetTop() + attack[mode][index][num].rect.GetHeight()) * attackSize;
		}
	}
	else
	{
		if (reverse == false)
		{
			tmp.x = pos.x + (attack[mode][index][num].rect.GetLeft()) * attackSize;
			tmp.y = pos.y + (attack[mode][index][num].rect.GetTop()) * attackSize;
		}
		else
		{
			tmp.x = pos.x - (attack[mode][index][num].rect.GetLeft()) * attackSize;
			tmp.y = pos.y + (attack[mode][index][num].rect.GetTop()) * attackSize;
		}
	}

	return tmp;
}

// �������̎擾
CutData Player::GetCut(void)
{
	return cut[mode][index];
}
