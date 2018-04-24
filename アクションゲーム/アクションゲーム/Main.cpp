#define EXPORT_MAIN
#include "Main.h"
#include "Typedef.h"
#include "DxLib.h"

// �V�X�e���̏�����
bool System(void)
{
	//�O���t�B�b�N���[�h�̐ݒ�
	SetGraphMode(WINDOW_X, WINDOW_Y, 32);		

	//true:window�@false:�ٽ�ذ�
	ChangeWindowMode(true);

	//window�e�L�X�g�̐ݒ�
	SetWindowText(_T("1601271_���ɒj"));

	//Dxlib�̏�����
	if (DxLib_Init() == -1)
	{
		return false;
	}

	//�ЂƂ܂��ޯ��ޯ̧�ɕ`��
	SetDrawScreen(DX_SCREEN_BACK);		
	
	return true;
}

// �C���X�^���X��
void Create(void)
{
	//�w�i�N���X
	back = std::make_shared<BackGround>();

	//�v���C���[�N���X
	pl = std::make_shared<Player>();
}

// �`��
void Draw(void)
{
	//��ʏ���
	ClsDrawScreen();

	back->Draw();
	pl->Draw();

	//����ʂ�\��ʂɏu�ԃR�s�[
	ScreenFlip();
}

// ����
void UpData(void)
{
	Draw();


}

// ���C������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	System();
	Create();
	
	//���[�v����
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_RETURN) == 0)
	{
		UpData();
	}

	Destroy();

	return 0;
}

// �I������
void Destroy(void)
{
	//Dxlib�̏I��
	DxLib_End();
}