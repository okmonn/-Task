#include "Game.h"
#include "DxLib.h"
#include "../Load.h"
#include "../EnemyMane.h"

const int SIZE_X = 768;
const int SIZE_Y = 448;

// �R���X�g���N�^
Game::Game()
{
}

// �R���X�g���N�^
Game::Game(const Game&)
{
}

// �f�X�g���N�^
Game::~Game()
{
}

void Game::Init(void)
{
	//�O���t�B�b�N���[�h�̐ݒ�
	SetGraphMode(SIZE_X, SIZE_Y, 32);

	//true:window�@false:�ٽ�ذ�
	ChangeWindowMode(true);

	//window�e�L�X�g�̐ݒ�
	SetWindowText(_T("1601271_���ɒj"));

	//Dxlib�̏�����
	if (DxLib_Init() == -1)
	{
		return;
	}

	//�ЂƂ܂��ޯ��ޯ̧�ɕ`��
	SetDrawScreen(DX_SCREEN_BACK);


	Create();
}

// �N���X�̃C���X�^���X��
void Game::Create(void)
{
	//���[�h�N���X
	Load::Create();

	// �G�̊Ǘ��N���X
	EnemyMane::Create();

	//�w�i�N���X
	back = std::make_shared<BackGround>();

	//UI�N���X
	ui = std::make_shared<Interface>();

	//�C���v�b�g
	in = std::make_shared<Input>();

	//�v���C���[�N���X
	pl = std::make_shared<Player>(in);

	//�G�N���X
	e_list.push_back(EnemyMane::GetInstance()->CreateDeadman(300, 330, pl));
	e_list.push_back(EnemyMane::GetInstance()->CreateDeadman(500, 330, pl));

	//�n�ʃN���X
	ground = std::make_shared<Ground>(pl);
}

// ���ꂼ��̃N���X�̕`��
void Game::Draw(void)
{
	//��ʏ���
	ClsDrawScreen();

	back->Draw();
	ui->Draw();
	pl->Draw();
	for (auto itr = e_list.begin(); itr != e_list.end(); ++itr)
	{
		(*itr)->Draw();
	}
	ground->Draw();

#ifdef _DEBUG
	DrawFormatString(600, 10, GetColor(0, 255, 0), "%d", e_list.size());
#endif
	//����ʂ�\��ʂɏu�ԃR�s�[
	ScreenFlip();
}

// ���ꂼ��̃N���X�̏���
void Game::UpData(void)
{
	Draw();

	in->UpData();

	pl->UpData();

	if (pl->GetPos().x == 300)
	{
		if (e_list.size() < 10)
		{
			e_list.push_back(EnemyMane::GetInstance()->CreateDeadman(50, 330, pl));
		}
	}

	for (auto itr = e_list.begin(); itr != e_list.end();)
	{
		(*itr)->UpData();
		if ((*itr)->GetDie() == true)
		{
			itr = e_list.erase(itr);
		}
		else
		{
			++itr;
		}
	}

	ground->UpData();
}

// ���C�����[�v
void Game::Run(void)
{
	//���[�v����
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_RETURN) == 0)
	{
		UpData();
	}

	Destroy();
}

// �I������
void Game::Destroy(void)
{
	EnemyMane::Destroy();
	Load::Destroy();

	//Dxlib�̏I��
	DxLib_End();
}
