#include "Main.h"
#include "Typedef.h"
#include "DxLib.h"

// システムの初期化
bool System(void)
{
	//65536色ﾓｰﾄﾞに設定
	SetGraphMode(WINDOW_X, WINDOW_Y, 16);		

	//true:window　false:ﾌﾙｽｸﾘｰﾝ
	ChangeWindowMode(true);

	//windowテキストの設定
	SetWindowText("1601271_岡繁男");

	//Dxlibの初期化
	if (DxLib_Init() == -1)
	{
		return false;
	}

	//ひとまずﾊﾞｯｸﾊﾞｯﾌｧに描画
	SetDrawScreen(DX_SCREEN_BACK);		

	return true;
}

// メイン処理
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	System();
	int a = 0;
	//ループ処理
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		
	}

	Destroy();

	return 0;
}

// 終了処理
void Destroy(void)
{
	//Dxlibの終了
	DxLib_End();
}