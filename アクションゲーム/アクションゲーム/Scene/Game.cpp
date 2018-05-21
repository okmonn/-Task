#include "Game.h"
#include "DxLib.h"
#include "../Load.h"
#include "../EnemyMane.h"
#include "Title.h"

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

	//�C���v�b�g
	in = std::make_shared<Input>();

	ChangeScene(new Title(in));
}

// �V�[���̈ڍs
void Game::ChangeScene(Scene * s)
{
	scene.reset(s);
}

// ���ꂼ��̃N���X�̕`��
void Game::Draw(void)
{
	//��ʏ���
	ClsDrawScreen();

	scene->Draw();

	//����ʂ�\��ʂɏu�ԃR�s�[
	ScreenFlip();
}

// ���ꂼ��̃N���X�̏���
void Game::UpData(void)
{
	Draw();

	in->UpData();

	scene->UpData();
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
