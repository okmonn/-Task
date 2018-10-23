#include "GameMane.h"
#include "../Scene/Title.h"
#include "../../Func/Func.h"

// �E�B���h�E�T�C�Y
#define WINDOW_X 640
#define WINDOW_Y 480

// �R���X�g���N�^
GameMane::GameMane()
{
}

// �f�X�g���N�^
GameMane::~GameMane()
{
}

// �V�[���ڍs
void GameMane::ChangeScene(Scene * scene)
{
	this->scene.reset(scene);
}

// ������
void GameMane::Init(void)
{
	func::SetWindowSize(WINDOW_X, WINDOW_Y);
	func::Start();

	ChangeScene(new Title());
}

// �`��
void GameMane::Draw(void)
{
	func::Set();

	scene->Draw();

	func::Do();
}

// ����
void GameMane::UpData(void)
{
	Draw();

	scene->UpData();
}

// ����
void GameMane::Run(void)
{
	Init();

	while (func::CheckMsg() && func::CheckKey(INPUT_ESCAPE) != true)
	{
		UpData();
	}

	func::End();
}
