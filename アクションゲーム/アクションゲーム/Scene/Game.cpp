#include "Game.h"
#include "DxLib.h"

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
	//�w�i�N���X
	back = std::make_shared<BackGround>();

	//UI�N���X
	ui = std::make_shared<Interface>();

	//�C���v�b�g
	in = std::make_shared<Input>();

	//�v���C���[�N���X
	pl = std::make_shared<Player>(in);

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
	ground->Draw();

	//����ʂ�\��ʂɏu�ԃR�s�[
	ScreenFlip();
}

// ���ꂼ��̃N���X�̏���
void Game::UpData(void)
{
	Draw();

	in->UpData();

	pl->UpData();

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
}

// �I������
void Game::Destroy(void)
{
	//Dxlib�̏I��
	DxLib_End();
}
