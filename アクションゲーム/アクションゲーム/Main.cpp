#include "Main.h"
#include "Typedef.h"
#include "DxLib.h"

// �V�X�e���̏�����
bool System(void)
{
	//65536�FӰ�ނɐݒ�
	SetGraphMode(WINDOW_X, WINDOW_Y, 16);		

	//true:window�@false:�ٽ�ذ�
	ChangeWindowMode(true);

	//window�e�L�X�g�̐ݒ�
	SetWindowText("1601271_���ɒj");

	//Dxlib�̏�����
	if (DxLib_Init() == -1)
	{
		return false;
	}

	//�ЂƂ܂��ޯ��ޯ̧�ɕ`��
	SetDrawScreen(DX_SCREEN_BACK);		

	return true;
}

// ���C������
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	System();
	int a = 0;
	//���[�v����
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		
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