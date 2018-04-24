#define EXPORT_MAIN
#include "Main.h"
#include "Typedef.h"
#include "DxLib.h"

// システムの初期化
bool System(void)
{
	//グラフィックモードの設定
	SetGraphMode(WINDOW_X, WINDOW_Y, 32);		

	//true:window　false:ﾌﾙｽｸﾘｰﾝ
	ChangeWindowMode(true);

	//windowテキストの設定
	SetWindowText(_T("1601271_岡繁男"));

	//Dxlibの初期化
	if (DxLib_Init() == -1)
	{
		return false;
	}

	//ひとまずﾊﾞｯｸﾊﾞｯﾌｧに描画
	SetDrawScreen(DX_SCREEN_BACK);		
	
	return true;
}

// インスタンス化
void Create(void)
{
	//背景クラス
	back = std::make_shared<BackGround>();

	//プレイヤークラス
	pl = std::make_shared<Player>();
}

// 描画
void Draw(void)
{
	//画面消去
	ClsDrawScreen();

	back->Draw();
	pl->Draw();

	//裏画面を表画面に瞬間コピー
	ScreenFlip();
}

// 処理
void UpData(void)
{
	Draw();


}

// メイン処理
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	System();
	Create();
	
	//ループ処理
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_RETURN) == 0)
	{
		UpData();
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