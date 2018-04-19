#define EXPORT_MAIN
#include "Main.h"
#include "Typedef.h"
#include "DxLib.h"
int a = 0;
int b = 0;
int c = 0;
// �V�X�e���̏�����
bool System(void)
{
	//�O���t�B�b�N���[�h�̐ݒ�
	SetGraphMode(WINDOW_X, WINDOW_Y, 32);		

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
	a = LoadGraph("img/splatterhouse.png");
	b = LoadGraph("img/bar_bottom.png");
	c = LoadGraph("img/bar_top.png");
	return true;
}

// �`��
void Draw(void)
{
	//��ʏ���
	ClsDrawScreen();
	DrawGraph(0, 0, a, true);
	DrawTurnGraph(576, 0,  a, false);
	DrawGraph(0, 0, c, false);
	DrawGraph(0, 448 - 64, b, false);
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